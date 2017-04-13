/*
  Rivoldi RF Library (1.0.0) - Transmitter Code
  
  Author: Wyatt Phillips
  Email: datscigye@gmail.com
*/

int transmitter = 2; // Pin going to the RF module
int led_tx = 13; // Pin that outputs transmission state

#define address 8020 // The integer address of this device
#define packet_size 64 // The size of the packet's content. This excludes the header and footer!
#define transmission_speed 2000 // The microseconds it takes for one bit to be transmitted

char packet_buffer[packet_size];

bool getSerialInput(char buffer[], int n) {
  if(Serial.available() > 0) {
    for(int i = 0; i < n; i++) {
      buffer[i] = 0;
    }
    for(int i = 0; Serial.available() > 0; i++) {
      buffer[i] = Serial.read();
    }   
    return true;
  }
  return false;
}

unsigned int getChecksumLRC(char buffer[], int n) {
  unsigned int x = 0;
  for(int i = 0; i < n; i++) {
    x += packet_buffer[i];
  }
  return x;
}

void setup() {
    pinMode(transmitter, OUTPUT);
    pinMode(led_tx, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  if(getSerialInput(packet_buffer, sizeof(packet_buffer))) {
    Serial.print("Input: ");
    Serial.println(packet_buffer);

    Serial.print("Checksum (x8): ");
    Serial.println(getChecksumLRC(packet_buffer, sizeof(packet_buffer)));
  }
  delay(500);
}
