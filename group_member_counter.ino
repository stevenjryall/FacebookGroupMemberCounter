/**
 * By Steven Ryall
 * 
 * This program is built specifically to query the number of members in a group
 * from Facebook using the 'Facebook Graph' API. It will only send over the
 * number if the request contains an access token generated by an admin
 * of the group in question (as far as I'm aware, at the time of writing). 
 * 
 * Credit goes to "sjsd" over at "https://www.instructables.com/Facebook-Fan-Count/" 
 * for the original code! I simply tweaked it for my needs & spruced it up for the 
 * FTIJPI community! His process of doing this is expired now since Facebook
 * changed it's API's and Developer App setup...
 * 
 * Updates needed:
 * 
 * . A menu system utilising buttons to view different data (maybe from different groups?)
 * and toggle debug mode!
 * . A screensaver since OLED screens can suffer from burn-in
 * . Create a server/AP so the user can connect and change credentials from their phone 
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 **/

// Load necessary libraries
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306Wire.h"

// This is used to show the splashscreen, you can replace this with any xbm image data but it has to be very low res... 
// I use 64 x 64!
#include "images.h"

// Debug mode shows what's going on in the background on screen so you can see if anything has failed...
// It also shows when an update is incoming!
// 1 for on, 0 for off
int debugMode = 0;

// First time indicator ensures we don't show 'Updating...' the first time the system is run
// but does for each update after that
// 1 for on, 0 for off
int firstTime = 1;

// Initialising the OLED display...
// This creates a connection via I2C using the Wire library!
// SDA pin on display to - D3 pin on ESP8266 board
// SCL pin on display to - D5 pin on ESP8266 board
SSD1306Wire  display(0x3c, D3, D5);

// Fill in your WiFi credentials
const char* ssid = "YOUR-NETWORK-NAME(SSID)"; 
const char* password = "YOUR-NETWORK-PASSWORD";

// Various strings to create the hostname we'll be connecting to
// Ensure to fill this section out carefully and correctly where necessary!
// You don't need your App ID, or your App secret or anything... 
// Just the group ID you want to query, the access token you generated...
// And the fingerprint of Facebook via your browser if you wish! 
const char* host = "graph.facebook.com";
const String url = "/v10.0/YOUR-GROUP-ID/?fields=member_count&access_token=YOUR-ACCESS-TOKEN";
const int httpsPort = 443;
const char* fingerprint = "FINGERPRINT-OF-FACEBOOK-VIA-YOUR-BROWSER";

void setup() {
  
  // Create a Serial connection for debug mode
  Serial.begin(9600);

  // Initialise display
  display.init();

  // Not including this line means everything will be up-side-down! :( 
  display.flipScreenVertically();

  // You can use ArialMT_Plain_10, ArialMT_Plain_16 and ArialMT_Plain_24!
  display.setFont(ArialMT_Plain_10);

  // Clear the display in case there's anything left...
  display.clear();

  if (debugMode == 1) {

    // The 'drawString' method takes an x and y value for positioning and a string to display! 
    display.drawString(0, 0, "Initialising...");

    // Use 'display.display();' every time you want to add something new to the screen, otherwise it won't show...
    display.display();

    // Add a little delay after displaying to stabilise the system...
    delay(10);
    
  } else {

    // A fun splashscreen! 
    display.drawString(34, 0, "Tnx Google...");
    display.drawXbm(34, 10, splash_width, splash_height, splash_bits);
    display.display();

     // Change how long the splashscreen displays - 1000 = 1 second!
    delay(3000);

    // Loading screen...
    display.clear();
    display.drawString(14, 28, "Loading, please wait...");
    display.display();
    delay(10);
    
  }

  // Connect to WiFi using the credentials above...
  WiFi.begin(ssid, password);
  
  if (debugMode == 1) {

    // Show the user we're connecting...
    Serial.println();
    Serial.print("Connecting ");
    display.drawString(0, 10, "Connecting to WiFi...");
    display.display();
    delay(10);
    
  }

  // Whilst we're not connected, do nothing until we are...
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  if (debugMode == 1) {

    // Show our local IP address...
    Serial.println("Connected to...");
    Serial.println(WiFi.localIP());
    display.drawString(0, 30, "Connected to...");
    display.drawString(0, 40, WiFi.localIP().toString());
    display.display();
    delay(1500);
    
  }
}

