#pragma once

#include <array>

/**
 * Request Format:
 * - Address of device (currently always 0x01)
 * - Function (0x04 for read data)
 * - High byte for parameter (eg, frequency = 0x00)
 * - Low byte for parameter (eg, frequency = 0x46)
 * - Number of registers high
 * - Number of registers low
 * - Error check low
 * - Error check high
 * Checksum bytes can be computed using online calculators
 */

const static std::array<char, 8> REQUEST_FREQUENCY = {
    static_cast<char>(0x01),
    static_cast<char>(0x04),
    static_cast<char>(0x00), // Hi byte frequency
    static_cast<char>(0x46), // Lo byte frequency
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0x90),
    static_cast<char>(0x1E)};

const static std::array<char, 8> REQUEST_ACTIVE_POWER = {
    static_cast<char>(0x01),
    static_cast<char>(0x04),
    static_cast<char>(0x00), // Hi byte active power
    static_cast<char>(0x0C), // Lo byte active power
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0xB1),
    static_cast<char>(0xC8)};

const static std::array<char, 8> REQUEST_TOTAL_ACTIVE_ENERGY = {
    static_cast<char>(0x01),
    static_cast<char>(0x04),
    static_cast<char>(0x01), // Hi Byte total active energy
    static_cast<char>(0x56), // Lo Byte total active energy
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0x90),
    static_cast<char>(0x27)};

const static std::array<char, 8> REQUEST_VOLTAGE = {
    static_cast<char>(0x01),
    static_cast<char>(0x04),
    static_cast<char>(0x00), // Hi Byte voltage
    static_cast<char>(0x00), // Lo Byte voltage
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0x71),
    static_cast<char>(0xCB)};