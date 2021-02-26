#include <SPI_NEW.h>

char data[7] = {
  'C','a','k','e','s','\n','\0'};
int index = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin(SPI_SLAVE);
  SPI.attachInterrupt(hungry);
}

void loop()
{

}
void hungry()
{
  Serial.print(data[index]);
  SPI.transfer(data, 7);
  Serial.print(data);
}




