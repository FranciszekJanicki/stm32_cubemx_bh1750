#ifndef BH1750_HPP
#define BH1750_HPP

#include "bh1750_config.hpp"
#include "i2c_device.hpp"
#include <optional>
#include <utility>

namespace BH1750 {

    struct BH1750 {
    public:
        using I2CDevice = Utility::I2CDevice;

        BH1750() noexcept = default;
        BH1750(I2CDevice&& i2c_device, std::uint8_t const mtreg, Mode const mode) noexcept;

        BH1750(BH1750 const& other) = delete;
        BH1750(BH1750&& other) noexcept = default;

        BH1750& operator=(BH1750 const& other) = delete;
        BH1750& operator=(BH1750&& other) noexcept = default;

        ~BH1750() noexcept;

        [[nodiscard]] std::optional<float> get_light_scaled() const noexcept;

    private:
        void transmit_byte(std::uint8_t const byte) const noexcept;

        template <std::size_t SIZE>
        void transmit_bytes(std::array<std::uint8_t, SIZE> const& bytes) const noexcept;

        std::uint8_t receive_byte() const noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> receive_bytes() const noexcept;

        void initialize(std::uint8_t const mtreg) noexcept;
        void deinitialize() noexcept;

        std::optional<std::uint16_t> get_light_raw() const noexcept;

        bool is_valid_device_id() const noexcept;
        std::uint8_t get_device_id() const noexcept;

        void device_on() const noexcept;
        void device_off() const noexcept;
        void device_reset() const noexcept;
        void device_trigger_conversion() const noexcept;

        void set_power_state(CMD const instruction) const noexcept;
        void set_mtreg(std::uint8_t const mtreg) const noexcept;
        void set_mode(Mode const Mode) const noexcept;

        bool initialized_{false};

        std::uint8_t mtreg_{};
        Mode mode_{};

        I2CDevice i2c_device_{};
    };

    template <std::size_t SIZE>
    inline void BH1750::transmit_bytes(std::array<std::uint8_t, SIZE> const& bytes) const noexcept
    {
        this->i2c_device_.transmit_bytes(bytes);
    }

    template <std::size_t SIZE>
    inline std::array<std::uint8_t, SIZE> BH1750::receive_bytes() const noexcept
    {
        return this->i2c_device_.receive_bytes<SIZE>();
    }

}; // namespace BH1750

#endif // BH1750_HPP