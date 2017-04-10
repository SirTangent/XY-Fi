int led = 13;
int receiver = 3;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  if(analogRead(receiver) >= 512) {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
}
