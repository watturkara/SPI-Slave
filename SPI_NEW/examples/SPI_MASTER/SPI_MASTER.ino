#include <SPI_NEW.h>

char data[7] = {
  'H','e','l','L','o','\n','\0'};
int index;

void setup()
{
  Serial.begin(9600);
  SPI.begin(SPI_MASTER);
}

void loop()
{
  digitalWrite(SS, LOW);
  SPI.transfer(data, 7);
  digitalWrite(SS,HIGH);
  Serial.print(data);
  delay(200);
}



