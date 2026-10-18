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

#include <GFHBridgeMotor.h>

// Create motor instances (IN1, IN2, PWM)
GFHBridgeMotor leftMotor(4, 7, 5);
GFHBridgeMotor rightMotor(8, 9, 6);

void setup()
{
	// Initialize motor hardware
	leftMotor.begin();
	rightMotor.begin();

	// Invert one motor to compensate for opposite mounting
	rightMotor.setInverted(true);
}

void loop()
{
	// Move forward
	leftMotor.setSpeed(75);
	rightMotor.setSpeed(75);
	delay(2000);

	// Stop both motors
	leftMotor.stop();
	rightMotor.stop();
	delay(1000);

	// Move backward
	leftMotor.setSpeed(-75);
	rightMotor.setSpeed(-75);
	delay(2000);

	// Stop both motors
	leftMotor.stop();
	rightMotor.stop();
	delay(1000);

	// Turn left
	leftMotor.setSpeed(-50);
	rightMotor.setSpeed(50);
	delay(1000);

	// Turn right
	leftMotor.setSpeed(50);
	rightMotor.setSpeed(-50);
	delay(1000);

	// Brake both motors
	leftMotor.brake();
	rightMotor.brake();
	delay(2000);
}
