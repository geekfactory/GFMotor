/*	GFMotor - Geek Factory Motor Library
	Copyright (C) 2026 Jesus Ruben Santa Anna Zamudio.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
	General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Author website: https://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
#pragma once

#include <Arduino.h>

/**
 * @brief Abstract interface for controlling brushed DC motors.
 *
 * Provides a common API for motor speed, direction inversion, stopping, and
 * braking independently of the underlying motor driver.
 *
 * Motor speed is expressed as a percentage in the range -100 to 100. Positive
 * values represent forward rotation, negative values represent reverse rotation,
 * and zero stops the motor according to the configured zero-speed behavior.
 *
 * Derived classes must implement the hardware-specific operations for driving,
 * coasting, and braking the motor.
 */
class GFMotor
{
public:
	/**
	 * @brief Defines the behavior when motor speed is set to zero.
	 */
	enum ZeroMode : uint8_t
	{
		COAST, ///< Disable motor drive and allow free rotation.
		BRAKE  ///< Apply active electrical braking.
	};

	/**
	 * @brief Virtual destructor for safe polymorphic destruction.
	 */
	virtual ~GFMotor() = default;

	/**
	 * @brief Initializes the motor hardware.
	 *
	 * Derived classes should configure the required hardware and initialize
	 * the motor in a safe coasting state.
	 *
	 * The default implementation performs no operation, allowing motor
	 * controllers with shared hardware initialization.
	 *
	 * @note Must be called before configuring or controlling the motor,
	 * unless initialization is handled by a separate controller.
	 */
	virtual void begin() {}

	/**
	 * @brief Sets the commanded motor speed.
	 *
	 * @param speed Signed speed percentage (-100 to 100).
	 *
	 * Values outside the supported range are constrained. The configured
	 * inversion mode determines the physical direction of rotation.
	 *
	 * Setting speed to zero applies the configured zero-speed mode.
	 */
	void setSpeed(int16_t speed)
	{
		speed = constrain(speed, -100, 100);
		_speed = speed;

		applySpeed();
	}

	/**
	 * @brief Returns the last commanded motor speed.
	 *
	 * @return Signed speed percentage (-100 to 100).
	 *
	 * The returned value is independent of direction inversion and does not
	 * represent measured motor RPM.
	 */
	int16_t getSpeed() const
	{
		return _speed;
	}

	/**
	 * @brief Enables or disables motor direction inversion.
	 *
	 * @param inverted True to invert direction, false for normal direction.
	 *
	 * The current commanded speed is reapplied immediately.
	 */
	void setInverted(bool inverted)
	{
		if (_inverted != inverted)
		{
			_inverted = inverted;
			applySpeed();
		}
	}

	/**
	 * @brief Returns the current direction inversion setting.
	 *
	 * @return True if direction inversion is enabled.
	 */
	bool getInverted() const
	{
		return _inverted;
	}

	/**
	 * @brief Configures the behavior when speed is set to zero.
	 *
	 * @param mode Zero-speed behavior (COAST or BRAKE).
	 *
	 * If the commanded speed is already zero, the new behavior is applied
	 * immediately.
	 */
	void setZeroMode(ZeroMode mode)
	{
		if (_zeroMode != mode)
		{
			_zeroMode = mode;
			if (_speed == 0)
				applySpeed();
		}
	}

	/**
	 * @brief Returns the configured zero-speed behavior.
	 *
	 * @return Current zero-speed mode.
	 */
	ZeroMode getZeroMode() const
	{
		return _zeroMode;
	}

	/**
	 * @brief Stops the motor by disabling its drive outputs.
	 *
	 * The motor is allowed to coast freely. The commanded speed is reset to
	 * zero.
	 *
	 * This operation is independent of the configured zero-speed mode.
	 */
	void stop()
	{
		_speed = 0;
		coastMotor();
	}

	/**
	 * @brief Applies active electrical braking to the motor.
	 *
	 * The commanded speed is reset to zero.
	 *
	 * This operation is independent of the configured zero-speed mode.
	 *
	 * @note Braking effectiveness depends on the motor driver hardware.
	 */
	void brake()
	{
		_speed = 0;
		brakeMotor();
	}

protected:
	GFMotor() = default;

	/**
	 * @brief Drives the motor using the specified PWM and direction.
	 *
	 * @param pwm PWM duty cycle (1 to 255).
	 * @param reverse True for reverse direction, false for forward.
	 *
	 * The PWM value and direction have already been calculated by the
	 * base class, including any configured direction inversion.
	 *
	 * This method is called only for nonzero commanded speeds.
	 * Derived classes must implement the hardware-specific operation.
	 */
	virtual void drive(uint8_t pwm, bool reverse) = 0;

	/**
	 * @brief Disables motor drive to allow coasting.
	 *
	 * Implemented by derived classes.
	 */
	virtual void coastMotor() = 0;

	/**
	 * @brief Applies active electrical braking.
	 *
	 * Implemented by derived classes.
	 */
	virtual void brakeMotor() = 0;

private:
	void applySpeed()
	{
		if (_speed == 0)
		{
			if (_zeroMode == BRAKE)
				brakeMotor();
			else
				coastMotor();

			return;
		}

		int16_t speed = _inverted ? -_speed : _speed;
		bool reverse = speed < 0;

		if (reverse)
			speed = -speed;

		uint8_t pwm = (speed * 255L) / 100;

		drive(pwm, reverse);
	}

	int16_t _speed = 0;
	bool _inverted = false;
	ZeroMode _zeroMode = COAST;
};
