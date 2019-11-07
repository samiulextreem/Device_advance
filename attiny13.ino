int Switch = 0;
int Gate = 1;

void setup() {
 // initialize pin 4 (ATtiny leg 3) as an output.
 pinMode(Gate, OUTPUT);
 pinMode(Switch, INPUT);
 digitalWrite(Gate, LOW);


}
void loop() {
  if (digitalRead(Switch) == LOW && digitalRead(Gate) == HIGH){
    digitalWrite(Gate, LOW);   // turn the LED off (HIGH is the voltage level)
    while(digitalRead(Switch) == LOW){
      delay(300);
    }
     
  }
  if (digitalRead(Switch) == LOW && digitalRead(Gate) == LOW){
    digitalWrite(Gate, HIGH);   // turn the LED on (HIGH is the voltage level)
    while(digitalRead(Switch) == LOW){
      delay(300);
    }
  }

} 
