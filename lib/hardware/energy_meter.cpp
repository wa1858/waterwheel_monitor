#include "energy_meter.hpp"

namespace waterwheel::hardware {

EnergyMeter::EnergyMeter(utils::Logger &logger, uint8_t port_number,
                         uint8_t device_address)
    : Modbus(logger, port_number, device_address), logger_(logger) {
  logger_.log(utils::LogLevel::kDebug, "New Energy Meter (%d) created",
              device_address);
}

EnergyMeter::~EnergyMeter() {
  logger_.log(utils::LogLevel::kDebug, "Energy Meter (%d) destroyed",
              getDeviceAddress());
}

float EnergyMeter::requestData(std::pair<uint8_t, uint8_t> request_pair) {
  // Checksums handled by Modbus class
  std::array<uint8_t, 8> frame = {getDeviceAddress(),
                                  kEnergyMeterRead,
                                  request_pair.first,
                                  request_pair.second,
                                  kEnergyMeterPointsHi,
                                  kEnergyMeterPointsLo,
                                  0,
                                  0};
  return sendRequest(frame);
}

float EnergyMeter::getAverage(float value,
                              std::array<float, kSizeOfAverageArrays> &array) {
  array[average_count_] = value;
  float average = 0.0;
  for (int i = 0; i < kSizeOfAverageArrays; ++i) {
    average += array[i];
  }
  return average / kSizeOfAverageArrays;
}

void EnergyMeter::incrementAverage() {
  ++average_count_;
  if (average_count_ >= kSizeOfAverageArrays) {
    average_count_ = 0;
  }
}

float EnergyMeter::getAverageFrequency(float frequency) {
  return getAverage(frequency, averaging_frequency_data_);
}

// TODO - Find a better audio prompt than MessageBeep
void EnergyMeter::checkAverageFrequency(float average_frequency) {
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

float EnergyMeter::getAverageActivePower(float active_power) {
  return getAverage(active_power, averaging_power_data_);
}
}  // namespace waterwheel::hardware