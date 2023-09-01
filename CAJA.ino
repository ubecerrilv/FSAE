#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include <ESP32Servo.h>
#include <analogWrite.h>

#include <EEPROM.h>

const char* ssid = "ESP-caja";
const char* password = "formula123";

AsyncWebServer server(80);

Servo servo1;
Servo aleron;
int velAct =0;

//ANGULOS DEL SERVO
int bajo=55;
int alto=125;

//ENTRADAS DE LOS BOTONES DE VALIDACION
int sb = 18; //PARA BAJAR
int ss = 19; //PARA SUBIR

//PEDAL DEL CLUCH
int c=21;

//PEDAL DEL FRENO
int f= 4;

//CONTADOR PARA VALIDAR BOTONES
bool si =true;

//VARIABLE PARA LOS LEDS
int led;

//VARIBLE DEL ALERON
  int en = 0;

void setup() {
  //BOTONES DE VALIDACION
  pinMode(sb, INPUT_PULLUP); // Boton bajo
  pinMode(ss, INPUT_PULLUP); // Botón subio

 //CONFIGURACIONES INICIALES DEL SERVO
  Serial.begin(115200);
  servo1.attach(15); 
  servo1.write(90);

  aleron.attach(2);//PENDIENTE

 //BOTON DE CLUCH
  pinMode(c, INPUT_PULLUP);

  //BOTON DE FRENO
  pinMode(f, INPUT_PULLUP);

 //MEMORIA EEPROM
 EEPROM.begin(8);
 velAct= EEPROM.read(0);

  imprimir();

 //PINES DE LOS LEDS
 pinMode(13, OUTPUT);//N
 pinMode(12, OUTPUT);//1
 pinMode(14, OUTPUT);//2 
 pinMode(27, OUTPUT);//3
 pinMode(26, OUTPUT);//4
 pinMode(25, OUTPUT);//5
 pinMode(33, OUTPUT);//6

  //SETUP PARA WIFI
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  //MANDAR VELOCIDAD ACTUAL AL VOLANTE(SETUP)
  server.on("/inicia", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", inicio().c_str());
   });

//RESPONSE DE LAS FUNCIONES DE LOS BOTONES DEL VOLANTE
//BAJAR
  server.on("/bajo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", bajar().c_str());
   });

//SUBIR
  server.on("/arriba", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", subir().c_str());
  });

//NEUTRAL
  server.on("/neutral", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", neutral().c_str());
  });

//ALERON
server.on("/freno", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", frenar().c_str());
  });

  server.begin();
}//FIN SETUP
void loop() {
  if(digitalRead(sb)==LOW){
    si=true;
    delay(300);
  }else if(digitalRead(ss)==LOW){
    si=true;
    delay(300);
  }
  imprimir();

  digitalWrite(led, HIGH);

  switch(velAct){
    case 0:
    led=13;
    break;

    case 1:
    led=12;
    break;

    case 2:
    led=14;
    break;

    case 3:
    led=27;
    break;

    case 4:
    led=26;
    break;

    case 5:
    led=25;
    break;

    case 6:
    led=33;
    break;
  }//FIN SWITCH

  if((digitalRead(f)==LOW) && en==1){
    aleron.write(45);
  }else if((digitalRead(f)==HIGH) && en==1){
    aleron.write(0);
  }

}//FIN LOOP

String inicio(){
  return (String)velAct;
}

String subir(){
  String res = "";
  if (0<=velAct && 5>=velAct && digitalRead(c)==LOW) { //AGREGAR CONDICION DE CLUCH (TODOS)
    if(velAct==1){
      servo1.write(bajo);
      delay(350);
      servo1.write(90);
     /* delay(450);
      servo1.write(bajo);
      delay(350);
      servo1.write(90);*/

      if(si){velAct++;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }else if(velAct==0){
      servo1.write(alto);
      delay(350);
      servo1.write(90);

      if(si){velAct++;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }else{
      servo1.write(bajo);
      delay(350);
      servo1.write(90);

      if(si){velAct++;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }
  }
  return res;
}//FIN SUBIR

/*
*FUNCION PARA BAJAR VELOCIDADES
*/
 String bajar(){
   String res = "";
  if (1<=velAct && 6>=velAct && digitalRead(c)==LOW){
    if(velAct==2){  
      servo1.write(alto);
      delay(350);
      servo1.write(90);
      /*delay(450);
      servo1.write(alto);
      delay(350);
      servo1.write(90);*/

      if(si){velAct--;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }else if (velAct == 1){
      servo1.write(73);
      delay(350);
      servo1.write(90);

      if(si){velAct--;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }else{    
      servo1.write(alto);
      delay(350);
      servo1.write(90);

      if(si){velAct--;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }
  } 
  return res;
}//FIN BAJAR

/*
*FUNCION PARA REGRESAR A NEUTRAL DESDE CUALQUIER VELOCIDAD
*/
String neutral(){
 String res = ""; 
  if (velAct != 0 && digitalRead(c)==LOW){
    if(velAct == 1){
      servo1.write(73);
      delay(450);
     servo1.write(90);

     if(si){velAct--;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
   }else if(velAct == 2){
     servo1.write(107);
     delay(450);
     servo1.write(90);

     if(si){velAct-=2;si=true;}//CAMBIO TEMPORAL
      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
    }else{
      int aux = velAct-1;
      for(int i =0;i<aux;i++){
       servo1.write(alto);
       delay(450);
       servo1.write(90);
       delay(350);
       if(si){velAct--;si=true;}//CAMBIO TEMPORAL
      }
      if(velAct==1){
        velAct--;
      }

      res = (String)velAct;

      EEPROM.write(0, velAct);
      EEPROM.commit();

      return res;
   }
  }
  return res;
}//FIN NEUTRAL

/*
* FUNCION PARA DESPLEGAR ALERON
*/
String frenar(){
  if(en == 0){
    en=1;
    return "1";
  }else {
    en=0;
    return "0";
  }
}//FIN FRENAR

void imprimir(){
  Serial.print("Velocidad actual = ");
  Serial.println(velAct);
  delay(100);
}