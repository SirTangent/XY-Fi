import peasy.*;
import processing.serial.*;
PGraphics canvas;
PeasyCam cam;
Serial receiver;

int radius = 0;

void drawGraph(int center_x, int center_y, int center_z, int segments, int segsize, int size) {
  canvas.stroke(255, 0, 0);
  canvas.line(center_x, center_y, center_z, center_x+size, center_y, center_z);
  canvas.line(center_x, center_y, center_z, center_x, center_y+size, center_z);
  canvas.line(center_x, center_y, center_z, center_x, center_y, center_z+size);
  
  canvas.stroke(255);
  for(int x = 0; x < size; x+=(size/segments)) {
    canvas.line(center_x+x, center_y-segsize, center_z, center_x+x, center_y+segsize, center_z);
  }
  canvas.text("X", center_x+size, center_y+16, center_z);
  
  for(int y = 0; y < size; y+=(size/segments)) {
    canvas.line(center_x-segsize, center_y+y, center_z, center_x+segsize, center_y+y, center_z);
  }
  canvas.text("Y", center_x+16, center_y+size, center_z);
  
  for(int z = 0; z < size; z+=(size/segments)) {
    canvas.line(center_x-segsize, center_y, center_z+z, center_x+segsize, center_y, center_z+z);
  }
  
  canvas.pushMatrix();
  canvas.rotateX(radians(270));
  canvas.text("Z", center_x+16, center_y-size, center_z);
  canvas.popMatrix();
}

void setup() {
  size(1920, 1080, P2D);
  frame.setResizable(true);
  canvas = createGraphics(width, height, P3D);

  cam = new PeasyCam(this, canvas, 100);
  cam.setMinimumDistance(100);
  cam.setMaximumDistance(1000);

  receiver = new Serial(this, "COM10", 9600);
}

void draw() {
 canvas.beginDraw();
 canvas.background(0);
 canvas.lights();
 textAlign(LEFT);
   
 drawGraph(0, 0, 0, 8, 10, 320);
 
 if(receiver.available() > 0) {
   radius = receiver.read();
 }
 
 canvas.pushMatrix();
 canvas.translate(0, 0, 0);
 canvas.noStroke();
 canvas.fill(255,255,255);
 canvas.sphere(radius);
 canvas.popMatrix();
 canvas.endDraw();
 
 image(canvas, 0, 0);
 
 fill(255);
 textSize(24);
 text("XY-Fi: Single Point Test", 32, height-60);
 textSize(14);
 text("© 2017 Wyatt Phillips. All rights reserved.", 30, height-34);
 
 fill(18, 139, 153);
 textSize(24);
 textAlign(RIGHT);
 text("Distance: " + radius + "cm", width-16, height-32);
}