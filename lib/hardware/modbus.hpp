#pragma once

#include <string.h>
#include <windows.h>

#include <array>
#include <hardware/requests.hpp>
#include <iostream>
#include <sstream>
#include <utils/logger.hpp>
#include <utils/utils.hpp>

namespace waterwheel::hardware {

class Modbus {
 public:
  Modbus(int port_number, utils::Logger &logger);
  ~Modbus();
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
   * @brief Send a defined request frame to the meter through MODBUS, retrieve
   * the response from the meter, and convert the result into a float.
   */
  float getData(const std::array<char, 8> &request);

  /**
   * @brief Determine the average value of the elements of an array. Used for
   * calculating average frequency and active power
   */
  float getAverage(float value, std::array<float, 10> &array);

  int writeData(const std::array<char, 8> &request);

  bool readData(std::array<char, 9> &bytes_to_read);
  /**
   * @brief Create a HANDLE variable for accessing the serial port
   */
  HANDLE setupSerial(int port_number);
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

  HANDLE serial_;
  utils::Logger &logger_;
};
}  // namespace waterwheel::hardware
