int led = 13;
int receiver = 3;

int read_val = 0;
int count = 0;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  read_val = analogRead(receiver);
  Serial.println(read_val);
  
  if(read_val >= 512) {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }

  count++;
  delay(50);
}
