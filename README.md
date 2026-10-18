# GFMotor - Geek Factory Motor Library #

This library is designed as a minimal abstraction layer between **DC motor driver circuits** and the user's high-level code. **This library is designed to be used with the Arduino framework using the Arduino IDE or PlatformIO**.

DC motors are commonly used in robotics, automation, and educational projects. They can be controlled using inexpensive H-bridge motor drivers that allow the microcontroller to regulate motor speed and direction.

We designed the library to provide a simple and consistent interface for controlling brushed DC motors, independently of the underlying motor driver hardware.

The library currently includes the following classes:

* **GFMotor:** An abstract base class that provides a common interface for motor speed, direction inversion, stopping, and braking.
* **GFHBridgeMotor:** A motor controller implementation for H-bridges using two direction inputs and one PWM input, such as the popular TB6612FNG.

The common interface also allows other motor driver libraries to inherit from `GFMotor`, making it possible to use different motor controllers without modifying the application's high-level code.

## Project Objectives ##

Our library should fulfill the following goals:

* Code should be very simple and have no dependencies other than the functions provided by the Arduino framework
* Should not directly rely on timers or other architecture dependent features
* We should be able to use this code in courses to demonstrate how to create an Arduino library and use C++ inheritance
* Be able to use the code on other platforms besides the AVR Arduino boards (i.e. ESP32, ATSAMD21, Raspberry Pi Pico)
* Provide a common interface that can be reused by other motor driver libraries
* Support signed motor speed values, direction inversion, and configurable stopping behavior
* Provide simple, clean, usable code

## Basic library usage ##

In order to use the library, we need to include the appropriate header file and instantiate an object from the `GFHBridgeMotor` class.

The constructor accepts the pin numbers used to control the H-bridge. Two pins control the motor direction, while a third PWM-capable pin controls the motor speed.

Multiple instances can be created simultaneously to control multiple motors.

```cpp
// include the library header file
#include <GFHBridgeMotor.h>

// instantiate a motor using pins 4 and 5 for direction
// and pin 6 for PWM speed control
GFHBridgeMotor motor(4, 5, 6);
```

We also need to call the `begin()` method in order to initialize the pins used for motor control. This should be done in the `setup()` function before configuring or operating the motor.

```cpp
void setup()
{
  // initialize the motor control pins
  motor.begin();
}
```

To control the motor speed and direction, we call the `setSpeed()` method:

```cpp
void loop()
{
  // run motor forward at 50% speed
  motor.setSpeed(50);
  delay(2000);

  // run motor in reverse at 75% speed
  motor.setSpeed(-75);
  delay(2000);

  // stop the motor
  motor.stop();
  delay(1000);
}
```

Motor speed is expressed as a signed percentage between **-100 and 100**:

* `100` represents full speed in the forward direction
* `50` represents 50% speed in the forward direction
* `0` stops the motor according to the configured zero-speed behavior
* `-50` represents 50% speed in the reverse direction
* `-100` represents full speed in the reverse direction

Values outside this range are automatically constrained.

The speed percentage corresponds to the PWM duty cycle applied to the motor driver. It does not represent the actual motor RPM.

## Motor direction inversion ##

The library allows motor direction to be inverted without modifying the application logic.

This is particularly useful in mobile robots where motors are often mounted in opposite orientations.

```cpp
// invert the motor direction
motor.setInverted(true);

// run the motor in the inverted forward direction
motor.setSpeed(75);
```

The `isInverted()` method returns the current inversion setting.

Direction inversion does not affect the value returned by `getSpeed()`, which always represents the logical commanded speed.

## Motor stopping and braking ##

The library supports two different methods for stopping a motor:

* **Coasting:** The motor driver outputs are disabled, allowing the motor to rotate freely until it stops.
* **Braking:** The motor driver applies active electrical braking to stop the motor more quickly.

These operations are available through the `stop()` and `brake()` methods:

```cpp
// allow the motor to coast
motor.stop();

// apply active electrical braking
motor.brake();
```

Both methods reset the commanded speed to zero.

The library also allows configuring the behavior when `setSpeed(0)` is called:

```cpp
// configure active braking when speed is zero
motor.setZeroMode(GFMotor::BRAKE);

// this will apply active braking
motor.setSpeed(0);
```

Alternatively, the motor can be configured to coast:

```cpp
// configure coasting when speed is zero
motor.setZeroMode(GFMotor::COAST);
```

The default zero-speed behavior is `COAST`.

The `getZeroMode()` method returns the currently configured behavior.

## Reading motor speed ##

The last commanded motor speed can be retrieved using the `getSpeed()` method:

```cpp
motor.setSpeed(75);

// read the current commanded speed
int16_t speed = motor.getSpeed();
```

This method returns the signed speed percentage requested by the application, independently of the direction inversion setting.

It does not measure the actual motor speed. Applications requiring closed-loop speed regulation should use additional hardware, such as rotary encoders.

## Creating custom motor drivers ##

The `GFMotor` class provides an abstract interface that can be extended to support additional motor controllers.

Custom drivers can inherit from `GFMotor` and implement the hardware-specific methods:

* `drive()` — Apply PWM and motor direction
* `coastMotor()` — Disable motor drive
* `brakeMotor()` — Apply active electrical braking

The base class handles speed normalization, direction inversion, zero-speed behavior, and commanded-speed state.

This allows different motor controllers to be used through the same interface:

```cpp
void runMotor(GFMotor& motor)
{
  motor.setSpeed(50);
}
```

The function above can accept any motor object derived from `GFMotor`, independently of the hardware used to control it.

## Supported motor drivers ##

The library currently provides an implementation for H-bridge motor drivers using two direction inputs and one PWM input.

The `GFHBridgeMotor` class is designed around the control scheme used by the **TB6612FNG**.

Other H-bridge drivers may be compatible if they use the same direction, PWM, coasting, and braking logic. Always verify the motor driver's truth table before using it with this library.

For the TB6612FNG, the STBY input must be held HIGH to enable motor operation.

## Supported devices ##

The library uses standard Arduino functions such as `pinMode()`, `digitalWrite()`, and `analogWrite()`, and is intended to support multiple Arduino-compatible architectures.

Target platforms include:

* Arduino UNO / Nano (ATmega328P)
* Arduino Mega 2560
* Arduino M0 (ATSAMD21G18)
* Raspberry Pi Pico / Pico 2
* ESP32 based devices

Compatibility and PWM capabilities depend on the selected board and Arduino core. Hardware validation should be performed for each supported platform.

## License ##

This library is distributed under the GNU Lesser General Public License, version 3 or (at your option) any later version.

See the LICENSE file for additional information.

## Contact me ##

* Feel free to write for any inquiry: ruben at geekfactory.mx
* Check our website: https://www.geekfactory.mx