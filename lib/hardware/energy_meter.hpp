#pragma once

#include <windows.h>

#include <hardware/modbus.hpp>

namespace waterwheel::hardware {

// Start Address Hi and Lo for each Modbus request, as defined in datasheet
constexpr static std::pair<uint8_t, uint8_t> kRequestFrequency(0x00, 0x46);
constexpr static std::pair<uint8_t, uint8_t> kRequestActivePower(0x00, 0x0C);
constexpr static std::pair<uint8_t, uint8_t> kRequestTotalActiveEnergy(0x01,
                                                                       0x56);
constexpr static std::pair<uint8_t, uint8_t> kRequestVoltage(0x00, 0x00);
constexpr static std::pair<uint8_t, uint8_t> kRequestCurrent(0x00, 0x06);
constexpr static std::pair<uint8_t, uint8_t> kRequestReactivePower(0x00, 0x18);
constexpr static std::pair<uint8_t, uint8_t> kRequestApparentPower(0x00, 0x12);
constexpr static std::pair<uint8_t, uint8_t> kRequestPowerFactor(0x00, 0x1E);
constexpr static std::pair<uint8_t, uint8_t> kRequestPhaseAngle(0x00, 0x24);

class EnergyMeter : Modbus {
 public:
  EnergyMeter(utils::Logger &logger, uint8_t port_number,
              uint8_t device_address = kDefaultDeviceAddress);
  ~EnergyMeter();

  /**
   * @brief Construct and send a read request frame to the meter via Modbus,
   * and return the result
   */
  float requestData(std::pair<uint8_t, uint8_t> request_pair);

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
   * @brief Determine the average value of the elements of an array. Used for
   * calculating average frequency and active power
   */
  float getAverage(float value, std::array<float, 10> &array);

 private:
  // Additional Modbus frame constants from datasheet
  constexpr static uint8_t kEnergyMeterRead = 0x04;
  constexpr static uint8_t kEnergyMeterPointsHi = 0x00;
  constexpr static uint8_t kEnergyMeterPointsLo = 0x02;

  constexpr static int kSizeOfAverageArrays = 10;
  constexpr static float kFrequencyMin = 44.5;
  constexpr static float kFrequencyMax = 48.5;
  std::array<float, kSizeOfAverageArrays> averaging_frequency_data_ = {};
  std::array<float, kSizeOfAverageArrays> averaging_power_data_ = {};
  int average_count_ = 0;

  utils::Logger logger_;
};
}  // namespace waterwheel::hardware