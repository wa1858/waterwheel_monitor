#include "modbus.hpp"

namespace waterwheel::hardware {
Modbus::Modbus(utils::Logger &logger, int port_number, int device_address)
    : logger_(logger),
      serial_(logger, port_number),
      device_address_(device_address) {
  this->logger_.log(utils::LogLevel::kDebug, "New Modbus(%d) created",
                    device_address_);
}

Modbus::~Modbus() {
  this->serial_.~Serial();
  this->logger_.log(utils::LogLevel::kDebug, "Modbus(%d) object destroyed",
                    device_address_);
}

float Modbus::readValue(std::pair<uint8_t, uint8_t> request_pair) {
  // Construct request frame
  request_frame_[0] = device_address_;
  request_frame_[2] = request_pair.first;
  request_frame_[3] = request_pair.second;
  computeRequestChecksum(request_frame_);
  logger_.log(
      utils::LogLevel::kDebug,
      "Frame constructed: {0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x}",
      request_frame_[0], request_frame_[1], request_frame_[2],
      request_frame_[3], request_frame_[4], request_frame_[5],
      request_frame_[6], request_frame_[7]);

  // Send request and receive response
  serial_.writeData(request_frame_);
  std::array<uint8_t, 9> response_frame;
  serial_.readData(response_frame);
  return convertDataToFloat(response_frame);
}

// Implemented based on Modbus documentation, Section 3.5.2
void Modbus::computeRequestChecksum(std::array<uint8_t, 8> &request_frame) {
  // Preload 16-bit register with all 1s
  uint16_t reg = 0xFFFF;
  // For every byte (except the two error checking bytes being computed)
  for (int i = 0; i < request_frame.size() - 2; ++i) {
    // XOR reg with byte
    reg ^= request_frame[i];
    // For every bit in the byte
    for (int j = 0; j < 8; ++j) {
      if (reg & 0x001) {
        // 0xA001 value from Modbus documentation
        reg = (reg >> 1) ^ 0xA001;
      } else {
        reg = reg >> 1;
      }
    }
  }
  // Split result into two checksum bytes
  request_frame[6] = (reg & 0xFF);
  request_frame[7] = ((reg >> 8) & 0xFF);
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

float Modbus::convertDataToFloat(const std::array<uint8_t, 9> &data_frame) {
  // Convert four data bytes into a single 32-bit number
  uint32_t data = (data_frame[3] << 24) + (data_frame[4] << 16) +
                  (data_frame[5] << 8) + data_frame[6];

  // Assign result to memory assigned for float, resulting in float cast
  float f;
  memcpy(&f, &data, sizeof(f));

  return f;
}

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

float Modbus::getAverageActivePower(float active_power) {
  return getAverage(active_power, averaging_power_data_);
}
}  // namespace waterwheel::hardware
