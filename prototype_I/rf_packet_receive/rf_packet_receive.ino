int led = 13;
int receiver = 3;
int high_threshold = 512;
int bitcycle = 25;

#define packet_size 32
char packet[packet_size];

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(analogRead(receiver) >= high_threshold) {
    
    digitalWrite(led, HIGH);
    
    for(int i = 0; i < packet_size; i++) {
      Serial.print(i+1);
      Serial.print(":\t");
      for(int b = 7; b > -1; b--) {
        int n = (analogRead(receiver) >= high_threshold) ? 1 : 0;
        bitWrite(packet[i], b, n);
        Serial.print(bitRead(packet[i], b));
        delay(bitcycle);
      }
      Serial.println();
    }

    for(int i = 0; i < sizeof(packet); i++) {
      Serial.print(packet[i]);
    }
    Serial.println();

    bool light_flag = false;
    for(int i = 0; i < 2000; i += 50) {
      if(light_flag) {
        digitalWrite(led, LOW);
        light_flag = false;
      }else{
        digitalWrite(led, HIGH);
        light_flag = true;
      }
      delay(50);
    }

    digitalWrite(led, LOW);
    
  }
  
}
