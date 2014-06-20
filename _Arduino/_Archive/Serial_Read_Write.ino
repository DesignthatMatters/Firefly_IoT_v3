// arduino device code
int led = 13;            // led pin number

void setup() {
  Serial.begin(9600);    // configure serial
  pinMode(led, OUTPUT);  // configure LED pin
  digitalWrite(led, 0);  // turn LED off
}

void loop() {
  int b = 0;
  // if there's data available
  if (Serial.available () > 0) {
    // read a byte
    b = Serial.read();
    if (b == 0x00) {
      digitalWrite(led, LOW);
      Serial.println(0x10);
      Serial.write(0x10);      
    } else if (b == 0x01) {
      digitalWrite(led, HIGH);
      Serial.write(0x11);
    }
  }
}

