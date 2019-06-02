/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include "dht.h"
#define dht_apin A0


dht DHT;
 
int redLed = 8;
int blueLed = 9;

String readString;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {

 pinMode(redLed,OUTPUT);
  pinMode(blueLed,OUTPUT);
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
   DHT.read11(dht_apin);
   
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();



        if(readString.length() <100)
            {
              readString +=c;
            }
 
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin

              client.print("Hi Arduino   ");
               client.println("<br>");
              client.print(" Humidity : ");
              client.print( DHT.humidity);
              client.print("% ");
             client.println("<br>");
              client.print(" temperature : ");
              client.print( DHT.temperature);
              client.println("C ");

client.println("<br>");
client.println("<br>");
client.println("<br>");
client.println("<a href=\"/?button1on\"\"\>Trun on LED 1</a>");
client.println("<a href=\"/?button1off\"\"\>Trun off LED 1</a><br>");

client.println("<br>");
client.println("<br>");
client.println("<br>");
client.println("<a href=\"/?button2on\"\"\>Trun on LED 2</a>");
client.println("<a href=\"/?button2off\"\"\>Trun off LED 2</a><br>");
  
          client.println("</html>");
          break;
        }

        if(readString.indexOf("?button1on") >0){
          digitalWrite(redLed , HIGH);
        }
         if(readString.indexOf("?button1off") >0){
          digitalWrite(redLed , LOW);
        }


       if(readString.indexOf("?button2on") >0){
          digitalWrite(blueLed , HIGH);
        }
         if(readString.indexOf("?button2off") >0){
          digitalWrite(blueLed , LOW);
        }
        
           
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
     readString = "";
  }
}
