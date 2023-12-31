#pragma once

#include <stdint.h>

#include <array>
#include <core/logger.hpp>
#include <core/serial.hpp>
#include <iostream>
#include <sstream>
#include <utils/utils.hpp>

namespace waterwheel::core {
// TODO - Check that no two devices have the same address on construction
// Device address can be in range 1-247 inclusive
constexpr static uint8_t kDefaultDeviceAddress = 0x01;

class Modbus {
 protected:
  Modbus(Logger &logger, uint8_t port_number,
         uint8_t device_address = kDefaultDeviceAddress);
  ~Modbus();

  /**
   * @brief Send a Modbus request frame and process the result
   */
  float sendRequest(std::array<uint8_t, 8> request_frame);

  uint8_t getDeviceAddress();

 private:
  /**
   * @brief Compute the required checksum bits for a given Modbus data frame
   * (CRC-16)
   */
  void computeRequestChecksum(std::array<uint8_t, 8> &request_frame);

  /**
   * @brief Converts four bytes from the Modbus repsonse into an equivalent
   * 32-bit float. Implemnted as the energy meter generates data in floating
   * point format spread across four bytes
   */
  float convertDataToFloat(const std::array<uint8_t, 9> &data_frame);

 private:
  /**
   * Request Format:
   * - Address of device
   * - Function Code (0x04 for read data)
   * - High byte for parameter (eg, frequency = 0x00)
   * - Low byte for parameter (eg, frequency = 0x46)
   * - Number of registers high
   * - Number of registers low
   * - Error check low
   * - Error check high
   */
  std::array<uint8_t, 8> request_frame_ = {};

  Serial serial_;
  Logger &logger_;
  uint8_t device_address_;
};
}  // namespace waterwheel::core
