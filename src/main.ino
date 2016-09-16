/* WIFI-Briefkasten */
/* Paul Hein */
/* 13.09.2016 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "config.h"

/******************************************************************************/

/* ESP I/O Config */
const int buttonPin = 4;

/******************************************************************************/
void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Use WiFiClientSecure class to create TLS connection */
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.print(host);
  if(!client.connect(host, httpsPort))
  {
    Serial.println("conection failed");
    return;
  }

  if(client.verify(fingerprint, host)) { Serial.println("\ncertificate matches"); }
  else { Serial.println("\ncertificate doesn't match"); }

  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += key;
  String data = "\n{\"value1\":\"test1\",\"value2\":\"test2\",\"value3\":\"test3\"}";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: esp8266\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(data.length()) + "\r\n\r\n");
  client.print(data);



  Serial.println("request sent");

  //print the response

  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}

void loop() {}