void loop() {
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  String wifistatus = "WIFI OK";
  
  // Use WiFiClientSecure class to create TLS connection with the host!
  WiFiClientSecure client;

  // [WARNING: This line creates a connection to the host without validating an SSL certificate...
  // You can check out "https://maakbaas.com/esp8266-iot-framework/logs/https-requests/" for more
  // information and more secure methods of making the connection!]
  client.setInsecure();
  
  if (debugMode == 1) {

    // Show the hostname we're connecting to...
    Serial.print("connecting to ");
    Serial.println(host);
    display.drawString(0, 0, "Connecting to...");
    display.drawString(0, 10, String(host));
    display.display();
    delay(10);
    
  }
  
  if (!client.connect(host, httpsPort)) {
    
    String wifistatus = "WIFI FAIL";
    if (debugMode == 1) {

      // If the connection to the host fails for some reason, say so here...
      Serial.println("connection failed");
      display.drawString(0, 20, "Connection failed...");
      display.display();
      delay(10);
      
    }
    return;
  }
  
  if (client.verify(fingerprint, host)) {

    if (debugMode == 1) {

      // Here we can verify if the fingerprint of the host matches the one we got from our browser! 
      // Show the user the certificate matched!
      Serial.println("Matching certificate!");
      display.drawString(0, 20, "Matching certificate!");
      display.display();
      delay(10);
      
    }
  } else {
    if (debugMode == 1) {

      // Show the user the certificate didn't match...
      Serial.println("Certificate does not match...");
      display.drawString(0, 20, "Certificate does not match...");
      display.display();
      delay(10);
      
    }
  }

  // Send the request for the information to the host...
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
               
  if (debugMode == 1) {

    // Show the user the request was sent...
    Serial.println("Request sent...");
    display.drawString(0, 30, "Request sent...");
    display.display();
    delay(10);
    
  }
  
  while (client.connected()) {

    // Confirm if and when something is sent back...
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      if (debugMode == 1) {
        
        Serial.println("Headers received...");
        display.drawString(0, 40, "Headers received...");
        display.display();
        delay(10);
        
      }
      break;
    }
  }

  // We should have received JSON information...

  // data: [ "information" ] 

  // So let's parse it! 
  String line = client.readStringUntil('\n');
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(line);

  // We specifically want the 'member_count' field from the response...
  int member_count = root[String("member_count")];

  // If firstTime is 0, we have looped through the system more than once...
  if (firstTime == 0) {
    if (debugMode == 1) {
      
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.drawString(40, 0, "Updating...");
      display.drawString(50, 20, "3...");
      display.display();
      delay(1000);
      display.drawString(50, 30, "2...");
      display.display();
      delay(1000);
      display.drawString(50, 40, "1...");
      display.display();
      delay(1000);
      
    }
  } else if (firstTime == 1) {

    // We set 'firstTime' to be 1 when the device boots...
    // So by setting it to 0 here we ensure it's 0 for the rest of the time the device is on!
    firstTime = 0;
    
  }

  // If the number we parsed from the host is more than 0... 
  if (member_count > 0) {
    if (debugMode == 1) {
      
      Serial.print("FTIJPI member count:");
      Serial.println(member_count);
      
    }

    // Display the number on screen! 
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(13, 10, "FTIJPI member count:");

    // You can change font size on the fly to create a big number after small text...
    display.setFont(ArialMT_Plain_24);
    display.drawString(23, 30, String(member_count));
    display.display();
    delay(10);
    
  } else {
    if (debugMode == 1) {

    // Oh no... No members! What happened?
    Serial.print("Returned 0...");
    display.setFont(ArialMT_Plain_10);
    display.drawString(13, 10, "Returned 0...");
    display.display();
    delay(10);
      
    }
  }

  if (debugMode == 1) {

    // We do a 3 second countdown during the update in debug mode, so we'll account for that here
    // to ensure we're still waiting only 30 seconds between each update...
  delay(27000);
  
  } else {

    // Wait 30 seconds between each update, as facebook only allows 200 queries per hour...
  delay(30000);
    
  }
}
