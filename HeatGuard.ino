#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Your Wi-Fi network credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// ThingSpeak API Key
String apiKey = "YOUR_THINGSPEAK_API_KEY";

// Telegram Bot Token
const char* botToken = "YOUR_TELEGRAM_BOT_TOKEN";

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DS18B20 Temperature Sensor
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float ds18b20Temp = 0.0;

// DHT22 Temperature and Humidity Sensor
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float dhtTemp = 0.0;
float dhtHumidity = 0.0;

// Timers
unsigned long lastTimeThingSpeak = 0;
unsigned long lastTimeBotRan = 0;
unsigned long lastNotificationTime = 0;
const unsigned long thingSpeakDelay = 60000; // 60 seconds
const unsigned long botRequestDelay = 1000;  // 1 second
const unsigned long notificationDelay = 60000; // 1 minute

// Temperature Threshold
float threshold = 40.0; // Default value
bool isNotificationSent = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C for OLED
  Wire.begin(5, 4); // SDA on GPIO5, SCL on GPIO4

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to initialize OLED"));
    for (;;);
  }

  // Initialize DS18B20
  sensors.begin();
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP); // Enable internal pull-up

  // Initialize DHT22
  dht.begin();

  // Connect to Wi-Fi
  connectWiFi();

  // Setup Telegram bot
  client.setInsecure(); // Use insecure connection for simplicity
  Serial.println("Telegram Bot is ready");
}

void loop() {
  // Read sensor data
  readSensors();

  // Display data on OLED
  displayTemperature();

  // Send data to ThingSpeak every minute
  if (millis() - lastTimeThingSpeak > thingSpeakDelay) {
    sendDataToThingSpeak();
    lastTimeThingSpeak = millis();
  }

  // Check temperature threshold for notifications
  checkTemperatureThreshold();

  // Handle Telegram bot messages
  if (millis() - lastTimeBotRan > botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  delay(500); // Main loop delay
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Connecting to Wi-Fi");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  Serial.println("\nWi-Fi connected");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Wi-Fi connected");
  display.display();
}

void readSensors() {
  // Read DS18B20 temperature
  sensors.requestTemperatures();
  ds18b20Temp = sensors.getTempCByIndex(0);
  Serial.print("DS18B20 Temperature: ");
  Serial.println(ds18b20Temp);

  // Read DHT22 temperature and humidity
  dhtTemp = dht.readTemperature();
  dhtHumidity = dht.readHumidity();

  if (isnan(dhtTemp) || isnan(dhtHumidity)) {
    Serial.println("DHT22 Read Error!");
  } else {
    Serial.print("DHT22 Temperature: ");
    Serial.println(dhtTemp);
    Serial.print("DHT22 Humidity: ");
    Serial.println(dhtHumidity);
  }
}

void displayTemperature() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.setTextSize(3);
  display.setCursor(0, 24);
  display.print(ds18b20Temp);
  display.print(" C");
  display.display();
}

void sendDataToThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = "http://api.thingspeak.com/update?api_key=" + apiKey +
                        "&field1=" + String(ds18b20Temp) +
                        "&field2=" + String(dhtTemp) +
                        "&field3=" + String(dhtHumidity);

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("ThingSpeak Response: " + response);
    } else {
      Serial.print("Error sending data to ThingSpeak. Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    connectWiFi();
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("Processing new messages");
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (from_name == "") from_name = "Guest";

    Serial.println("Message from " + from_name + ": " + text);

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + "!\n";
      welcome += "Available commands:\n";
      welcome += "📊 /status - Current data\n";
      welcome += "⚙️ /set_threshold - Set temperature threshold\n";
      bot.sendMessage(chat_id, welcome, "");
    } else if (text == "/status") {
      String message = "🌡️ DS18B20: " + String(ds18b20Temp) + " °C\n";
      message += "🌡️ DHT22: " + String(dhtTemp) + " °C\n";
      message += "💧 Humidity: " + String(dhtHumidity) + " %";
      bot.sendMessage(chat_id, message, "");
    } else if (text.startsWith("/set_threshold")) {
      float newThreshold = text.substring(15).toFloat();
      if (newThreshold > 0) {
        threshold = newThreshold;
        String message = "⚙️ New temperature threshold: " + String(threshold) + " °C";
        bot.sendMessage(chat_id, message, "");
      } else {
        bot.sendMessage(chat_id, "❌ Invalid value. Please try again.", "");
      }
    } else {
      bot.sendMessage(chat_id, "Unknown command. Use /start to see available commands.", "");
    }
  }
}

void checkTemperatureThreshold() {
  if (ds18b20Temp < threshold && !isNotificationSent && millis() - lastNotificationTime > notificationDelay) {
    String message = "❗ Battery temperature has dropped below the threshold (" + String(threshold) + " °C).\nCurrent temperature: " + String(ds18b20Temp) + " °C";
    bot.sendMessage(bot.messages[0].chat_id, message, "");
    isNotificationSent = true;
    lastNotificationTime = millis();
  } else if (ds18b20Temp >= threshold) {
    isNotificationSent = false; // Reset notification
  }
}
