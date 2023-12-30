#include <cstring>
#include <hardware/energy_meter.hpp>
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

  auto energy_meter =
      waterwheel::hardware::EnergyMeter(logger, port_number, device_address);

  // TODO - Can this big loop with print statements be cleaned up?
  while (true) {
    // Break between each record
    std::cout << std::endl;

    float frequency =
        energy_meter.requestData(waterwheel::hardware::kRequestFrequency);
    float average_frequency = energy_meter.getAverageFrequency(frequency);
    // TODO - The centering of the output prints is done using tabs here; is
    // there a better way?
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Frequency (Hz):              %2.1f   Average Frequency (Hz):   "
               "  %2.1f",
               frequency, average_frequency);
    energy_meter.checkAverageFrequency(average_frequency);

    float active_power =
        energy_meter.requestData(waterwheel::hardware::kRequestActivePower);
    float average_active_power =
        energy_meter.getAverageActivePower(active_power);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Active Power (W): %2.3f%sAverage Active Power (W): %2.3f",
               active_power, waterwheel::utils::kWhitespace,
               average_active_power);

    float total_active_energy = energy_meter.requestData(
        waterwheel::hardware::kRequestTotalActiveEnergy);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Total Active Energy (kWh):   %2.0f", total_active_energy);

    float voltage =
        energy_meter.requestData(waterwheel::hardware::kRequestVoltage);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Voltage (V):                 %2.1f", voltage);

    float current =
        energy_meter.requestData(waterwheel::hardware::kRequestCurrent);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Current (A):                 %2.3f", current);

    float reactive_power =
        energy_meter.requestData(waterwheel::hardware::kRequestReactivePower);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Reactive Power (VAr):        %2.3f", reactive_power);

    float apparent_power =
        energy_meter.requestData(waterwheel::hardware::kRequestApparentPower);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Apparent Power (VA):         %2.3f", apparent_power);

    float power_factor =
        energy_meter.requestData(waterwheel::hardware::kRequestPowerFactor);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Power Factor:                %2.3f", power_factor);

    float phase_angle =
        energy_meter.requestData(waterwheel::hardware::kRequestPhaseAngle);
    logger.log(waterwheel::utils::LogLevel::kInfo,
               "Phase Angle (degrees):       %2.3f", phase_angle);

    // TODO - Find a better way to do this
    energy_meter.incrementAverage();

    waterwheel::utils::delay(delay_amount);
  }

  return 0;
}
