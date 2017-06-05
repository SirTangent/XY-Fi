/*
	Final Release of code for displaying the results of system. (Ver. 1.0)
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

import processing.serial.*;

Serial board;
String serial_com = "COM15";
int[] values = new int[6];

int area_multiplier = 8;
int area_base = 80;
int area_height = 55;

int point1x, point2x, point3x;
int point1y, point2y, point3y;

int servo1_angle, servo2_angle, servo3_angle;
int servo1_dist, servo2_dist, servo3_dist;

void setup() {
  size(1280, 720, P2D);
  surface.setResizable(true);
  
  board = new Serial(this, serial_com, 115200);
  board.bufferUntil('\n');
}

void draw() {
   background(220);
   
   point1x = width/2;
   point1y = (height/2)-(area_height*area_multiplier/2);
   point2x = (width/2)-(area_base*area_multiplier/2);
   point2y = (height/2)+(area_height*area_multiplier/2);
   point3x = (width/2)+(area_base*area_multiplier/2);
   point3y = (height/2)+(area_height*area_multiplier/2);
   
   stroke(0);
   noFill();
   
   triangle(point1x, point1y, point2x, point2y, point3x, point3y);
   
   ellipseMode(RADIUS);
   noStroke();
   fill(255, 0, 0);
   ellipse(point1x, point1y, 10, 10);
   fill(0, 255, 0);
   ellipse(point2x, point2y, 10, 10);
   fill(0, 0, 255);
   ellipse(point3x, point3y, 10, 10);
   
   servo1_angle = values[0];
   servo1_dist = values[1];
   servo2_angle = values[2];
   servo2_dist = values[3];
   servo3_angle = values[4];
   servo3_dist = values[5];
   
   noFill();
   
   stroke(255, 0, 0);
   line(point1x, point1y, point1x-cos(radians(servo1_angle))*servo1_dist*area_multiplier, point1y+sin(radians(servo1_angle))*servo1_dist*area_multiplier);
   
   stroke(0, 255, 0);
   line(point2x, point2y, point2x+cos(radians(servo2_angle-60))*servo2_dist*area_multiplier, point2y-sin(radians(servo2_angle-60))*servo2_dist*area_multiplier);
   
   stroke(0, 0, 255);
   line(point3x, point3y, point3x-cos(radians(servo3_angle-120))*servo3_dist*area_multiplier, point3y+sin(radians(servo3_angle-120))*servo3_dist*area_multiplier);
   
   textSize(32);
   noStroke();
   fill(255, 0, 0);
   text(serial_com, 16, 48);
   textSize(20);
   text("Point R: "+servo1_dist+"cm (@"+servo1_angle+"°)", 16, 80);
   text("Point G: "+servo2_dist+"cm (@"+servo2_angle+"°)", 16, 110);
   text("Point B: "+servo3_dist+"cm (@"+servo3_angle+"°)", 16, 140);
   
   fill(0);
   textSize(14);
   text("©2017 Wyatt Phillips. All Rights Reserved.", 16, height-16);
}

void serialEvent(Serial p) {
   String buffer = p.readStringUntil('\n');
   
   if(buffer != null) {
    buffer = trim(buffer);
    values = int(splitTokens(buffer, ","));
   }
}