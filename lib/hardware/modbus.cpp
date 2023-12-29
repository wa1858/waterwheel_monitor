#include "modbus.hpp"

namespace waterwheel::hardware {
Modbus::Modbus(int port_number, utils::Logger &logger) : logger_(logger) {
  this->serial_ = this->setupSerial(port_number);
  this->logger_.log(utils::LogLevel::kDebug, "New Modbus created");
}

Modbus::~Modbus() {
  CloseHandle(this->serial_);
  this->logger_.log(utils::LogLevel::kDebug, "Serial connection closed");
}

float Modbus::getData(const std::array<char, 8> &request) {
  writeData(request);
  std::array<char, 9> data;
  readData(data);
  return convertDataToFloat(data);
}

float Modbus::getAverage(float value,
                         std::array<float, kSizeOfAverageArrays> &array) {
  array[average_count_] = value;
  float average = 0.0;
  for (int i = 0; i < kSizeOfAverageArrays; ++i) {
    average += array[i];
  }
  return average / kSizeOfAverageArrays;
}

void Modbus::incrementAverage() {
  ++average_count_;
  if (average_count_ >= kSizeOfAverageArrays) {
    average_count_ = 0;
  }
}

int Modbus::writeData(const std::array<char, 8> &request) {
  DWORD bytes_written = 0;
  if (!WriteFile(this->serial_, request.data(), request.size(), &bytes_written,
                 nullptr)) {
    return 1;
  }
  return 0;
};

// TODO - What should this return if okay?
bool Modbus::readData(std::array<char, 9> &bytes_to_read) {
  DWORD bytes_read = 0;
  bool status = ReadFile(this->serial_, bytes_to_read.data(),
                         bytes_to_read.size(), &bytes_read, nullptr);
  if (!status) {
    logger_.log(utils::LogLevel::kWarning, "Could not read from serial port");
  }
  return status;
}

HANDLE Modbus::setupSerial(int port_number) {
  // Declare variables and structures
  DCB dcbSerialParams = {0};
  COMMTIMEOUTS timeouts = {0};

  // String concatenation
  std::stringstream ss;
  ss << "\\\\.\\COM" << port_number;
  std::string port = ss.str();

  // Open the desired serial port
  logger_.log(utils::LogLevel::kInfo, "Opening serial port COM%d", port_number);

  HANDLE hSerial =
      CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (hSerial == INVALID_HANDLE_VALUE) {
    logger_.log(utils::LogLevel::kFatal, "Error");
    exit(EXIT_FAILURE);
  }
  logger_.log(utils::LogLevel::kInfo, "Port opened successfully");
  // Set device parameters (9600 baud, 1 start bit, 1 stop bit, no parity)
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  if (GetCommState(hSerial, &dcbSerialParams) == 0) {
    logger_.log(utils::LogLevel::kFatal, "Error getting device state");
    CloseHandle(hSerial);
    exit(EXIT_FAILURE);
  }

  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  if (SetCommState(hSerial, &dcbSerialParams) == 0) {
    logger_.log(utils::LogLevel::kFatal, "Error setting device parameters");
    CloseHandle(hSerial);
    exit(EXIT_FAILURE);
  }

  // Set COM port timeout settings
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if (SetCommTimeouts(hSerial, &timeouts) == 0) {
    logger_.log(utils::LogLevel::kFatal, "Error setting timeouts");
    CloseHandle(hSerial);
    exit(EXIT_FAILURE);
  }
  return hSerial;
}

float Modbus::convertDataToFloat(const std::array<char, 9> &bytes_to_read) {
  // Note static_cast forces compiler to treat array members as int, not char
  int bytes[4];
  bytes[0] = static_cast<int>(bytes_to_read[3]);
  bytes[1] = static_cast<int>(bytes_to_read[4]);
  bytes[2] = static_cast<int>(bytes_to_read[5]);
  bytes[3] = static_cast<int>(bytes_to_read[6]);

  // Convert the four integers into a single integer representing a 32-bit
  // number
  uint32_t int_data =
      (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];

  // Assign result to memory assigned for float, resulting in float cast
  float f;
  memcpy(&f, &int_data, sizeof(float));

  return f;
}

float Modbus::getFrequency() { return getData(kRequestFrequency); }

float Modbus::getAverageFrequency(float frequency) {
  return getAverage(frequency, averaging_frequency_data_);
}

// TODO - Find a better audio prompt than MessageBeep
void Modbus::checkAverageFrequency(float average_frequency) {
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

float Modbus::getActivePower() { return getData(kRequestActivePower); }

float Modbus::getAverageActivePower(float active_power) {
  return getAverage(active_power, averaging_power_data_);
}

float Modbus::getTotalActiveEnergy() {
  return getData(kRequestTotalActiveEnergy);
}

float Modbus::getVoltage() { return getData(kRequestVoltage); }

float Modbus::getCurrent() { return getData(kRequestCurrent); }

float Modbus::getReactivePower() { return getData(kRequestReactivePower); }

float Modbus::getApparentPower() { return getData(kRequestApparentPower); }

float Modbus::getPowerFactor() { return getData(kRequestPowerFactor); }

float Modbus::getPhaseAngle() { return getData(kRequestPhaseAngle); }
}  // namespace waterwheel::hardware
