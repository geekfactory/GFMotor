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
#include "GFMotor.h"

/**
 * @brief DC motor driver using a three-pin H-bridge interface.
 *
 * Controls a brushed DC motor using two direction inputs and
 * one PWM input.
 *
 * Designed for H-bridges compatible with the TB6612FNG
 * IN1/IN2/PWM control scheme.
 *
 * The motor driver must be enabled externally before use.
 */
class GFHBridgeMotor : public GFMotor
{
public:
	/**
	 * @brief Constructs an H-bridge motor controller.
	 *
	 * @param pin1 First direction control pin.
	 * @param pin2 Second direction control pin.
	 * @param pinPWM PWM-capable speed control pin.
	 */
	GFHBridgeMotor(uint8_t pin1, uint8_t pin2, uint8_t pinPWM);

	/**
	 * @brief Initializes the motor control pins.
	 *
	 * Configures all pins as outputs and initializes the motor in a
	 * coasting state.
	 *
	 * @note Must be called before controlling the motor.
	 */
	void begin() override;

protected:
	/** @copydoc GFMotor::drive */
	void drive(uint8_t pwm, bool reverse) override;

	/** @copydoc GFMotor::coastMotor */
	void coastMotor() override;

	/** @copydoc GFMotor::brakeMotor */
	void brakeMotor() override;

private:
	uint8_t _pin1;
	uint8_t _pin2;
	uint8_t _pinPWM;
};
