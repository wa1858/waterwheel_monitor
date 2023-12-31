#include <core/logger.hpp>
#include <core/modbus.hpp>
#include <cstring>
#include <hardware/serial.hpp>
#include <utils/debug.hpp>
#include <utils/utils.hpp>
// #include "config.h"

int main(int argc, char *argv[]) {
  // Determine if debug mode is enabled
  bool debug_mode = argc > 1 && strcmp(argv[0], waterwheel::utils::kDebugFlag);
  waterwheel::core::LogLevel level = debug_mode
                                         ? waterwheel::core::LogLevel::kDebug
                                         : waterwheel::core::LogLevel::kInfo;

  auto logger = waterwheel::core::Logger(level);

  logger.log(waterwheel::core::LogLevel::kInfo,
             "Waterwheel Monitoring Program");
  // TODO - Restructure CMakeLists.txt to allow inclusion of config.h in build
  // logger.log(core::LogLevel::debug, "Revision %d.%d.%d",
  // PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
  // logger.log(core::LogLevel::debug, "Build Date: %s", BUILD_DATE);

  // TODO - Imput validation
  int port_number;
  logger.log(waterwheel::core::LogLevel::kInfo,
             "Enter the desired serial port number: ");
  std::cin >> port_number;
  logger.log(waterwheel::core::LogLevel::kDebug, "Selected port: COM%d",
             port_number);

  // TODO - Find a better way of calling debug mode functions
  int delay_amount = waterwheel::utils::kDefaultDelay;
  if (debug_mode) {
    waterwheel::utils::debugConfigureDelay(logger, &delay_amount);
  }

  auto monitor = waterwheel::core::Modbus(logger, port_number);

  // TODO - Can this big loop with print statements be cleaned up?
  while (true) {
    // Break between each record
    std::cout << std::endl;

    float frequency = monitor.getFrequency();
    float average_frequency = monitor.getAverageFrequency(frequency);
    // TODO - The centering of the output prints is done using tabs here; is
    // there a better way?
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Frequency (Hz):              %2.1f   Average Frequency (Hz):   "
               "  %2.1f",
               frequency, average_frequency);
    monitor.checkAverageFrequency(average_frequency);

    float active_power = monitor.getActivePower();
    float average_active_power = monitor.getAverageActivePower(active_power);
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Active Power (W): %2.3f%sAverage Active Power (W): %2.3f",
               active_power, waterwheel::core::kWhitespace,
               average_active_power);

    float total_active_energy = monitor.getTotalActiveEnergy();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Total Active Energy (kWh):   %2.0f", total_active_energy);

    float voltage = monitor.getVoltage();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Voltage (V):                 %2.1f", voltage);

    float current = monitor.getCurrent();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Current (A):                 %2.3f", current);

    float reactive_power = monitor.getReactivePower();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Reactive Power (VAr):        %2.3f", reactive_power);

    float apparent_power = monitor.getApparentPower();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Apparent Power (VA):         %2.3f", apparent_power);

    float power_factor = monitor.getPowerFactor();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Power Factor:                %2.3f", power_factor);

    float phase_angle = monitor.getPhaseAngle();
    logger.log(waterwheel::core::LogLevel::kInfo,
               "Phase Angle (degrees):       %2.3f", phase_angle);

    // TODO - Find a better way to do this
    monitor.incrementAverage();

    waterwheel::utils::delay(delay_amount);
  }

  return 0;
}
