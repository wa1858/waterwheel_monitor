#pragma once

#include <array>

namespace waterwheel::hardware {
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

const static std::array<char, 8> kRequestFrequency = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte frequency
    static_cast<char>(0x46),  // Lo byte frequency
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x90), static_cast<char>(0x1E)};

const static std::array<char, 8> kRequestActivePower = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte active power
    static_cast<char>(0x0C),  // Lo byte active power
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0xB1), static_cast<char>(0xC8)};

const static std::array<char, 8> kRequestTotalActiveEnergy = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x01),  // Hi byte total active energy
    static_cast<char>(0x56),  // Lo byte total active energy
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x90), static_cast<char>(0x27)};

const static std::array<char, 8> kRequestVoltage = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte voltage
    static_cast<char>(0x00),  // Lo byte voltage
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x71), static_cast<char>(0xCB)};

const static std::array<char, 8> kRequestCurrent = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte current
    static_cast<char>(0x06),  // Lo byte current
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x91), static_cast<char>(0xCA)};

const static std::array<char, 8> kRequestReactivePower = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte reactive power
    static_cast<char>(0x18),  // Lo byte reactive power
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0xF1), static_cast<char>(0xCC)};

const static std::array<char, 8> kRequestApparentPower = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte apparent power
    static_cast<char>(0x12),  // Lo byte apparent power
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0xD1), static_cast<char>(0xCE)};

const static std::array<char, 8> kRequestPowerFactor = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte power factor
    static_cast<char>(0x1E),  // Lo byte power factor
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x11), static_cast<char>(0xCD)};

const static std::array<char, 8> kRequestPhaseAngle = {
    static_cast<char>(0x01), static_cast<char>(0x04),
    static_cast<char>(0x00),  // Hi byte power factor
    static_cast<char>(0x24),  // Lo byte power factor
    static_cast<char>(0x00), static_cast<char>(0x02),
    static_cast<char>(0x31), static_cast<char>(0xC0)};
}  // namespace waterwheel::hardware
