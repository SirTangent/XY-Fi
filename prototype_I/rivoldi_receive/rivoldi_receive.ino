/*
  Rivoldi RF Library (1.0.0) - Receiving Code
  
  Author: Wyatt Phillips
  Email: datscigye@gmail.com
*/

int receiver = 3;
int led_rx = 13;

#define address 2017 // The integer address of this device
#define packet_size 32 // The size of the packet's content. This excludes the header and footer!
#define receiving_speed 2000 // The microseconds it takes for one bit to be received
#define receiveThreshold 512 // Used to differentiate analog signals from RF module

char packet[packet_size + 9];
char message[packet_size];

unsigned int getChecksumLRC(char buffer[], int n) {
  unsigned int x = 0;
  for(int i = 0; i < n; i++) {
    x += buffer[i];
  }
  return x;
}

int rawRecieve(bool enable_led) {
  if(analogRead(receiver) >= receiveThreshold) {
    digitalWrite(led_rx, HIGH);
    return 1;
  }else{
    digitalWrite(led_rx, LOW);
    return 0;
  }
  delayMicroseconds(receiving_speed);
}

char receiveChar() {
  char x;
  for(int b = 7; b > -1; b--) {
    bitWrite(x, b, rawRecieve(true));
  }
  return x;
}

int receiveInt() {
  int x;
  for(int b = 15; b > -1; b--) {
    bitWrite(x, b, rawRecieve(true));
  }
  return x;
}

char getSplitInt(int x, bool rightside) {
  char n;
  
  if(rightside) {
    for(int i = 7; i > -1; i--) {
      bitWrite(n, i, bitRead(x, i));
    }
  }else{
    for(int i = 15; i > 7; i--) {
      bitWrite(n, i-8, bitRead(x, i));
    }
  }
}

bool listenPacket(char buffer[]) {
  if(receiveChar() == 67) {
    /*
    if(int a = receiveInt() == address){
      buffer[0] = 67;
      buffer[1] = getSplitInt(a, false);
      buffer[2] = getSplitInt(a, true);
      return true;
    }
    */
    int a = receiveInt();
    buffer[0] = 67;
    buffer[1] = getSplitInt(a, false);
    buffer[2] = getSplitInt(a, true);
    return true;
  }
  return false;
}

void receivePacket(char buffer[], int buffer_size) {
  for(int i = 3; i < buffer_size; i++) {
    buffer[i] = receiveChar();
  }
}

void setup() {
  pinMode(led_rx, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  if(listenPacket(packet)) {
    receivePacket(packet, sizeof(packet));
    
    Serial.print("Packet Received: ");
    for(int i = 0; i < sizeof(packet); i++){
      Serial.print((int) packet[i]);
      Serial.print(" ");
    }
    Serial.println();
  }

  /*
  for(int i = 0; i < sizeof(packet); i++) {
    packet[i] = receiveChar();
  }
  
  Serial.print("Packet Received: ");
    for(int i = 0; i < sizeof(packet); i++){
      Serial.print((int) packet[i]);
      Serial.print(" ");
    }
  Serial.println();
  */
  delayMicroseconds(receiving_speed * 100);
}
