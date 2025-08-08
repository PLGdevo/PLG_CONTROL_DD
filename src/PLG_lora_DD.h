
#include <Arduino.h>
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <hardwareSerial.h>

// define the pins used by the transceiver module
#define ss 05
#define rst 04
#define dio0 14
#define led_connected 26
#define led_slave 33
#define led_master 25
#define TX 17
#define RX 16

String ID_control = "0";
String ID_CB = "1";
String ID_DD = "2";
String ID_control_DD = "4";
String ID_master = "3";

unsigned long lastTime = 0;

float ph_nuoc = 0.0; // V20
float ec_nuoc = 0.0; // V19

bool start_sensor = false; // Flag to indicate if sensor data should be processed
bool Auto_cool = false;
int couter = 0, mode = 0;
float temp_setpoint = 40.0;
// String receivedData = "";
String cmd = "";
void sen_lora_data_4()
{
    // Example of sending sensor data
    LoRa.beginPacket(); // Start a new packet
    LoRa.print(messages4);
    LoRa.endPacket();               // Finish the packet and send it
    digitalWrite(led_master, HIGH); // Turn off LED for slave status
    delay(20);                      // Delay to ensure the message is sent
    digitalWrite(led_master, LOW);  // Turn off LED for slave status
}
void setup()
{

    Serial.begin(115200);
    DEBUG_PRINTLN("LoRa Sender");

    // setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    // 433E6 for Asia
    while (!LoRa.begin(433E6))
    {
        Serial.print(".");
        delay(500);
    }
    DEBUG_PRINTLN("LoRa Initialized");
    pinMode(led_slave, OUTPUT);
    pinMode(led_master, OUTPUT);
    pinMode(led_connected, OUTPUT);
    digitalWrite(led_connected, LOW);
    digitalWrite(led_slave, HIGH);
    digitalWrite(led_master, LOW);
    DEBUG_PRINTLN("PLG RUNNING DD");

    Serial2.begin(115200, SERIAL_8N1, RX, TX);
}
void thucthilenh()
{

}

void loop()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        receivedData = "";
        while (LoRa.available())
        {
            receivedData += (char)LoRa.read();
        }
        PLG_check_message(); // Check the received data
        thucthilenh();       // Execute the command
        digitalWrite(led_connected, HIGH);
        delay(20);
        digitalWrite(led_connected, LOW);
    }

    if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh();                                // Execute the command
        DEBUG_PRINTLN(receivedData);
    }

    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 2345)
    {
        lastTime = currentTime;
    }
}
