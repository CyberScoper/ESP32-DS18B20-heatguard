# HeatGuard 🔥🏠 

[![CodeFactor](https://www.codefactor.io/repository/github/cyberscopetoday/heatguard/badge)](https://www.codefactor.io/repository/github/cyberscopetoday/heatguard)

<img src="https://github.com/user-attachments/assets/a1c779a0-c684-49df-ba12-f5cc9c0bc62d" alt="HeatGuard Demo" width="300"/>

## Table of Contents
- [🌟 Overview](#-overview)
- [✨ Features](#-features)
- [🛠️ Hardware Requirements](#️-hardware-requirements)
- [💻 Software Requirements](#-software-requirements)
- [🚀 Setup Instructions](#-setup-instructions)
  - [1. 📥 Clone the Repository](#1--clone-the-repository)
  - [2. 📚 Install Arduino Libraries](#2--install-arduino-libraries)
  - [3. ⚙️ Configure the Code](#3--configure-the-code)
  - [4. 📤 Upload the Code to ESP32](#4--upload-the-code-to-esp32)
- [📖 Usage](#-usage)
  - [🤖 Telegram Bot Commands](#-telegram-bot-commands)
- [🔧 Troubleshooting](#-troubleshooting)
- [📄 License](#-license)
- [📢 Project Demo](#-project-demo)
- [🤝 Contributing](#-contributing)
- [📧 Contact](#-contact)

## 🌟 Overview

**HeatGuard** is a **Smart Heating Monitoring System** designed to keep a close eye on your indoor heating environment. Utilizing an ESP32 microcontroller, it integrates multiple sensors to monitor temperature and humidity, displays real-time data on an OLED screen, sends data to ThingSpeak for cloud-based monitoring, and provides instant notifications via a Telegram bot. This system is perfect for applications such as home heating management, environmental monitoring, and IoT-based alert systems to ensure a comfortable and safe living environment.

## ✨ Features

- **📶 Wi-Fi Connectivity**: Seamlessly connects to your local Wi-Fi network for internet-based functionalities.
- **🌡️ Temperature & 💧 Humidity Monitoring**: Utilizes DS18B20 and DHT22 sensors to accurately measure temperature and humidity.
- **🖥️ OLED Display**: Displays real-time temperature and humidity data on a 128x64 OLED screen.
- **☁️ ThingSpeak Integration**: Sends sensor data to ThingSpeak for cloud-based data logging and visualization.
- **📲 Telegram Notifications**: Sends instant alerts via Telegram when temperature or humidity crosses predefined thresholds.
- **🛠️ User Interaction**: Control and monitor the system using intuitive Telegram bot commands.

## 🛠️ Hardware Requirements

- **🔌 ESP32 Development Board**
- **🌡️ DS18B20 Temperature Sensor**
- **🌡️💧 DHT22 Temperature and Humidity Sensor**
- **🖥️ 128x64 OLED Display (e.g., SSD1306)**
- **🔧 Resistors**:
  - 4.7kΩ resistor for DS18B20 data line
- **🪛 Breadboard and Jumper Wires**
- **🔋 Power Supply**: Suitable for ESP32 (e.g., USB power)

## 💻 Software Requirements

- **💾 Arduino IDE** (version 1.8.0 or later)
- **📦 Arduino Libraries**:
  - `Wire.h`
  - `WiFi.h`
  - `WiFiClientSecure.h`
  - `UniversalTelegramBot.h`
  - `HTTPClient.h`
  - `OneWire.h`
  - `DallasTemperature.h`
  - `Adafruit_GFX.h`
  - `Adafruit_SSD1306.h`
  - `DHT.h`

## 🚀 Setup Instructions

### 1. 📥 Clone the Repository

Clone this repository to your local machine using Git:

```bash
git clone https://github.com/CyberScopeToday/heatguard.git
```

### 2. 📚 Install Arduino Libraries

Open the Arduino IDE and install the required libraries:

1. Go to **Sketch** > **Include Library** > **Manage Libraries**.
2. Search for and install the following libraries:
   - **Universal Telegram Bot**
   - **DallasTemperature**
   - **OneWire**
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**
   - **DHT sensor library**

### 3. ⚙️ Configure the Code

Open the `HeatGuard.ino` file in the Arduino IDE and update the following parameters:

- **🔒 Wi-Fi Credentials**:
  ```cpp
  const char* ssid = "Your_WiFi_SSID";
  const char* password = "Your_WiFi_Password";
  ```

- **🔑 ThingSpeak API Key**:
  ```cpp
  String apiKey = "YOUR_THINGSPEAK_API_KEY";
  ```

- **🤖 Telegram Bot Token**:
  ```cpp
  const char* botToken = "YOUR_TELEGRAM_BOT_TOKEN";
  ```

- **📍 GPIO Pins** (if different from defaults):
  ```cpp
  #define ONE_WIRE_BUS 12 // DS18B20 data pin
  #define DHTPIN 15        // DHT22 data pin
  #define SDA_PIN 5        // OLED SDA
  #define SCL_PIN 4        // OLED SCL
  ```

- **🔧 Temperature Thresholds** (optional):
  ```cpp
  float temperatureThreshold = 25.0; // Example: 25°C
  float humidityThreshold = 60.0;    // Example: 60%
  ```

### 4. 📤 Upload the Code to ESP32

1. Connect your ESP32 board to your computer via USB.
2. Select the appropriate board and port in the Arduino IDE:
   - **Tools** > **Board** > **ESP32 Dev Module**
   - **Tools** > **Port** > *Select your ESP32 port*
3. Click the **Upload** button to compile and upload the code to the ESP32.

<img src="https://github.com/user-attachments/assets/8229c2ab-dfa7-4de9-bbb8-161947a0fd99" alt="ardideexample" width="500"/>

## 📖 Usage

Once the system is powered and connected to Wi-Fi, it will start monitoring the sensors, display data on the OLED, send data to ThingSpeak, and await Telegram commands.

### 🤖 Telegram Bot Commands

Interact with **HeatGuard** using the following Telegram commands:

- **/start** 🎉
  - **Description**: Initializes the bot and displays available commands.
  - **Response**: Welcome message with command list.

- **/status** 📊
  - **Description**: Retrieves the current sensor readings.
  - **Response**:
    ```
    🌡️ DS18B20: XX.X °C
    🌡️ DHT22: XX.X °C
    💧 Humidity: XX.X %
    ```

- **/set_temp_threshold [value]** 🔥
  - **Description**: Sets a new temperature threshold for notifications.
  - **Usage**: `/set_temp_threshold 28.5`
  - **Response**:
    - **Success**: `🔥 New temperature threshold set to 28.5 °C`
    - **Failure**: `❌ Invalid value. Please try again.`

- **/set_humidity_threshold [value]** 💦
  - **Description**: Sets a new humidity threshold for notifications.
  - **Usage**: `/set_humidity_threshold 55`
  - **Response**:
    - **Success**: `💦 New humidity threshold set to 55%`
    - **Failure**: `❌ Invalid value. Please try again.`

- **/help** 🆘
  - **Description**: Displays the list of available commands.
  - **Response**: List of commands with descriptions.

## 🔧 Troubleshooting

- **📶 Wi-Fi Connection Issues**:
  - Ensure that the SSID and password in the code are correct.
  - Check the distance between the ESP32 and the Wi-Fi router.

- **🌡️ Sensor Read Errors**:
  - Verify the wiring connections for DS18B20 and DHT22.
  - Ensure that the sensors are properly powered.

- **🖥️ OLED Display Not Working**:
  - Check the I2C connections (SDA and SCL pins).
  - Confirm that the OLED address (`0x3C`) matches your display.

- **🤖 Telegram Bot Not Responding**:
  - Make sure the bot token is correct.
  - Ensure that the ESP32 is connected to the internet.
  - Check if the bot is started in Telegram.

- **☁️ ThingSpeak Data Not Updating**:
  - Verify the ThingSpeak API key.
  - Ensure that the ESP32 has internet access.
  - Check ThingSpeak channel settings.

- **🔔 Notifications Not Sending**:
  - Confirm that the temperature and humidity thresholds are set correctly.
  - Ensure that the ESP32 is connected to the internet.
  - Check Telegram bot permissions and chat status.

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

## 📢 Project Demo

<img src="https://github.com/user-attachments/assets/f2a54785-f9db-4055-99bc-4c4dbb6ee992" alt="HeatGuard Demo" width="500"/>

## 🤝 Contributing

Contributions are welcome! Please fork the repository and create a pull request with your enhancements. For major changes, please open an issue first to discuss what you would like to change.

## 📧 Contact

For any questions or suggestions, feel free to [open an issue](https://github.com/CyberScopeToday/heatguard/issues) or contact me directly.

---

**HeatGuard** ensures your heating environment is always monitored and maintained, providing peace of mind through real-time data and instant alerts. Stay warm and stay informed! 🔥🏠✨

---
