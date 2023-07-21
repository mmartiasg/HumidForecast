// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <WiFi.h>

const char* ssid     = "MIWIFI_PmA3";
const char* password = "dHpTMdbR";

std::string response_json = "C++";

WiFiServer server(80);

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

//Green led to indicate the setup was susscessfull
#define GREENLIGHT 15
#define BLUELIGHT 5
#define REDLIGHT 18

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  pinMode(GREENLIGHT, OUTPUT);
  pinMode(BLUELIGHT, OUTPUT);
  pinMode(REDLIGHT, OUTPUT);

  digitalWrite(REDLIGHT, HIGH);
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
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  
  //Sussccesfull setup
  digitalWrite(GREENLIGHT, HIGH);
  digitalWrite(REDLIGHT, LOW);
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // this will be on demand
            // delay(2000);
            digitalWrite(REDLIGHT, LOW);
            digitalWrite(GREENLIGHT, HIGH);

            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            digitalWrite(BLUELIGHT, HIGH);
            float h = dht.readHumidity();
            digitalWrite(BLUELIGHT, LOW);
            // Read temperature as Celsius (the default)
            digitalWrite(BLUELIGHT, HIGH);
            float t = dht.readTemperature();
            digitalWrite(BLUELIGHT, LOW);
            // Read temperature as Fahrenheit (isFahrenheit = true)
            digitalWrite(BLUELIGHT, HIGH);
            float f = dht.readTemperature(true);
            digitalWrite(BLUELIGHT, LOW);

            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
            // Serial.println(F("Failed to read from DHT sensor!"));
            client.print("Failed to read from DHT sensor!");
            client.println();
            client.stop();

            digitalWrite(REDLIGHT, HIGH);
            digitalWrite(GREENLIGHT, LOW);
            return;
            }

            // Compute heat index in Fahrenheit (the default)
            digitalWrite(BLUELIGHT, HIGH);
            float hif = dht.computeHeatIndex(f, h);
            digitalWrite(BLUELIGHT, LOW);
            // Compute heat index in Celsius (isFahreheit = false)
            digitalWrite(BLUELIGHT, HIGH);
            float hic = dht.computeHeatIndex(t, h, false);
            digitalWrite(BLUELIGHT, LOW);

            // Serial.print(F("Humidity: "));
            // Serial.print(h);
            // Serial.print(F("%  Temperature: "));
            // Serial.print(t);
            // Serial.print(F("°C "));
            // Serial.print(f);
            // Serial.print(F("°F  Heat index: "));
            // Serial.print(hic);
            // Serial.print(F("°C "));
            // Serial.print(hif);
            // Serial.println(F("°F"));

            // the content of the HTTP response follows the header:
            digitalWrite(BLUELIGHT, HIGH);

            // client.print("Humidity: ");
            // client.print(h);
            // client.print("%");
            // client.print("<br>");
            // client.print("Temperature in Celsius: ");
            // client.print(t);
            // client.print("<br>");
            // client.print("Temperature in Fahrenheit: ");
            // client.print(f);
            // client.print("<br>");
            // client.print("Celsius Heat index: ");
            // client.print(hic);
            // client.print("<br>");
            // client.print("Fahrenheit Heat index: ");
            // client.print(hif);

            auto response_json = "{\"Humidity\" : " + String(h) +", "+ "\"C\" : "+String(t)+", "+"\"F\" : "+String(f)+", "+"\"C_H_index\" : "+String(hic)+", "+"\"F_H_index\" : "+String(hif)+"}";
            client.print(response_json);

            digitalWrite(BLUELIGHT, LOW);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    // Serial.println("Client Disconnected.");
  }
}