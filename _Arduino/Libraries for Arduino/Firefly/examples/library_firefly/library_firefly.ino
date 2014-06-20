#include <Firefly.h>

Iot firefly= Iot(12);
int i=0;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if(i==0)
    firefly.on();
   
  boolean tester = firefly.auto_off(true);
  Serial.println(tester);
  i++;
//  firefly.off();
  /*morse.on(); morse.on(); morse.on();
  morse.off(); morse.off(); morse.off();
  morse.on(); morse.on(); morse.on();*/
  // delay(3000);
}

