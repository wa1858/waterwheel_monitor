#pragma once

#include <stdint.h>
#include <windows.h>

#include <array>
#include <utils/logger.hpp>

namespace waterwheel::hardware {
// TODO - Return optional type for constructors?
class Serial {
 public:
  Serial(utils::Logger &logger, int portNumber);
  ~Serial();

  /**
   * @brief Select the serial port to be used at the start of the program
   */
  static int selectSerialPort(utils::Logger &logger);

  /**
   * @brief Send a data request through the serial port
   */
  void writeData(const std::array<uint8_t, 8> &request);

  /**
   * @brief Retrieve data from the serial port and store the result
   */
  void readData(std::array<uint8_t, 9> &response);

 private:
  /**
   * @brief Create a HANDLE variable for accessing the serial port
   */
  HANDLE setUpSerial(int portNumber);

  // TODO - static getSerialPorts();

 private:
  utils::Logger &logger_;
  HANDLE serial_;
};
}  // namespace waterwheel::hardware