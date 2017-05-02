int led = 13;
int receiver = 3;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(receiver, INPUT);
}

void loop() {
  if(digitalRead(receiver)) {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
}
