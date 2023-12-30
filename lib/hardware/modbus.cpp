#include "modbus.hpp"

namespace waterwheel::hardware {
Modbus::Modbus(utils::Logger &logger, int port_number)
    : logger_(logger), serial_(logger, port_number) {
  logger_.log(utils::LogLevel::kDebug, "New Modbus created");
}

Modbus::~Modbus() {
  serial_.~Serial();
  logger_.log(utils::LogLevel::kDebug, "Modbus object destroyed");
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
  int bytes[4];
  bytes[0] = static_cast<int>(bytes_to_read[3]);
  bytes[1] = static_cast<int>(bytes_to_read[4]);
  bytes[2] = static_cast<int>(bytes_to_read[5]);
  bytes[3] = static_cast<int>(bytes_to_read[6]);

  // Convert the four integers into a single integer representing a 32-bit
  // number
  uint32_t int_data =
      (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];

  // Assign result to memory assigned for float, resulting in float cast
  float f;
  memcpy(&f, &int_data, sizeof(float));

  return f;
}

float Modbus::getFrequency() { return getData(kRequestFrequency); }

float Modbus::getAverageFrequency(float frequency) {
  return getAverage(frequency, averaging_frequency_data_);
}

// TODO - Find a better audio prompt than MessageBeep
void Modbus::checkAverageFrequency(float average_frequency) {
  if (average_frequency > kFrequencyMax) {
    logger_.log(utils::LogLevel::kWarning,
                "%2.1fHz average frequency - OVERSPEED WARNING",
                average_frequency);
    MessageBeep(MB_ICONWARNING);
  } else if (average_frequency < kFrequencyMin) {
    logger_.log(utils::LogLevel::kWarning,
                "%2.1fHz average frequency - UNDERSPEED WARNING",
                average_frequency);
    MessageBeep(MB_ICONWARNING);
  }
}

float Modbus::getActivePower() { return getData(kRequestActivePower); }

float Modbus::getAverageActivePower(float active_power) {
  return getAverage(active_power, averaging_power_data_);
}

float Modbus::getTotalActiveEnergy() {
  float total_active_energy = getData(kRequestTotalActiveEnergy);
  logger_.log(utils::LogLevel::kInfo, "Total Active Energy (kWh):   %2.0f",
              total_active_energy);
  return total_active_energy;
}

float Modbus::getVoltage() {
  float voltage = getData(kRequestVoltage);
  logger_.log(utils::LogLevel::kInfo, "Voltage (V):                 %2.1f",
              voltage);
  return voltage;
}

float Modbus::getCurrent() {
  float current = getData(kRequestCurrent);
  logger_.log(utils::LogLevel::kInfo, "Current (A):                 %2.3f",
              current);
  return current;
}

float Modbus::getReactivePower() {
  float reactive_power = getData(kRequestReactivePower);
  logger_.log(utils::LogLevel::kInfo, "Reactive Power (VAr):        %2.3f",
              reactive_power);
  return reactive_power;
}

float Modbus::getApparentPower() {
  float apparent_power = getData(kRequestApparentPower);
  logger_.log(utils::LogLevel::kInfo, "Apparent Power (VA):         %2.3f",
              apparent_power);
  return apparent_power;
}

float Modbus::getPowerFactor() {
  float power_factor = getData(kRequestPowerFactor);
  logger_.log(utils::LogLevel::kInfo, "Power Factor:                %2.3f",
              power_factor);
  return power_factor;
}

float Modbus::getPhaseAngle() {
  float phase_angle = getData(kRequestPhaseAngle);
  logger_.log(utils::LogLevel::kInfo, "Phase Angle (degrees):       %2.3f",
              phase_angle);
  return phase_angle;
}
}  // namespace waterwheel::hardware
