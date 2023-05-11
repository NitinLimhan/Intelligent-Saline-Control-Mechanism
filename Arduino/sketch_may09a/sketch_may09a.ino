volatile int IRQcount;
int pin = 2;
int pin_irq = 0; //IRQ that matches to pin 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  attachInterrupt(pin_irq, IRQcounter, FALLING);
}

void IRQcounter() {
  IRQcount++;
}

void loop() {
  // put your main code here, to run repeatedly:
 // IRQcount = 0;
  delay(25);
  int result = IRQcount;
  Serial.print(F("Counted = "));
  Serial.println(result);
}
