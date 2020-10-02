//Fire Alarm System Using NodeMCU
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

BlynkTimer timer;

String apiKey = "LKMS7PX3P1R1A7RU";
char auth[] = "fisHLL9_Sukyj7-0SfPc0LCBd4YqKHuw"; //Auth code sent via Email
char ssid[] = "Pranavkumar"; //Wifi name
char pass[] = "pk197443kk";  //Wifi Password

int flag=0;
const int flame = 5;
const int gas = A0;
const int buzz = 4;
const char* server = "api.thingspeak.com";

#define DHTPIN 0          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
WiFiClient client;

void notifyOnFire()
{
  int isButtonPressed = digitalRead(flame);
  int gasinput = analogRead(gas);
  Blynk.virtualWrite(V3,gasinput);
  while((isButtonPressed==0)  && (flag ==0) && (gasinput>=200) ) 
  {
    Serial.println("Fire in the House");
    Blynk.notify("Alert : Fire in the House");
    digitalWrite(buzz,HIGH);
    tone(buzz,1000);
    delay(1000);
    noTone(buzz);
    delay(1000);
    flag =1;
    
  }
  if (isButtonPressed==0)
  {
    digitalWrite(buzz,LOW);
    flag=0;
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
      
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

   if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
   {                       
     String postStr = apiKey;
     postStr +="&field1=";
     postStr += String(t);
     postStr +="&field2=";
     postStr += String(h);
     postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius, Humidity: ");
     Serial.print(h);
     Serial.println("%. Send to Thingspeak.");
   }
   
  client.stop();
  Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(buzz,OUTPUT);
  Blynk.begin(auth, ssid, pass);
  pinMode(flame,INPUT_PULLUP);
  pinMode(gas,INPUT);
  timer.setInterval(1000L,notifyOnFire); 
}

void loop()
{
  Blynk.run();
  timer.run();
}
