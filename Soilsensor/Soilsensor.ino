#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

int output = 8;
int input = 7;

void setup() {
  Serial.begin(115200);
  pinMode(output, OUTPUT);
  pinMode(input, INPUT);
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop() {
  uint16_t capread = ss.touchRead(0);
  
  if(capread < 400)
  {
    digitalWrite(output, HIGH);
  }
  // For debugging use arduino serial monitor
  Serial.print("Capacitive: "); 
  Serial.println(capread);
  String status = "";
  if(digitalRead(input))
  {
    status = "Fill water";
  }
  else
  {
    status = "Don't fill water";
  }
  //Serial.println("Status: " + status);
  //Serial.println(digitalRead(input));
  
  delay(100);
  digitalWrite(output, LOW);
}
