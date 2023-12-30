#pragma once

#include <stdint.h>

#include <array>
#include <hardware/serial.hpp>
#include <iostream>
#include <sstream>
#include <utils/logger.hpp>
#include <utils/utils.hpp>

namespace waterwheel::hardware {

// TODO - Restructure for use with many different Modbus peripherals?
// Device address can be in range 1-247 inclusive
constexpr static uint8_t kDefaultDeviceAddress = 0x01;

// Start Address Hi and Lo for each Modbus request, as defined in datasheet
constexpr static std::pair<uint8_t, uint8_t> kFrequency(0x00, 0x46);
constexpr static std::pair<uint8_t, uint8_t> kActivePower(0x00, 0x0C);
constexpr static std::pair<uint8_t, uint8_t> kTotalActiveEnergy(0x01, 0x56);
constexpr static std::pair<uint8_t, uint8_t> kVoltage(0x00, 0x00);
constexpr static std::pair<uint8_t, uint8_t> kCurrent(0x00, 0x06);
constexpr static std::pair<uint8_t, uint8_t> kReactivePower(0x00, 0x18);
constexpr static std::pair<uint8_t, uint8_t> kApparentPower(0x00, 0x12);
constexpr static std::pair<uint8_t, uint8_t> kPowerFactor(0x00, 0x1E);
constexpr static std::pair<uint8_t, uint8_t> kPhaseAngle(0x00, 0x24);

class Modbus {
 public:
  Modbus(utils::Logger &logger, int port_number,
         int device_address = kDefaultDeviceAddress);
  ~Modbus();

  /**
   * @brief Construct and send a read request frame to the meter via Modbus,
   * retrieve the response, and convert the result into a float.
   */
  float readValue(std::pair<uint8_t, uint8_t> request_pair);

  /**
   * @brief Use a set number of previous readings (kSizeOfAverageArrays) to
   * determine the average frequency.
   */
  float getAverageFrequency(float frequency);

  /**
   * @brief Use a set number of previous readings (kSizeOfAverageArrays) to
   * determine the average active power.
   */
  float getAverageActivePower(float active_power);

  /**
   * @brief Check that the average frequency is within the upper and lower
   * limits defined by kFrequencyMax and kFrequencyMin. If outwith these limits,
   * warn the user.
   */
  void checkAverageFrequency(float average_frequency);

  /**
   * @brief Increment the Class-defined array index for average value arrays
   */
  void incrementAverage();

 private:
  /**
   * @brief Compute the required checksum bits for a given MODBUS data frame
   * (CRC-16)
   */
  void computeRequestChecksum(std::array<uint8_t, 8> &request_frame);

  /**
   * @brief Determine the average value of the elements of an array. Used for
   * calculating average frequency and active power
   */
  float getAverage(float value, std::array<float, 10> &array);

  /**
   * @brief Converts four bytes from the Modbus repsonse into an equivalent
   * 32-bit float. Implemnted as the energy meter generates data in floating
   * point format spread across four bytes
   */
  float convertDataToFloat(const std::array<uint8_t, 9> &data_frame);

 private:
  // Additional Modbus frame constants from datasheet
  constexpr static uint8_t kModbusRead = 0x04;
  constexpr static uint8_t kModbusPointsHi = 0x00;
  constexpr static uint8_t kModbusPointsLo = 0x02;

  constexpr static int kSizeOfAverageArrays = 10;
  constexpr static float kFrequencyMin = 44.5;
  constexpr static float kFrequencyMax = 48.5;
  std::array<float, kSizeOfAverageArrays> averaging_frequency_data_ = {};
  std::array<float, kSizeOfAverageArrays> averaging_power_data_ = {};
  int average_count_ = 0;

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
   * Dynamic values initialised as 0x00
   */
  std::array<uint8_t, 8> request_frame_ = {
      0x00, kModbusRead, 0x00, 0x00, kModbusPointsHi, kModbusPointsLo,
      0x00, 0x00};

  Serial serial_;
  utils::Logger &logger_;
  uint8_t device_address_;
};
}  // namespace waterwheel::hardware
