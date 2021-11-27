#include "MPU6050.h"
#include "Wire.h"
#include <WiFi.h>

// Access Point credentials
const char *ssid = "esp32_001";
const char *password = "qwerty123";
int connections = 0;

// Onboard WiFi server
WiFiServer server(80);

MPU6050 accelgyro;

int16_t ax, ay, az; 
int16_t gx, gy, gz;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  connections += 1;
}

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

void setup() {
    // Start access point 
    WiFi.mode(WIFI_AP);                   
    WiFi.softAP(ssid, password);
    WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_AP_STACONNECTED);

    IPAddress ip_address = WiFi.softAPIP();     //IP Address of our accesspoint

    // Start web server
    server.begin();
  
    Serial.print("AP IP address: ");
    Serial.println(ip_address);
  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin(22,21);
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
    WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("Client is here.");
    
    String currentLine = "";   // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {        // if there's bytes to read from the client,
       // char c = client.read();      // read a byte, then
       // Serial.write(c);  

         // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    #endif
    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif

    
        client.print("A");// print it out the serial monitor
        client.print(ax);
        client.print("F");
        client.print(ay);
        client.print("F");
        client.print(az);
        client.print("F");
        client.print(gx);
        client.print("F");
        client.print(gy);
        client.print("F");
        client.print(gz);
      }
    }
    // Close the connection
    client.stop();
  }
}
