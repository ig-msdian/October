#include<LiquidCrystal.h>

LiquidCrystal lcd(2,3,5,6,7,8); //RS=2; EN=3; d4=5; d5=6; d6=7; d7=8;

const int trigPin1=13;
const int echoPin1=12;
const int trigPin2=11;
const int echoPin2=10;

const int led=9;

const int IRsensor=4;

unsigned long timesincelastreset=0;
unsigned long flag=0;

int counter1=0;
int counter2=0;
int counter=0;
int IRcounter=0;

int interval_1=3000;
int interval_2=3000;
int interval_ir=2000;

long microsecondsToCentimeters(long microseconds) 
{
   return microseconds / 29 / 2;
}

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(led,OUTPUT);
  pinMode(IRsensor,INPUT);
}

void loop()
{
  long duration11, distance11, duration12, distance12;
  int IRstatus1=digitalRead(IRsensor);
  int IRstatus2=digitalRead(IRsensor);
  if(IRstatus1==0)
  {
    int t=0;
    flag=millis();
    while(t==0 && (millis()-flag)<interval_ir)
    {
      if(IRstatus==1)
      {
        IRcounter=(IRcounter+1)%2;
      }
    }
  }
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration11=pulseIn(echoPin1, HIGH);
  distance11=microsecondsToCentimeters(duration11);
//  Serial.print("Distance11 ");
//  Serial.print(distance11);
//  Serial.println( );
  if(distance11<50 && distance11>0)
  {
    int t=0;
    timesincelastreset=millis();
    while(t==0 && (millis()-timesincelastreset)<interval_1)
    {
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);
      duration12=pulseIn(echoPin2, HIGH);
      distance12=microsecondsToCentimeters(duration12);
      if(distance12<50 && distance12>0)
      {
        counter1++;
        Serial.print("Counter1 = ");
        Serial.println(counter1);
        t=2000;
        delay(1000);
      }
    }
    counter=counter1-counter2;
  }

  int duration21, distance21, duration22, distance22;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration21=pulseIn(echoPin2, HIGH);
  distance21=microsecondsToCentimeters(duration21);
  if(distance21<50 && distance21>0)
  {
    int t=0;
    timesincelastreset=millis();
    while(t==0 && (millis()-timesincelastreset)<interval_2)
    {
      digitalWrite(trigPin1, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration22=pulseIn(echoPin1, HIGH);
      distance22=microsecondsToCentimeters(duration22);
      if(distance22<50 && distance22>0)
      {
        if(counter2==counter1)
        {
          counter1=0;
          counter2=0;
          break;
        }
        counter2++;
        Serial.print("Counter2 = ");
        Serial.println(counter2);
        t=2000;
        delay(1000);
      }
    }
    counter=counter1-counter2;
  }
  if( counter==0 || (counter>0 && IRcounter==1) )
  {
    digitalWrite(led,LOW);
    Serial.println("Lights Off");
    IRcounter=0;
  }
  else if(counter>0 && IRcounter==0)
  {
    digitalWrite(led,HIGH);
    Serial.println("Lights On");
  }
  lcd.setCursor(0,0);
  lcd.print("People In: ");
  lcd.print(counter);
  lcd.setCursor(0,1);
  lcd.print("Light Status:");
  if(counter>0 && IRcounter==0){
    lcd.print("ON");
  }
  else if(counter==0 || (counter>0 && IRcounter==1)){
    lcd.print("OFF");
  }
  delay(300);
}
