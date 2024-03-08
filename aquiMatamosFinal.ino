#include <Stepper.h>
#include <Servo.h>
#include <Keypad.h>

# define TIMER 5

# define BUZZER 7
# define SERVO 6
# define AC 53
# define PIR 2

# define LED_SALA 3
# define LED_QUARTO 4

# define STEPPER_PIN_1 49
# define STEPPER_PIN_2 47
# define STEPPER_PIN_3 45
# define STEPPER_PIN_4 43

int step_number = 0;
int sensorPIR = 0;

Servo meuServo;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {37, 35, 33, 31};
byte colPins[COLS] = {29, 27, 25, 23};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char codigoInserido[4];
char codigoArmazenado[4] = {'0','0','0','0'};

int ac = 0;
int motor = 0;
int i = 0;
int alarme = 0;
int sala = 0;
int quarto = 0;

void setup() {
  Serial.begin(9600);
  
  meuServo.attach(SERVO);

  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  pinMode(BUZZER,OUTPUT);
  pinMode(PIR, INPUT);

  pinMode(LED_SALA, OUTPUT);
  pinMode(LED_QUARTO, OUTPUT);

  pinMode(AC,OUTPUT);
  digitalWrite (AC, LOW);
}

void loop() {
  intruso();

  char key = keypad.getKey(); // vai buscar botao do keypad
  
  if(key != NO_KEY){
  	Serial.println(key);
  }

  if(key == 'C'){
    leButoes();
    for(int i = 0; i < 4 ; i++)
  	{
      //Serial.print(codigoInserido[i]);
      //Serial.print(" -> ");
      //Serial.println(codigoArmazenado[i]);
  	}
    verificaCodigo(codigoInserido);
  } else if(key == 'A'){
    campainha();
  } else if(key == 'B'){
    mudaCodigo();
    for(int i = 0; i < 4 ; i++)
  	{
      Serial.println(codigoArmazenado[i]);
  	}
  } else if(key == '0'){
    fechPorta();
  } else if(key == '#'){
    setAC();
  } else if(key == '*'){
    setMotor();
  } else if(key == 'D'){
    setAlarme();
  } else if(key == '1'){
    ligaLuzQuarto();
  } else if(key == '2'){
    ligaLuzSala();
  } else if(key == '3'){
    desligaLuz();
  }
}

void ligaLuzQuarto(){
  if(quarto == 1){
    digitalWrite (LED_QUARTO, LOW);
    quarto = 0;
  } else {
    digitalWrite (LED_QUARTO, HIGH);
    quarto = 1;
  }
}

void ligaLuzSala(){
  if(sala == 1){
    digitalWrite (LED_SALA, LOW);
    sala = 0;
  } else {
    digitalWrite (LED_SALA, HIGH);
    sala = 1;
  }
}

void desligaLuz(){
  digitalWrite (LED_SALA, LOW);
  digitalWrite (LED_QUARTO, LOW);

  sala = 0;
  
    quarto = 0;
}

void setAlarme(){
  if(alarme == 1){  // liga alarme
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    alarme = 0;
  } else {  // desliga alarme
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    delay(500);
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    alarme = 1;
  }
}

void tocaAlarme(){
  int i;
  for(i = 0; i < TIMER ; i++){
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    delay(500);
  }
}

void intruso(){
  sensorPIR = digitalRead(PIR);
  
  if(sensorPIR && (alarme == 1)){
    tocaAlarme();
  }
}

void OneStep(bool dir){
  if(dir) {
    switch(step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }  
  } else {
    switch(step_number){
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
    } 
  }
  
  step_number++;

  if(step_number > 3){
    step_number = 0;
  }
}

void setMotor(){
  if(motor == 1){
    motor = 0;
    for(i = 0; i < 1024;i++){       
      OneStep(false);
      delay(2);
    }
  } else {
    motor = 1;
    for(i = 0; i < 1024;i++){
      OneStep(true);
      delay(2);
    }
  }
}

void setAC(){
  if(ac == 1){
    digitalWrite (AC, LOW);
    ac = 0;
    Serial.write('U');
  } else {
    digitalWrite (AC, HIGH);
    ac = 1;
    Serial.write('Y');
  }
}

void fechPorta(){
  meuServo.write(90);  // fecha porta
}

void campainha(){
  //Serial.println("Está alguém à porta!");
  digitalWrite(BUZZER,HIGH);
  delay(150);
  digitalWrite(BUZZER,LOW);
  delay(150);
  digitalWrite(BUZZER,HIGH);
  delay(150);
  digitalWrite(BUZZER,LOW);
  delay(150);
  digitalWrite(BUZZER,HIGH);
  delay(150);
  digitalWrite(BUZZER,LOW);
  delay(150);
}

void mudaCodigo()
{
	leButoes();
  	
  	int i;
  	for (i = 0; i < 4; i++)
    {
      if (codigoInserido[i] != codigoArmazenado[i])
      {
        break;
      }
    }

    if (i == 4)
    {  
      leButoes();
      escreveCodigo(codigoInserido);
  	} 
  	else 
    {
  	  //Serial.println("Invalido, Tenta Outra Vez!");
    }
}

void escreveCodigo(char codigo[])
{
  for(int i = 0; i < 4 ; i++)
  {
    codigoArmazenado[i] = codigo[i];
  }
}

void leButoes()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    codigoInserido[i] = '*';
  }
  
  char key;

  key = keypad.getKey();

  int cont = 0;

  while (key != 'C')
  {
    if((key != 'B') && (key != 'A') && (key != 'D') && (key != NO_KEY))
    {
      codigoInserido[cont] = key;
      cont++;
      
  	  //Serial.println(key);
    }
    
    key = keypad.getKey();
  }
}

void verificaCodigo(char codigo[]){
    int i;
    for (i = 0; i < 4; i++)
    {
      if (codigo[i] != codigoArmazenado[i])
      {
        break;
      }
    }

    if (i == 4)
    { 
      
  	  //Serial.println("abre porta");
      meuServo.write(0);  // abre porta
      // Serial.write('W');  // Verifica Lux Entrada esta no outro arduino
    } 
  	else 
    {
  	  //Serial.println("fecha porta");
    }
}