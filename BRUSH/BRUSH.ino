#include <TinkerKit.h>
//Create "accelerometer" object of
//TKAccelerometer class.
TKAccelerometer acc(I0, I1); 
int accelX;
int accelY;
//Same with two leds
TKLed Led(O0);

unsigned long deltaT = 1000;

int state = LOW;

int avgX = 0;
int avgY = 0;

float ravgX = 0;
float ravgY = 0;
float ravgLagX = 0;
float ravgLagY = 0;
float deltaX = 0;
float deltaY = 0;

unsigned long lastTime = millis();
void setup() {
  Serial.begin(9600);
}

void loop() {   
  accelX = abs(acc.readX());
  accelY = abs(acc.readY());
  ravgX = ravgX * 0.5 + 0.5 * (float)accelX;
  ravgY = ravgY * 0.5 + 0.5 * (float)accelY;
  
  deltaX = ravgX - ravgLagX;
  deltaY = ravgY - ravgLagY;
  
  ravgLagX = ravgX;
  ravgLagY = ravgY;
  
  if ((millis() - lastTime) > deltaT) {
    lastTime = millis();

    Serial.print("x: ");
    Serial.print(deltaX);
    Serial.print(", y: ");
    Serial.println(deltaY);
  }
}

