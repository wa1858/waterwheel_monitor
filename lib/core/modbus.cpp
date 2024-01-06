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
  // TODO - Implement full error checking on Modbus response
  std::array<uint8_t, 9> response_frame = {};
  serial_.readData(response_frame);

  // Handle error response if applicable
  // Condition for error response defined in Modbus specification
  if (response_frame[1] == 0x10000000 | request_frame[1]) {
    return handleResponseError(response_frame);
  }
  return convertDataToFloat(response_frame);
}

// TODO - Implement proper error handling
float Modbus::handleResponseError(std::array<uint8_t, 9> error_frame) {
  std::string error_message;
  uint8_t error_code = error_frame[2];
  switch (error_code) {
    case ModbusErrorCodes::kIllegalFunction:
      error_message = "Illegal Function";
      break;
    case ModbusErrorCodes::kIllegalDataAddress:
      error_message = "Illegal Data Address";
      break;
    case ModbusErrorCodes::kIllegalDataValue:
      error_message = "Illegal Data Value";
      break;
    case ModbusErrorCodes::kDeviceFailure:
      error_message = "Device Failure";
      break;
    case ModbusErrorCodes::kAcknowledge:
      error_message = "Acknowledged; Device is processing request...";
      break;
    case ModbusErrorCodes::kDeviceBusy:
      error_message = "Device Busy";
      break;
    case ModbusErrorCodes::kNegativeAcknowledge:
      error_message = "Negative Acknowledge";
      break;
    case ModbusErrorCodes::kMemoryParityError:
      error_message = "Memory Parity Error";
      break;
    case ModbusErrorCodes::kNoError:
    default:
      error_message = "Unknown Error";
      break;
  }
  logger_.log(LogLevel::kWarning, "Modbus device 0x%x response error: 0x%x; %s",
              error_frame[0], error_code, error_message.c_str());
  return 0.0;
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
  modbus_data data = {data_frame[6], data_frame[5], data_frame[4],
                      data_frame[3]};
  return data.result;
}
}  // namespace waterwheel::core
