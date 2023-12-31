#include "modbus.hpp"

namespace waterwheel::core {
Modbus::Modbus(core::Logger &logger, int port_number)
    : logger_(logger), serial_(logger, port_number) {
  this->logger_.log(core::LogLevel::kDebug, "New Modbus created");
}

Modbus::~Modbus() {
  this->serial_.hardware::Serial::~Serial();
  this->logger_.log(core::LogLevel::kDebug, "Modbus object destroyed");
}

float Modbus::getData(const std::array<char, 8> &request) {
  serial_.writeData(request);
  std::array<char, 9> data;
  serial_.readData(data);
  return convertDataToFloat(data);
}

float Modbus::getAverage(float value,
                         std::array<float, kSizeOfAverageArrays> &array) {
  array[average_count_] = value;
  float average = 0.0;
  for (int i = 0; i < kSizeOfAverageArrays; ++i) {
    average += array[i];
  }
  return average / kSizeOfAverageArrays;
}

void Modbus::incrementAverage() {
  ++average_count_;
  if (average_count_ >= kSizeOfAverageArrays) {
    average_count_ = 0;
  }
}

float Modbus::convertDataToFloat(const std::array<char, 9> &bytes_to_read) {
  // Note static_cast forces compiler to treat array members as int, not char
  uint8_t bytes[4];
  bytes[0] = static_cast<uint8_t>(bytes_to_read[3]);
  bytes[1] = static_cast<uint8_t>(bytes_to_read[4]);
  bytes[2] = static_cast<uint8_t>(bytes_to_read[5]);
  bytes[3] = static_cast<uint8_t>(bytes_to_read[6]);

  // Convert the four integers into a single integer representing a 32-bit
  // number
  uint32_t int_data =
      (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];

  // Assign result to memory assigned for float, resulting in float cast
  float f;
  memcpy(&f, &int_data, sizeof(float));

  return f;
}

float Modbus::getFrequency() { return getData(hardware::kRequestFrequency); }

float Modbus::getAverageFrequency(float frequency) {
  return getAverage(frequency, averaging_frequency_data_);
}

// TODO - Find a better audio prompt than MessageBeep
void Modbus::checkAverageFrequency(float average_frequency) {
  if (average_frequency > kFrequencyMax) {
    logger_.log(core::LogLevel::kWarning,
                "%2.1fHz average frequency - OVERSPEED WARNING",
                average_frequency);
    MessageBeep(MB_ICONWARNING);
  } else if (average_frequency < kFrequencyMin) {
    logger_.log(core::LogLevel::kWarning,
                "%2.1fHz average frequency - UNDERSPEED WARNING",
                average_frequency);
    MessageBeep(MB_ICONWARNING);
  }
}

float Modbus::getActivePower() {
  return getData(hardware::kRequestActivePower);
}

float Modbus::getAverageActivePower(float active_power) {
  return getAverage(active_power, averaging_power_data_);
}

float Modbus::getTotalActiveEnergy() {
  return getData(hardware::kRequestTotalActiveEnergy);
}

float Modbus::getVoltage() { return getData(hardware::kRequestVoltage); }

float Modbus::getCurrent() { return getData(hardware::kRequestCurrent); }

float Modbus::getReactivePower() {
  return getData(hardware::kRequestReactivePower);
}

float Modbus::getApparentPower() {
  return getData(hardware::kRequestApparentPower);
}

float Modbus::getPowerFactor() {
  return getData(hardware::kRequestPowerFactor);
}

float Modbus::getPhaseAngle() { return getData(hardware::kRequestPhaseAngle); }
}  // namespace waterwheel::core
