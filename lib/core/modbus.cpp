#include "modbus.hpp"

namespace waterwheel::core {
Modbus::Modbus(Logger &logger, uint8_t port_number, uint8_t device_address)
    : logger_(logger),
      serial_(logger, port_number),
      device_address_(device_address) {
  logger_.log(LogLevel::kDebug, "New Modbus(%d) created", device_address_);
}

Modbus::~Modbus() {
  logger_.log(LogLevel::kDebug, "Modbus(%d) object destroyed", device_address_);
}

float Modbus::sendRequest(std::array<uint8_t, 8> request_frame) {
  computeRequestChecksum(request_frame);
  logger_.log(
      LogLevel::kDebug,
      "Frame constructed: {0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x}",
      request_frame[0], request_frame[1], request_frame[2], request_frame[3],
      request_frame[4], request_frame[5], request_frame[6], request_frame[7]);

  // Send request and receive response
  serial_.writeData(request_frame);
  std::array<uint8_t, 9> response_frame = {};
  serial_.readData(response_frame);
  return convertDataToFloat(response_frame);
}

uint8_t Modbus::getDeviceAddress() { return device_address_; }

// Implemented based on RS energy meter Modbus documentation, Section 3.5.2
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

float Modbus::convertDataToFloat(const std::array<uint8_t, 9> &data_frame) {
  // Convert four data bytes into a single 32-bit number
  // uint32_t data = (data_frame[3] << 24) + (data_frame[4] << 16) +
  // (data_frame[5] << 8) + data_frame[6];

  // Assign result to memory assigned for float, resulting in float cast
  // float f;
  // memcpy(&f, &data, sizeof(f));

  modbus_data i;

  i.data[0] = data_frame[6];
  i.data[1] = data_frame[5];
  i.data[2] = data_frame[4];
  i.data[3] = data_frame[3];

  return i.result;
  // return f;
}
}  // namespace waterwheel::core
