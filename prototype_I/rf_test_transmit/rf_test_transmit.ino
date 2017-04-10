int led = 13;
int transmitter = 2;
int button = 5;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(transmitter, OUTPUT);
}

void loop() {
  if(analogRead(button) >= 512) {
    digitalWrite(led, HIGH);
    digitalWrite(transmitter, HIGH);
  }else{
    digitalWrite(led, LOW);
    digitalWrite(transmitter, LOW);
  }
}
