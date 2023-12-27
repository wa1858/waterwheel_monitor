#pragma once

#include <array>

/**
 * Request Format:
 * - Address of device (currently always 0x01)
 * - Read input registers
 * - High byte for parameter (eg, frequency = 0x00)
 * - Low byte for parameter (eg, frequency = 0x46)
 * - Number of registers high
 * - Number of registers low
 * - Error check low
 * - Error check high
 * Checksum bytes can be computed using online calculators
 */

const static std::array<char, 8> REQUEST_FREQUENCY = {
    static_cast<char>(0x01), // address 01
    static_cast<char>(0x04),
    static_cast<char>(0x00), // Hi byte frequency
    static_cast<char>(0x46), // Lo byte frequency
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0x90),  // Error check low - computed using online calculator
    static_cast<char>(0x1E)}; // Error check high - computed using online calculator

const static std::array<char, 8> REQUEST_ACTIVE_POWER = {
    static_cast<char>(0x01), // address 01
    static_cast<char>(0x04),
    static_cast<char>(0x00), // Hi byte active power
    static_cast<char>(0x0C), // Lo byte active power
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0xB1),  // Error check low - computed using online calculator
    static_cast<char>(0xC8)}; // Error check high - computed using online calculator

const static std::array<char, 8> REQUEST_TOTAL_ACTIVE_ENERGY = {
    static_cast<char>(0x01), // address 01
    static_cast<char>(0x04),
    static_cast<char>(0x01), // Hi Byte total active energy
    static_cast<char>(0x56), // Lo Byte total active energy
    static_cast<char>(0x00),
    static_cast<char>(0x02),
    static_cast<char>(0x90),  // Error check low - computed using online calculator
    static_cast<char>(0x27)}; // Error check high - computed using online calculator