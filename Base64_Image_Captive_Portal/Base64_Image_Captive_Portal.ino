#include <WiFi.h>
#include <DNSServer.h>
#include "HTMLPAGE.h"

const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4);
DNSServer dnsServer;
WiFiServer server(80);

void setup() { 
  WiFi.mode(WIFI_AP);
  WiFi.softAP("test"); //put ssid here
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.start(DNS_PORT, "*", apIP);

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(HTMLPAGE);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
