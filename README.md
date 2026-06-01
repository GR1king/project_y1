# Automated Snow Cannon Control Systems

## About
First year electrical engineering project: An automated control system for snow cannons. The system optimizes artificial snow production by monitoring climate conditions and wind direction using a GD32VF103 (RISC-V) MCU on a Longan Nano board. It features a motorized base for automatic alignment and real-time data visualization on an ST7735 LCD.

---

## Overview
This project presents an automated control system for snow cannons designed to optimize production and mitigate operational risks. By integrating real-time environmental monitoring of wind direction, temperature, and humidity, the system ensures snow is produced only under viable conditions (wet-bulb $<-2^{\circ}C$) and ensures the cannon is correctly positioned to prevent icing and waste.

---

## Firmware
The firmware is developed for the GD32VF103 (RISC-V) microcontroller and utilizes a modular architecture to separate low-level I2C hardware communication from high-level application logic.

* **Environmental Sampling:** The MCU interfaces with an SHT35 sensor via I2C to capture raw 16-bit temperature and humidity values.
* **Data Conversion:** Raw sensor data is converted into physical units ($^{\circ}C$ and %RH) using fixed-point arithmetic to calculate the current wet-bulb temperature.
* **Rotary Feedback Loop:** The system polls the AS5600 12-bit magnetic encoder via I2C (Register 0x0C) to obtain a raw value (0-4095), which is then mapped to a $0^{\circ}-359^{\circ}$ directional value.
* **PWM Motor Control:** Based on wind direction logic, the MCU generates PWM signals via `T1setPWMchx` to a DRV8833 H-bridge to drive the Maxon DC motor.
* **Position Verification:** A hardware feedback mechanism using 8 metal plates and a GPIO-linked probe (`GPIO_PIN_11`) allows the firmware to verify the physical orientation of the cannon against the target wind angle.

---

## Software
* **Environmental Monitoring:** Constantly polls the SHT35 sensor for real-time climate data.
* **Directional Logic:** Interprets data from the AS5600 wind sensor to calculate the necessary rotation for the cannon.
* **User Feedback:** Outputs current status and environmental data to an ST7735 LCD screen.

<p align="center">
  <img src="images/st7735_lcd.png" alt="ST7735 LCD" width="300"/>
</p>

---

## Hardware
<p align="center">
  <img src="images/kicad_schematic.png" alt="Kicad Schematic" width="600"/>
  <br>
  <em>Kicad schematic showing how the Microcontroller is connected to the DC-motor and the LCD.</em>
</p>

<p align="center">
  <img src="images/maxon_motor.png" alt="Maxon DC Motor" width="250"/>
  <br>
  <em>Maxon DC motor used to drive the rotational assembly.</em>
</p>

---

## 3D-Design
<p align="center">
  <img src="images/design_holistic.png" alt="Design prototype Holistic perspective" width="45%" />
  <img src="images/design_side.png" alt="Design prototype Side view" width="45%" />
</p>
<p align="center">
  <em>Design prototype Holistic perspective &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Design prototype: Side view (CAD: SIDOVY)</em>
</p>

<p align="center">
  <img src="images/design_top.png" alt="Design prototype Top view" width="45%" />
  <img src="images/physical_prototype.png" alt="Design Physical prototype" width="45%" />
</p>
<p align="center">
  <em>Design prototype: Top view (CAD: TOPPVY) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Design: Physical prototype</em>
</p>

---

## Specifications (what is capable)
The table below illustrates the operational tolerances of our snow cannon, calculated using our specific formula and SHT35 sensor readings.

| Temp ($^{\circ}C$) | 20% | 30% | 40% | 50% | 60% | 70% | 80% | 90% | 100% |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **-8** | -10.0 | -10.3 | -10.5 | 10.4 | -10.2 | -9.8 | -9.4 | -8.8 | 8.2 |
| **-6** | -8.6 | -8.8 | -8.8 | -8.7 | -8.4 | -7.9 | -7.4 | -6.8 | -6.2 |
| **-4** | -7.3 | -7.3 | -7.2 | -6.9 | -6.6 | -6.1 | -5.5 | -4.9 | -4.2 |
| **-2** | -5.9 | -5.8 | -5.6 | -5.2 | -4.8 | -4.2 | -3.6 | -2.9 | -2.1 |
| **-0** | -4.5 | -4.3 | -3.9 | -3.5 | -3.0 | -2.4 | -1.7 | -0.9 | -0.1 |
| **+2** | -3.2 | -2.8 | -2.3 | -1.8 | -1.2 | -0.5 | 0.2 | 1.0 | 1.9 |
| **+4** | -1.8 | -1.3 | -0.7 | -0.1 | 0.6 | 1.4 | 2.2 | 3.0 | 3.9 |

> **Operational Status Key:** Excellent ($< -8^{\circ}C$), Good (-8 to $-5^{\circ}C$), Marginal (-5 to $-2^{\circ}C$), Unrunnable ($> -2^{\circ}C$)

---

## Acknowledgements
This project was made possible because of Kim Örnberg's experience with snow cannons from a previous job; he recognized the operational problems and conceptualized the automated solution.

---

## Contributors
This was a course project in which every member participated in design, review, and collaborative problem-solving. Primary responsibilities included:

* **George Pliatsikas Malinis (GR1king):** SHT35 sensor integration, technical specifications, and user manual.
* **Heorhii Bielkin:** SHT35 sensor integration and software development.
* **Kim Örnberg:** AS5600 sensor integration and 3D design for the wind-flag.
* **Abel Kibrom:** AS5600 software implementation and hardware integration.
* **Maddiba Thoma Saho:** DC motor 3D design for the complete prototype and hardware assembly.
