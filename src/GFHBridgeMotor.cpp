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

#include "GFHBridgeMotor.h"

GFHBridgeMotor::GFHBridgeMotor(uint8_t pin1, uint8_t pin2, uint8_t pinPWM) : _pin1(pin1), _pin2(pin2), _pinPWM(pinPWM)
{
}

void GFHBridgeMotor::begin()
{
	// Preload safe output levels
	digitalWrite(_pin1, LOW);
	digitalWrite(_pin2, LOW);
	digitalWrite(_pinPWM, LOW);

	// Configure motor control pins
	pinMode(_pin1, OUTPUT);
	pinMode(_pin2, OUTPUT);
	pinMode(_pinPWM, OUTPUT);

	// Initialize motor in coasting state
	coastMotor();
}

void GFHBridgeMotor::drive(uint8_t pwm, bool reverse)
{
	// Disable motor drive before changing direction
	analogWrite(_pinPWM, 0);

	// Configure motor direction
	digitalWrite(_pin1, reverse ? HIGH : LOW);
	digitalWrite(_pin2, reverse ? LOW : HIGH);

	// Apply requested PWM duty cycle
	analogWrite(_pinPWM, pwm);
}

void GFHBridgeMotor::coastMotor()
{
	// Disable motor drive
	analogWrite(_pinPWM, 0);
	// Set both direction inputs low
	digitalWrite(_pin1, LOW);
	digitalWrite(_pin2, LOW);
}

void GFHBridgeMotor::brakeMotor()
{
	// Disable motor drive during transition
	analogWrite(_pinPWM, 0);
	// Configure short-brake state
	digitalWrite(_pin1, HIGH);
	digitalWrite(_pin2, HIGH);
	// Enable H-bridge outputs for braking
	analogWrite(_pinPWM, 255);
}
