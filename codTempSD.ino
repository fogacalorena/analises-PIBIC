#include <OneWire.h>                
#include <DallasTemperature.h>
#include <SD.h>
#include <RTClib.h>

OneWire ourWire1(6); //pino 6 dig
OneWire ourWire2(2); //pino 2 dig
 
DallasTemperature sensors1(&ourWire1); //variavel sensor1
DallasTemperature sensors2(&ourWire2); //variavel sensor2

//cartao SD
int Pino_CS = 10;
RTC_DS1307 rtc;
File file;

void setup()
{
  Serial.begin(9600);
  sensors1.begin();   //inicia o sensor 1
  sensors2.begin();   //inicia o sensor 2
  
  //Inicia o cartao SD
  Serial.println("Iniciando cartao SD...");
  if (!SD.begin(Pino_CS))
  {
    Serial.println("Falha na inicializacao do SD!");
    return;
  }
  Serial.println("Cartao SD iniciado. OK");
  Serial.println();

  //Verifica as condicoes do RTC
  if (! rtc.begin())
  {
    Serial.println("RTC nao encontrado!");
    while (1);
  }
  if (! rtc.isrunning())
  {
    Serial.println("RTC nao operante!");
    
    //A linha abaixo ajusta o RTC com a data e hora do momento da compilacao
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //A linha abaixo permite ajustar manualmente a data e hora do RTC
    //Formato: DateTime(ano, mes, dia, hora, minuto, segundo)
    //rtc.adjust(DateTime(2020, 05, 16, 17, 00));
  }
}

void loop(void)
{
 //Le as informacoes da temperatura
 sensors1.requestTemperatures();   //manda ler temp1
 float temp1= sensors1.getTempCByIndex(0); //transforma temperatura em ºC

 sensors2.requestTemperatures();   //manda ler temp2
 float temp2= sensors2.getTempCByIndex(0); //transforma temperatura em ºC 
 
 //Le as informacoes do RTC
 DateTime now = rtc.now();  
    
 //Imprime na serial
  Serial.print(now.hour() < 10 ? "0" : "");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute() < 10 ? "0" : "");
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second() < 10 ? "0" : "");
  Serial.print(now.second(), DEC);
  Serial.print(" ");
 Serial.print("Temperatura 1 = ");
 Serial.print(temp1);
 Serial.print(" C");
 Serial.print("   Temperatura 2 = ");
 Serial.print(temp2);
 Serial.println(" C");
 
  
 
 //Abre arquivo no SD para gravacao
 file = SD.open("dados.csv", FILE_WRITE);
  
 //Grava os dados no cartao SD
 
  if ((now.hour()) < 10)
  {
    file.print("0");
  }
  file.print(now.hour());
  file.print(':');
  
  if ((now.minute()) < 10)
  {
    file.print("0");
  }
  file.print(now.minute());
  file.print(':');
  
  if ((now.second()) < 10)
  {
    file.print("0");
  }
  file.print(now.second());
  file.print(",");
  
  file.print(temp1);
  file.print(",");
  file.print(temp2);
  file.println();
  
  //Fecha arquivo
  file.close();
  
  //Delay ate a proxima leitura
  delay(60000);

  /*1 minuto = delay 60000*/
}
