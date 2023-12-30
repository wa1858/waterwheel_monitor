#pragma once

#include <array>
#include <hardware/requests.hpp>
#include <hardware/serial.hpp>
#include <iostream>
#include <sstream>
#include <utils/logger.hpp>
#include <utils/utils.hpp>

namespace waterwheel::hardware {

// Device address can be in range 1-247 inclusive
constexpr static int kDefaultDeviceAddress = 0x01;
constexpr static std::pair<char, char> kFrequency(0x00, 0x46);
constexpr static std::pair<char, char> kActivePower(0x00, 0x0C);
constexpr static std::pair<char, char> kTotalActiveEnergy(0x01, 0x56);
constexpr static std::pair<char, char> kVoltage(0x00, 0x00);
constexpr static std::pair<char, char> kCurrent(0x00, 0x06);
constexpr static std::pair<char, char> kReactivePower(0x00, 0x18);
constexpr static std::pair<char, char> kApparentPower(0x00, 0x12);
constexpr static std::pair<char, char> kPowerFactor(0x00, 0x1E);
constexpr static std::pair<char, char> kPhaseAngle(0x00, 0x24);

constexpr static char kModbusRead = 0x04;
constexpr static char kModbusPointsHi = 0x00;
constexpr static char kModbusPointsLo = 0x02;

// enum class MeterReadings {
//   kFrequency = 0,
//   kActivePower,
//   kTotalActiveEnergy,
//   kVoltage,
//   kCurrent,
//   kReactivePower,
//   kApparentPower,
//   kPowerFactor,
//   kPhaseAngle
// };

class Modbus {
 public:
  Modbus(utils::Logger &logger, int port_number,
         int device_address = kDefaultDeviceAddress);
  ~Modbus();

  float readValue(std::pair<char, char> request_pair);

  float getFrequency();
  float getActivePower();
  float getTotalActiveEnergy();
  float getVoltage();
  float getCurrent();
  float getReactivePower();
  float getApparentPower();
  float getPowerFactor();
  float getPhaseAngle();
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
   * @brief Compute the required checksum bits for a given Modbus data frame
   */
  void computeRequestChecksum(std::array<char, 8> &request_frame);

  /**
   * @brief Send a defined request frame to the meter through MODBUS, retrieve
   * the response from the meter, and convert the result into a float.
   */
  float getData(const std::array<char, 8> &request);

  /**
   * @brief Determine the average value of the elements of an array. Used for
   * calculating average frequency and active power
   */
  float getAverage(float value, std::array<float, 10> &array);

  /**
   * @brief Converts four char bytes to an equivalent 32-bit float. Implemnted
   * as the energy meter generates data in floating point format but program
   * reads data as four distinct bytes
   */
  float convertDataToFloat(const std::array<char, 9> &bytes_to_read);

 private:
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
   * Checksum bytes will be computed by computeRequestChecksum()
   */
  std::array<char, 8> request_frame_ = {static_cast<char>(device_address_),
                                        kModbusRead,
                                        0x00,
                                        0x00,
                                        kModbusPointsHi,
                                        kModbusPointsLo,
                                        0x00,
                                        0x00};

  Serial serial_;
  utils::Logger &logger_;
  int device_address_;
};
}  // namespace waterwheel::hardware
