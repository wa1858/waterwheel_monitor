#pragma once

namespace waterwheel::hardware
{
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

    static constexpr std::array<char, 8> REQUEST_FREQUENCY = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte frequency
        static_cast<char>(0x46), // Lo byte frequency
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x90),
        static_cast<char>(0x1E)};

    static constexpr std::array<char, 8> REQUEST_ACTIVE_POWER = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte active power
        static_cast<char>(0x0C), // Lo byte active power
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0xB1),
        static_cast<char>(0xC8)};

    static constexpr std::array<char, 8> REQUEST_TOTAL_ACTIVE_ENERGY = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x01), // Hi byte total active energy
        static_cast<char>(0x56), // Lo byte total active energy
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x90),
        static_cast<char>(0x27)};

    static constexpr std::array<char, 8> REQUEST_VOLTAGE = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte voltage
        static_cast<char>(0x00), // Lo byte voltage
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x71),
        static_cast<char>(0xCB)};

    static constexpr std::array<char, 8> REQUEST_CURRENT = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte current
        static_cast<char>(0x06), // Lo byte current
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x91),
        static_cast<char>(0xCA)};

    static constexpr std::array<char, 8> REQUEST_REACTIVE_POWER = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte reactive power
        static_cast<char>(0x18), // Lo byte reactive power
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0xF1),
        static_cast<char>(0xCC)};

    static constexpr std::array<char, 8> REQUEST_APPARENT_POWER = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte apparent power
        static_cast<char>(0x12), // Lo byte apparent power
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0xD1),
        static_cast<char>(0xCE)};

    static constexpr std::array<char, 8> REQUEST_POWER_FACTOR = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte power factor
        static_cast<char>(0x1E), // Lo byte power factor
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x11),
        static_cast<char>(0xCD)};

    static constexpr std::array<char, 8> REQUEST_PHASE_ANGLE = {
        static_cast<char>(0x01),
        static_cast<char>(0x04),
        static_cast<char>(0x00), // Hi byte power factor
        static_cast<char>(0x24), // Lo byte power factor
        static_cast<char>(0x00),
        static_cast<char>(0x02),
        static_cast<char>(0x31),
        static_cast<char>(0xC0)};
}
