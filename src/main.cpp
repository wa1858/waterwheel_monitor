#include <cstring>
#include <hardware/modbus.hpp>
#include <hardware/serial.hpp>
#include <utils/debug.hpp>
#include <utils/logger.hpp>
#include <utils/utils.hpp>

// #include "config.h"

int main(int argc, char *argv[]) {
  // Determine if debug mode is enabled
  bool debug_mode = argc > 1 && strcmp(argv[0], waterwheel::utils::kDebugFlag);
  waterwheel::utils::LogLevel level = debug_mode
                                          ? waterwheel::utils::LogLevel::kDebug
                                          : waterwheel::utils::LogLevel::kInfo;

  auto logger = waterwheel::utils::Logger(level);

  logger.log(waterwheel::utils::LogLevel::kInfo,
             "Waterwheel Monitoring Program");
  // TODO - Restructure CMakeLists.txt to allow inclusion of config.h in build
  // logger.log(waterwheel::utils::LogLevel::kDebug, "Revision %d.%d.%d",
  //  PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
  //  PROJECT_VERSION_PATCH);
  // logger.log(waterwheel::utils::LogLevel::kDebug, "Build Date: %s",
  // BUILD_DATE);

  // TODO - Imput validation
  int port_number;
  std::cout << "Enter the desired serial port number: ";
  std::cin >> port_number;
  logger.log(waterwheel::utils::LogLevel::kDebug, "Selected port: COM%d",
             port_number);

  // TODO - Find a better way of calling debug mode functions
  int delay_amount = waterwheel::utils::kDefaultDelay;
  if (debug_mode) {
    waterwheel::utils::debugConfigureDelay(logger, &delay_amount);
  }

  auto monitor = waterwheel::hardware::Modbus(logger, port_number);

  while (true) {
    // Break between each record
    std::cout << std::endl;

    float frequency = monitor.getFrequency();
    float average_frequency = monitor.getAverageFrequency(frequency);
    // TODO - Centering of prints is done with tabs here; is there a better way?
    // TODO - Prints with averages to be moved to the appropriate function in
    // hardware::Modbus
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Frequency (Hz):              %2.1f   Average Frequency (Hz):   "
               "  %2.1f",
               frequency, average_frequency);
    monitor.checkAverageFrequency(average_frequency);

    float active_power = monitor.getActivePower();
    float average_active_power = monitor.getAverageActivePower(active_power);
    logger.log(
        waterwheel::utils::LogLevel::kInfo,
        "Active Power (W):            %2.3f Average Active Power (W): %2.3f",
        active_power, average_active_power);

    float total_active_energy = monitor.getTotalActiveEnergy();
    float voltage = monitor.getVoltage();
    float current = monitor.getCurrent();
    float reactive_power = monitor.getReactivePower();
    float apparent_power = monitor.getApparentPower();
    float power_factor = monitor.getPowerFactor();
    float phase_angle = monitor.getPhaseAngle();

    // TODO - Find a better way to do this
    monitor.incrementAverage();

    waterwheel::utils::delay(delay_amount);
  }

  return 0;
}
