#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(3, 2);     // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin


//String apiKey = "WS7WJWYNLM3SP2TU";     // replace with your channel's thingspeak WRITE API key

String ssid = "UNE_F57D";  // Wifi network SSID
String password = "00002423704223"; // Wifi network password
boolean DEBUG = true;
String nan = " NAN";


void setup() {
  DEBUG = true;         // enable debug serial
  Serial.begin(9600);
  espSerial.begin(9600);  // enable software serial
  // Your esp8266 module's speed is probably at 115200.
  // For this reason the first time set the speed to 115200 or to your esp8266 configured speed
  // and upload. Then change to 9600 and upload again

  //espSerial.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);

  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);


  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  //espSerial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\""); // set your home router SSID and password
  // showResponse(5000);

  if (DEBUG)  Serial.println("Setup completed");

}
void loop() {
  if (Serial.available() > 0) {
    String apiKey = Serial.readStringUntil(',');
    Serial.read();
    String first  = Serial.readStringUntil(',');
    Serial.read(); //next character is comma, so skip it using this
    String second = Serial.readStringUntil(',');
    Serial.read();
    String third  = Serial.readStringUntil(',');
    Serial.read();
    String fourth  = Serial.readStringUntil('\n');
    
    Serial.println(first);
    Serial.println(second);
    Serial.println(third);
    Serial.println(fourth);
    thingSpeakWrite(apiKey, first, second, third,fourth);
  }
}

void showResponse(int waitTime) {
  long t = millis();
  char c;
  while (t + waitTime > millis()) {
    if (espSerial.available()) {
      c = espSerial.read();
      if (DEBUG) Serial.print(c);
    }
  }
}

boolean thingSpeakWrite(String apiKey, String nroMuestra, String valorHumedad, String valorTemperatura, String valorNivel) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if (espSerial.find("Error")) {
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }


  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  getStr += "&field1=";
  getStr += String(nroMuestra); //# toma
  getStr += "&field2=";
  if (valorHumedad.equals(nan) || valorTemperatura.equals(nan)) {
    getStr += "-1"; // humedad
    getStr += "&field3=";
    getStr += "-1"; // temperatura
  } else {
    getStr += String(valorHumedad); // humedad
    getStr += "&field3=";
    getStr += String(valorTemperatura); // temperatura
  }
  getStr += "&field4=";
  getStr += String(valorNivel);

  // getStr +="&field3=";
  // getStr += String(valorTemperatura);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);

  delay(100);
  if (espSerial.find(">")) {
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else {
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
