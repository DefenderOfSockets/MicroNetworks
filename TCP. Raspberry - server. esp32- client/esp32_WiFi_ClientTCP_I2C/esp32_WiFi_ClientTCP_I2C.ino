#include <WiFi.h>
#include "MPU6050.h"
#include "Wire.h"

const char* ssid     = "raspberry_001";
const char* password = "raspberry";

const char* host = "192.168.2.1";
const int port = 48984;

MPU6050 accelgyro;

int16_t ax, ay, az; 
int16_t gx, gy, gz;

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

bool True = 1;

void setup()
{
    // Инициализация I2C подключения между датчиком MPU6500 и микроконтроллером esp32, используя библиотеку "Wire.h"
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin(22,21);
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    delay(5000);

    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClient client;   // Listen for incoming clients
  if (client.connect(host, port)) {                       
    Serial.println("Connect to server - successful");
    
    String currentLine = "";   // make a String to hold incoming data from the client
    while (True) {  // loop while the client's connected
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

   /* uint8_t buf[12];
    
    buf[0]=ax & 0xff;
    buf[1]=(ax >> 8);
    
    buf[2]=ay & 0xff;
    buf[3]=(ay >> 8);
    
    buf[4]=az & 0xff;
    buf[5]=(az >> 8);
    
    buf[6]=gx & 0xff;
    buf[7]=(gx >> 8);
    
    buf[8]=gy & 0xff;
    buf[9]=(gy >> 8);
    
    buf[10]=gz & 0xff;
    buf[11]=(gz >> 8);*/

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

        //client.write(buf,12);
      }
    }
    // Close the connection
    client.stop();
  }
}
