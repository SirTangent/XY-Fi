/*
	Final Release of code for controlling the sonar system. (Ver. 2.0)
	Author: Wyatt Phillips
	
	Copyright 2017 Wyatt Phillips
	
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
	
		http://www.apache.org/licenses/LICENSE-2.0\
		
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include <Servo.h>

Servo anchor1, anchor2, anchor3;

char state; // 0 -> Scanning Mode, 1 -> Tracking Mode

int anchor1_rot, anchor1_inc;
int anchor2_rot, anchor2_inc;
int anchor3_rot, anchor3_inc;

#define ANGLE_MAX 180
#define ANGLE_MIN 0

#define anchor1_pin 3
#define anchor2_pin 5
#define anchor3_pin 6

#define sensor1_trig 4
#define sensor2_trig 7
#define sensor3_trig 8

#define sensor1_echo 9
#define sensor2_echo 10
#define sensor3_echo 12

#define piezo 11

int area_base = 80;
int area_height = 80;

void updateServos(bool debug) {
  anchor1.write(anchor1_rot);
  anchor2.write(anchor2_rot);
  anchor3.write(anchor3_rot);

  if(debug) {
    Serial.print(anchor1_rot);
    Serial.print(", ");
    Serial.print(anchor2_rot);
    Serial.print(", ");
    Serial.println(anchor3_rot);
  }
}
 
int getDistance(int trigPin, int echoPin, long timeout) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH, timeout)*0.034/2;
}

void setAngle(int* var, int angle) {
  if(angle < ANGLE_MIN) {
    var = ANGLE_MIN;
  }else if (angle > ANGLE_MAX) {
    var = ANGLE_MAX;
  }else {
    var = angle;
  }
}

void setup() {
  Serial.begin(115200);
  
  anchor1.attach(anchor1_pin);
  anchor2.attach(anchor2_pin);
  anchor3.attach(anchor3_pin);

  pinMode(sensor1_trig, OUTPUT);
  pinMode(sensor2_trig, OUTPUT);
  pinMode(sensor3_trig, OUTPUT);

  pinMode(sensor1_echo, INPUT);
  pinMode(sensor2_echo, INPUT);
  pinMode(sensor3_echo, INPUT);
  
  pinMode(piezo, OUTPUT);
  
  anchor1_rot = ANGLE_MAX/2+ANGLE_MIN;
  anchor2_rot = ANGLE_MAX/2+ANGLE_MIN;
  anchor3_rot = ANGLE_MAX/2+ANGLE_MIN;
  
  anchor1_inc = 1;
  anchor2_inc = -1;
  anchor3_inc = 1;

  state = 0;
  
  updateServos(false);

  tone(piezo, 1000);
  delay(1000);
  noTone(piezo);
}

void loop() {
  // Send Data
  Serial.print(anchor1_rot);
  Serial.print(",");
  Serial.print(getDistance(sensor1_trig, sensor1_echo, 10000));
  Serial.print(",");
  Serial.print(anchor2_rot);
  Serial.print(",");
  Serial.print(getDistance(sensor2_trig, sensor2_echo, 10000));
  Serial.print(",");
  Serial.print(anchor3_rot);
  Serial.print(",");
  Serial.print(getDistance(sensor3_trig, sensor3_echo, 10000));
  Serial.println();
  
  // Update Locations
  if(anchor1_rot >= ANGLE_MAX  || anchor1_rot <= ANGLE_MIN) {
    anchor1_inc *= -1;
  }
  if(anchor2_rot >= ANGLE_MAX  || anchor2_rot <= ANGLE_MIN) {
    anchor2_inc *= -1;
  }
  if(anchor3_rot >= ANGLE_MAX  || anchor3_rot <= ANGLE_MIN) {
    anchor3_inc *= -1;
  }
  
  anchor1_rot += anchor1_inc;
  anchor2_rot += anchor2_inc;
  anchor3_rot += anchor3_inc;
  
  updateServos(false);
  delay(15);
}
