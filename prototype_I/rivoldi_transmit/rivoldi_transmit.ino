/*
  Rivoldi RF Library (1.0.0) - Transmitter Code
  
  Author: Wyatt Phillips
  Email: datscigye@gmail.com
*/

int transmitter = 2; // Pin going to the RF module
int led_tx = 13; // Pin that outputs transmission state

#define address 8020 // The integer address of this device
#define packet_size 32 // The size of the packet's content. This excludes the header and footer!
#define transmission_speed 2000 // The microseconds it takes for one bit to be transmitted

char packet[packet_size + 9];
char message[packet_size];

void getSerialInput(char buffer[], int n, bool return_input) {
  while(Serial.available() <= 0) {
    delayMicroseconds(100); 
  }
  for(int i = 0; i < n; i++) {
    buffer[i] = 0;
  }
  delay(100);
  for(int i = 0; Serial.available() > 0; i++) {
    buffer[i] = Serial.read();
  }
  if(return_input) {
    Serial.println(message);
  }
}

unsigned int getChecksumLRC(char buffer[], int n) {
  unsigned int x = 0;
  for(int i = 0; i < n; i++) {
    x += buffer[i];
  }
  return x;
}

char getSplitInt(int x, bool rightside) {
  char n;
  
  if(rightside) {
    for(int i = 7; i > -1; i--){
      bitWrite(n, i, bitRead(x, i));
    }
  }else{
    for(int i = 15; i > 7; i--){
      bitWrite(n, i-8, bitRead(x, i));
    }
  }
  
  return n;
}

void buildPacket(char buffer[], int buffer_length, int destination, char content[], int content_length){
  int n = 0;
  // Write Master Callsign
  buffer[n] = 67;
  n++;
  // Write Destination Address
  buffer[n] = getSplitInt(destination, false);
  n++;
  buffer[n] = getSplitInt(destination, true);
  n++;
  // Write Sender Address
  buffer[n] = getSplitInt(address, false);
  n++;
  buffer[n] = getSplitInt(address, true);
  n++;
  // Write Packet Length
  buffer[n] = getSplitInt(packet_size, false);
  n++;
  buffer[n] = getSplitInt(packet_size, true);
  n++;
  // Write Content
  for(int i = 0; i < content_length; i++) {
    buffer[n] = content[i];
    n++;
  }
  // Write Checksum
  unsigned int checksum = getChecksumLRC(buffer, buffer_length);
  buffer[n] = getSplitInt(checksum, false);
  n++;
  buffer[n] = getSplitInt(checksum, true);
  n++;
}

void rawTransmit(int state, bool enable_led) {
  if(state) {
    digitalWrite(led_tx, HIGH);
    digitalWrite(transmitter, HIGH);
  }else{
    digitalWrite(led_tx, LOW);
    digitalWrite(transmitter, LOW);
  }
  delayMicroseconds(transmission_speed);
}  

void transmitPacket(char buffer[], int n) {
  for(int i = 0; i < n; i++) {
    for(int b = 7; b > -1; b--) {
      rawTransmit(bitRead(buffer[i], b), true);
    }
  }
  digitalWrite(led_tx, LOW);
  digitalWrite(transmitter, LOW);
}

void setup() {
    pinMode(transmitter, OUTPUT);
    pinMode(led_tx, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    String msg = "Hello, Swampscott!";
    msg.toCharArray(message, sizeof(message));

    Serial.print("Building Packet: ");
    buildPacket(packet, sizeof(packet), 2017, message, sizeof(message));
    for(int i = 0; i < sizeof(packet); i++){
      Serial.print((int) packet[i]);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("transmitting in Five Seconds (S:");
    Serial.print(transmission_speed);
    Serial.println("μs)...");

    transmitPacket(packet, sizeof(packet));

    Serial.println("Done!");
    Serial.println();
}
