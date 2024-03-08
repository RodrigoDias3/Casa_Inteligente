# include <LiquidCrystal.h>
# include "DHT.h"

# define TEMP A0

# define DHTTYPE DHT11 

# define LDR1 A1

# define LED1 2

LiquidCrystal LCD(13, 12, 11, 10, 9, 8);

DHT dht(TEMP, DHTTYPE);

int valorDigital;
int luz = 0;       
int valor_sensor = 0;       
int valor_limite = 430; 

void setup()
{
  Serial.begin(9600);

  pinMode(LDR1,INPUT);
  
  pinMode(LED1,OUTPUT);
  digitalWrite (LED1, LOW);
  
  LCD.begin(16,2);
  dht.begin();

  LCD.setCursor(1,0);
  LCD.print("Temperatura");
  LCD.setCursor(1,1);
  LCD.print("AC : OFF");
  LCD.setCursor(13,1);
  LCD.print(" \xDF" "C");
}

void loop()
{
  while (!Serial.available()) {
    luzExterior();
    setLcd();
  }

  char value = Serial.read();
  
  Serial.println(value);
  
  if((value == 'Y')){
    Serial.println("AC ON");
    ligaAC();
  } else if((value == 'U')){
    Serial.println("AC OFF");
    desligaAC();
  }
}

void luzExterior(){
  valor_sensor = analogRead(LDR1); 
  luz = (5.0 * valor_sensor * 100.0)/1024.0;                       
  
  if (luz <= valor_limite)
  {
    digitalWrite (LED1, LOW);
  }
  if (luz > valor_limite)
  {
    digitalWrite (LED1, HIGH);
  }
}

void setLcd(){
  float tmp = dht.readTemperature();
  int graus = (int)tmp;

  LCD.setCursor(1,0);
  LCD.print("Temperatura: ");
  LCD.print(graus);
  LCD.setCursor(13,1);
  LCD.print(" \xDF" "C");
}

void ligaAC(){
  LCD.setCursor(0,1);
  LCD.print("         ");
  LCD.setCursor(1,1);
  LCD.print("AC : ON");
}

void desligaAC(){
  LCD.setCursor(0,1);
  LCD.print("         ");
  LCD.setCursor(1,1);
  LCD.print("AC : OFF");
}
