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

// Create motor instance (IN1, IN2, PWM)
// Connections:
//  * IN1 -> Arduino pin 4
//  * IN2 -> Arduino pin 5
//  * PWM -> Arduino pin 6
// Note: The motor driver must be enabled externally.
GFHBridgeMotor motor(4, 5, 6);

// Function to control any motor implementing the GFMotor interface
void runMotor(GFMotor &motor)
{
	// Run forward at 50% speed
	motor.setSpeed(50);
	delay(2000);

	// Brake motor
	motor.setSpeed(0);
	delay(1000);

	// Run reverse at 50% speed
	motor.setSpeed(-50);
	delay(2000);

	// Coast to a stop
	motor.stop();
}

void setup()
{
	// Initialize motor hardware
	motor.begin();

	// Configure motor behavior
	motor.setInverted(false);
	motor.setZeroMode(GFMotor::BRAKE);
}

void loop()
{
	// Pass the concrete motor as a generic reference
	runMotor(motor);

	delay(2000);
}
