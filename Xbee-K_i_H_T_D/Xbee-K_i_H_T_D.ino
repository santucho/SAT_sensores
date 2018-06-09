#include "LowPower.h"
#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#include "DHT.h"
#define DHTPIN 4     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial XBeeSerial =  SoftwareSerial(3, 2);

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.



int pings = 5;
int values[5];
int sumatoria = 0;
int distancia = 0;
int promedio=0;
long i = 0;
String hilera;
String apiKey = "WS7WJWYNLM3SP2TU";
void setup() {
  //Serial.begin(9600);
  dht.begin();
  XBeeSerial.begin(9600);
}

void loop() {

  for (int i = 0; i < pings; i++) {
    delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    //Serial.print("Ping: ");
    distancia = sonar.ping_cm();
    //Serial.print(distancia); // Send ping, get distance in cm and print result (0 = outside set distance range)
    //Serial.println("cm");
    sumatoria += distancia;
  }
  //Serial.println(sumatoria);
  //Serial.print("\t\t");
  promedio=(sumatoria/pings);
  //Serial.println(promedio);
  sumatoria = 0;  

  hilera += apiKey;
  hilera += ",";
  hilera += i;
  hilera += ",";
  hilera += dht.readHumidity();
  hilera += ",";
  hilera += dht.readTemperature();
  hilera += ",";
  hilera += promedio;
  hilera += "\n";
  //Serial.print(hilera);
  XBeeSerial.print(hilera);
  hilera = "";
  delay(1000);
//15
  for (int i = 0; i < 37 ; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  i++;
}
