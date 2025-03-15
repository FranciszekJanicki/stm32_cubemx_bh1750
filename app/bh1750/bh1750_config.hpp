#ifndef BH1750_CONFIG_HPP
#define BH1750_CONFIG_HPP

#include <cstdint>

namespace BH1750 {

    enum struct DevAddress : std::uint8_t {
        AD0_LOW = 0x55,
        AD0_HIG = 0x23,
    };

    enum struct CMD : std::uint8_t {
        POWER_DOWN = 0x00,
        POWER_ON = 0x01,
        RESET = 0x07,
    };

    enum struct Mode : std::uint8_t {
        CONTINUOUS_HIGH_RES_MODE = 0x10,
        CONTINUOUS_HIGH_RES_MODE_2 = 0x11,
        CONTINUOUS_LOW_RES_MODE = 0x13,
        ONETIME_HIGH_RES_MODE = 0x20,
        ONETIME_HIGH_RES_MODE_2 = 0x21,
        ONETIME_LOW_RES_MODE = 0x23,
    };

    auto constexpr MTREG_MIN = 0x1F;
    auto constexpr MTREG_DEFAULT = 0x45;
    auto constexpr MTREG_MAX = 0xFE;
    auto constexpr MEASUREMENT_ACCURACY = 1.2F;

    inline float mode_to_resolution(Mode const mode) noexcept
    {
        if (mode == Mode::CONTINUOUS_HIGH_RES_MODE || mode == Mode::ONETIME_HIGH_RES_MODE) {
            return 1.0F;
        } else if (mode == Mode::CONTINUOUS_HIGH_RES_MODE_2 || mode == Mode::ONETIME_HIGH_RES_MODE_2) {
            return 0.5F;
        } else if (mode == Mode::CONTINUOUS_LOW_RES_MODE || mode == Mode::ONETIME_LOW_RES_MODE) {
            return 4.0F;
        }
        return 0.0F;
    }

    inline float raw_to_scaled(std::uint16_t const raw, Mode const mode, std::uint8_t const mtreg) noexcept
    {
        return raw * (1.0F / MEASUREMENT_ACCURACY) * (MTREG_DEFAULT / mtreg) * mode_to_resolution(mode);
    }

}; // namespace BH1750

#endif // BH1750_CONFIG_HPP