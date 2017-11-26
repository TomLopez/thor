#include <Wire.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define SLAVE_ADDRESS 0x04
#define trigPin 5
#define echoPin 6
#define led 12
#define led2 4
int question = 0;
int state = 0;
long duration, distance;
float voltage = 0;

void setup() {
Serial.begin(9600); // start serial for output
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(led, OUTPUT);
// initialize i2c as slave
Wire.begin(SLAVE_ADDRESS);

// define callbacks for i2c communication
Wire.onReceive(receiveData);
Wire.onRequest(sendData);

Serial.println('Ready!');
}

void loop() {
delay(1000);
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance < 20) {  // This is where the LED On/Off happens
    digitalWrite(led,HIGH);
    digitalWrite(led2,LOW);
  } else {
    digitalWrite(led,LOW);
    digitalWrite(led2,HIGH);
  }
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = sensorValue * (5.0 / 1023.0);
}

// callback for received data
void receiveData(int byteCount){

while(Wire.available()) {
question = Wire.read();
Serial.print('data received: ');
Serial.println(question);

}
}

// callback for sending data
void sendData(){
  //delay(100);
  //char text= distance+" "+voltage;
  if(question==1) {
    Wire.write(distance);
  } else if(question==2){
    int numberBack=voltage*100;
    Wire.write(numberBack);
  }
  else {
    Wire.write(0);
  }
}
