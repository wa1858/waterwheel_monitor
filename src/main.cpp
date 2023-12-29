// Program to monitor performance of the waterwheel

#include <hardware/modbus.hpp>
#include <utils/logger.hpp>
#include <utils/utils.hpp>
// #include "config.h"

int main() {
  auto logger = waterwheel::utils::Logger(waterwheel::utils::LogLevel::kDebug);
  logger.log(waterwheel::utils::LogLevel::kInfo,
             "Waterwheel Monitoring Program");
  // TODO - Restructure CMakeLists.txt to allow inclusion of config.h in build
  // logger.log(utils::LogLevel::kDebug, "Revision %d.%d.%d",
  // PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
  // logger.log(utils::LogLevel::kDebug, "Build Date: %s", BUILD_DATE);

  // TODO - Imput validation
  int port_number;
  logger.log(waterwheel::utils::LogLevel::kInfo,
             "Enter the desired serial port number: ");
  std::cin >> port_number;
  logger.log(waterwheel::utils::LogLevel::kDebug, "Selected port: COM%d",
             port_number);

  auto monitor = waterwheel::hardware::Modbus(port_number, logger);

  while (true) {
    logger.log(waterwheel::utils::LogLevel::kInfo, "");
    float frequency = monitor.getFrequency();
    float average_frequency = monitor.getAverageFrequency(frequency);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Frequency (Hz): %2.1f%sAverage Frequency (Hz): %2.1f",
               frequency, waterwheel::utils::kWhitespace, average_frequency);
    monitor.checkAverageFrequency(average_frequency);

    float active_power = monitor.getActivePower();
    float average_active_power = monitor.getAverageActivePower(active_power);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Active Power (W): %2.3f%sAverage Active Power (W): %2.3f",
               active_power, waterwheel::utils::kWhitespace,
               average_active_power);

    float total_active_energy = monitor.getTotalActiveEnergy();
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Total Active Energy (kWh): %2.0f", total_active_energy);

    float voltage = monitor.getVoltage();
    logger.log(waterwheel::utils::LogLevel::kInfo, "Voltage (V): %2.1f",
               voltage);

    float current = monitor.getCurrent();
    logger.log(waterwheel::utils::LogLevel::kInfo, "Current (A): %2.3f",
               current);

    float reactive_power = monitor.getReactivePower();
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Reactive Power (VAr): %2.3f", reactive_power);

    float apparent_power = monitor.getApparentPower();
    logger.log(waterwheel::utils::LogLevel::kInfo, "Apparent Power (VA): %2.3f",
               apparent_power);

    float power_factor = monitor.getPowerFactor();
    logger.log(waterwheel::utils::LogLevel::kInfo, "Power Factor: %2.3f",
               power_factor);

    float phase_angle = monitor.getPhaseAngle();
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Phase Angle (degrees): %2.3f", phase_angle);

    // TODO - Find a better way to do this
    monitor.incrementAverage();

    waterwheel::utils::delay(1800);
  }

  return 0;
}
