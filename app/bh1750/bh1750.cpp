#include "bh1750.hpp"
#include "bh1750_config.hpp"
#include <algorithm>
#include <optional>
#include <utility>

namespace BH1750 {

    BH1750::BH1750(I2CDevice&& i2c_device, std::uint8_t const mtreg, Mode const mode) noexcept :
        mtreg_{mtreg}, mode_{mode}, i2c_device_{std::forward<I2CDevice>(i2c_device)}
    {
        this->initialize(mtreg);
    }

    BH1750::~BH1750() noexcept
    {
        this->deinitialize();
    }

    std::optional<float> BH1750::get_light_scaled() const noexcept
    {
        return this->get_light_raw().transform(
            [this](std::uint16_t const raw) { return raw_to_scaled(raw, this->mode_, this->mtreg_); });
    }

    void BH1750::transmit_byte(std::uint8_t const byte) const noexcept
    {
        this->i2c_device_.transmit_byte(byte);
    }

    std::uint8_t BH1750::receive_byte() const noexcept
    {
        return this->i2c_device_.receive_byte();
    }

    void BH1750::initialize(std::uint8_t const mtreg) noexcept
    {
        if (this->is_valid_device_id()) {
            this->device_reset();
            this->device_on();
            this->set_mtreg(mtreg);
            this->initialized_ = true;
        }
    }

    void BH1750::deinitialize() noexcept
    {
        if (this->is_valid_device_id()) {
            this->device_off();
            this->initialized_ = false;
        }
    }

    std::optional<std::uint16_t> BH1750::get_light_raw() const noexcept
    {
        if (!this->initialized_) {
            std::optional<std::uint16_t>{std::nullopt};
        }
        this->device_trigger_conversion();
        return std::optional<std::uint16_t>{std::bit_cast<std::uint16_t>(this->receive_bytes<2UL>())};
    }

    bool BH1750::is_valid_device_id() const noexcept
    {
        return this->get_device_id() == this->i2c_device_.dev_address();
    }

    std::uint8_t BH1750::get_device_id() const noexcept
    {
        return this->receive_byte();
    }

    void BH1750::device_on() const noexcept
    {
        this->set_power_state(CMD::POWER_ON);
    }

    void BH1750::device_off() const noexcept
    {
        this->set_power_state(CMD::POWER_DOWN);
    }

    void BH1750::device_reset() const noexcept
    {
        this->set_power_state(CMD::RESET);
    }

    void BH1750::set_power_state(CMD const instruction) const noexcept
    {
        this->transmit_byte(0x01 & std::to_underlying(instruction));
    }

    void BH1750::set_mode(Mode const mode) const noexcept
    {
        this->transmit_byte(std::to_underlying(mode));
    }

    void BH1750::set_mtreg(std::uint8_t const mtreg) const noexcept
    {
        auto const mtreg_value = std::clamp(static_cast<int>(mtreg), MTREG_MIN, MTREG_MAX);
        this->transmit_byte(0x40 | (mtreg_value >> 5));
        this->transmit_byte(0x60 | (mtreg_value & 0x1F));
    }

    void BH1750::device_trigger_conversion() const noexcept
    {
        this->set_mode(this->mode_);
    }

}; // namespace BH1750