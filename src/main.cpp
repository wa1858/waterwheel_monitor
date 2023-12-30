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
  // logger.log(utils::LogLevel::debug, "Revision %d.%d.%d",
  // PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
  // logger.log(utils::LogLevel::debug, "Build Date: %s", BUILD_DATE);

  // TODO - Imput validation
  int port_number;
  logger.log(waterwheel::utils::LogLevel::kInfo,
             "Enter the desired serial port number: ");
  std::cin >> port_number;
  logger.log(waterwheel::utils::LogLevel::kDebug, "Selected port: COM%d",
             port_number);

  // TODO - Find a better way of calling debug mode functions
  int delay_amount = waterwheel::utils::kDefaultDelay;
  int device_address = waterwheel::hardware::kDefaultDeviceAddress;
  if (debug_mode) {
    waterwheel::utils::debugConfigureDelay(logger, &delay_amount);
    // TODO - debugSetModbusAddress();
  }

  auto monitor =
      waterwheel::hardware::Modbus(logger, port_number, device_address);

  // TODO - Can this big loop with print statements be cleaned up?
  while (true) {
    // Break between each record
    std::cout << std::endl;

    float frequency = monitor.readValue(waterwheel::hardware::kFrequency);
    float average_frequency = monitor.getAverageFrequency(frequency);
    // TODO - The centering of the output prints is done using tabs here; is
    // there a better way?
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Frequency (Hz):              %2.1f   Average Frequency (Hz):   "
               "  %2.1f",
               frequency, average_frequency);
    monitor.checkAverageFrequency(average_frequency);

    float active_power = monitor.readValue(waterwheel::hardware::kActivePower);
    float average_active_power = monitor.getAverageActivePower(active_power);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Active Power (W): %2.3f%sAverage Active Power (W): %2.3f",
               active_power, waterwheel::utils::kWhitespace,
               average_active_power);

    float total_active_energy =
        monitor.readValue(waterwheel::hardware::kTotalActiveEnergy);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Total Active Energy (kWh):   %2.0f", total_active_energy);

    float voltage = monitor.readValue(waterwheel::hardware::kVoltage);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Voltage (V):                 %2.1f", voltage);

    float current = monitor.readValue(waterwheel::hardware::kCurrent);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Current (A):                 %2.3f", current);

    float reactive_power =
        monitor.readValue(waterwheel::hardware::kReactivePower);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Reactive Power (VAr):        %2.3f", reactive_power);

    float apparent_power =
        monitor.readValue(waterwheel::hardware::kApparentPower);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Apparent Power (VA):         %2.3f", apparent_power);

    float power_factor = monitor.readValue(waterwheel::hardware::kPowerFactor);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Power Factor:                %2.3f", power_factor);

    float phase_angle = monitor.readValue(waterwheel::hardware::kPhaseAngle);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Phase Angle (degrees):       %2.3f", phase_angle);

    // TODO - Find a better way to do this
    monitor.incrementAverage();

    waterwheel::utils::delay(delay_amount);
  }

  return 0;
}
