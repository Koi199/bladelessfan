/*
 * Connects the ESP8266 NodeMCU board to wifi and prints the IP address
 * 
 * by:
 * ROBOSANS
 * based on ThingSpeak Library example
 * 
 * https://www.robosans.com/
 */

#include "ESP8266WiFi.h"
#include <Servo.h>

// WiFi parameters to be configured
const char* ssid = "Looking for plans"; // Write here your router's username
const char* password = "4973190901"; // Write here your router's passward

Servo servo; //create servo object

//int ledPin = 2; // GPIO2 of ESP8266
WiFiServer server(80);//Service Port

void setup(void)
{ 
  Serial.begin(9600);
  
  delay(10);

  pinMode(2, OUTPUT);
  //digitalWrite(ledPin, LOW);

  //initialise servo
  servo.attach(2);
  servo.write(45);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Start the server
server.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
}

void loop() {
// Check if a client has connected
WiFiClient client = server.available();
if (!client) {
return;
}

// Wait until the client sends some data
Serial.println("new client");
while(!client.available()){
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("/servoleft") != -1) {
  servo.write(90);
//digitalWrite(ledPin, HIGH);
//value = HIGH;
} 
if (request.indexOf("/servoright") != -1){
  servo.write(45);
//digitalWrite(ledPin, LOW);
//value = LOW;
}

//Set ledPin according to the request
//digitalWrite(ledPin, value);

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  do not forget this one
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.print("Led pin is now: ");

if(value == HIGH) {
client.print("On");  
} else {
client.print("Off");
}
client.println("<br><br>");
client.println("Click <a href=\"/servoleft\">here</a> turn the LED on pin 2 ON<br>");
client.println("Click <a href=\"/servoright\">here</a> turn the LED on pin 2 OFF<br>");
client.println("</html>");

delay(1);
Serial.println("Client disconnected");
Serial.println("");
}
