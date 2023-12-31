#pragma once

#include <windows.h>

#include <array>
#include <core/logger.hpp>

namespace waterwheel::hardware {
// TODO - Return optional type for constructors?
class Serial {
 public:
  Serial(core::Logger &logger, int portNumber);
  ~Serial();

  /**
   * @brief Select the serial port to be used at the start of the program
   */
  static int selectSerialPort(core::Logger &logger);

  /**
   * @brief Send a data request through the serial port
   */
  void writeData(const std::array<char, 8> &request);

  /**
   * @brief Retrieve data from the serial port and store the result
   */
  void readData(std::array<char, 9> &result);

 private:
  /**
   * @brief Create a HANDLE variable for accessing the serial port
   */
  HANDLE setUpSerial(int portNumber);

  // TODO - static getSerialPorts();

 private:
  core::Logger &logger_;
  HANDLE serial_;
};
}  // namespace waterwheel::hardware