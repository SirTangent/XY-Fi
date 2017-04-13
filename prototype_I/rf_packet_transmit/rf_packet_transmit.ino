int led = 13;
int transmitter = 2;
int button = 5;
int bitcycle = 25;

#define packet_size 32
char buffer[packet_size] = "Hello, Swampscott!";

void rfTransmit(int state) {
  if(state) {
    digitalWrite(transmitter, HIGH);
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(transmitter, LOW);
    digitalWrite(led, LOW);
  }
  delay(bitcycle);
}

void rfPacketSend(unsigned int destination) {
  for(int i = 0; i < sizeof(buffer); i++) {
    for(int b = 7; b > -1; b--) {
      rfTransmit(bitRead(buffer[i], b));
    }
  }
  Serial.println();
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(transmitter, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  rfPacketSend(2928);
  delay(2000);
}
