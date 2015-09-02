#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ssid = "PJ2";
#define password = "m1e2l3i4n5a6";
MDNSResponder mdns;

#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

ESP8266WebServer server ( 80 );

void handleRoot() {
  float temp;
  DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
  
  server.send ( 200, "application/json", String(temp, 1) );
}

void setup ( void ) {
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( mdns.begin ( "esp8266", WiFi.localIP() ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  mdns.update();
  server.handleClient();
}
