#include "WiFi.h"
#include "HTTPClient.h"

#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>
#include <SPI.h>

//Pines para pantalla
  #define TFT_CS         15  //case
  #define TFT_RST        19 //reset
  #define TFT_DC         16 //AO 
  #define TFT_MOSI       17 //Data = SDA 
  #define TFT_SCLK       18 //Clock = SCK 

 
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char* ssid = "ESP-caja";
const char* password = "formula123";

String velAct;

const char* serverNameInicia = "http://192.168.4.1/inicia";
const char* serverNameBaja = "http://192.168.4.1/bajo";
const char* serverNameArriba = "http://192.168.4.1/arriba";
const char* serverNameNeutral = "http://192.168.4.1/neutral";
const char* serverNameFreno = "http://192.168.4.1/freno";

//VARIABLES PARA DEFINIR CICLOS
const long intervalo = 2000;
unsigned long milisPrevios = 0;

int i = 0;

int en = 0;
String confA ="0";

void setup() {
  Serial.begin(115200);
  pinMode(27, INPUT_PULLUP); // Boton bajar
  pinMode(33, INPUT_PULLUP); // Botón subir
  pinMode(26, INPUT_PULLUP); // Boton neutral
  pinMode(32, INPUT_PULLUP); // Boton freno

  //SETUP DE LA PANTALLA
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
 //SETEAR EL VALOR DE LA VELOCIDAD (OBTENER DE EPROM)
  tft.setTextSize(3);
  tft.setCursor(70, 45);
  tft.print("N");

 //SETEAR LOGOTIPOS
  tft.setTextSize(2);
  tft.setCursor(25, 75);
  tft.println("FI-UAEMex");
  tft.setCursor(20, 90);
  tft.print("POTROSPORT");
  

 //SETUP PARA WIFI
  WiFi.begin(ssid, password);

  iniciar();
  aleronP();
}
 
void loop() {
  if(WiFi.status()== WL_CONNECTED){
    for(i; i<1;i++){
      iniciar();
    }
    if(digitalRead(27)==LOW){
      flechaBaja();
      delay(250);
      btn1();
   }else if(digitalRead(33)==LOW){
     flechaSube();
     delay(250);
     btn2();
   }else if(digitalRead(26)==LOW){
     flechaBaja();
     delay(250);
     btn3();
   }else if(digitalRead(32)==LOW){
     delay(250);
     btn4();
   }
  }

  unsigned long milisActuales = millis();
  if(milisActuales - milisPrevios >= intervalo){
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    if(WiFi.status()== WL_CONNECTED){
      tft.fillRect(20,115,100,15, ST77XX_BLACK);
      tft.setCursor(20, 115);
      tft.print("CONECTADO");
  }else {
     tft.fillRect(20,115,100,15, ST77XX_BLACK);
     tft.setCursor(20, 115);
     tft.print("DESCONECTADO...");
  }
  milisPrevios = milisActuales;
  }

  if(en == 0 && (digitalRead(32)==LOW)){
    delay(150);
    en=1;
    aleronP();
  }else if(en == 1 && (digitalRead(32)==LOW)){
    delay(150);
    en=0;
    aleronP();
  }

}//FIN LOOP

String httpGETRequest(const char* serverName){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int respuestaHTTP = http.GET();

  String respuesta ="";

  if (respuestaHTTP>0){
    respuesta = http.getString();
  }else{
    respuesta="0";
  }
  http.end();

  return respuesta;
}

void iniciar(){
  velAct=httpGETRequest(serverNameInicia);
  Serial.print(velAct);
  cambiarVelP();
}

void btn1(){
  velAct = httpGETRequest(serverNameBaja);
  Serial.println(velAct);
  cambiarVelP();
}

void btn2(){
  velAct = httpGETRequest(serverNameArriba);
  Serial.println(velAct);
  cambiarVelP();
}

void btn3(){
  velAct = httpGETRequest(serverNameNeutral);
  Serial.println(velAct);
  cambiarVelP();
}

void btn4(){
  confA = httpGETRequest(serverNameFreno);
  en = confA.toInt();
  aleronP();
}

void cambiarVelP(){
  tft.fillRect(5,5,70,20, ST77XX_BLACK);
  tft.setTextSize(3);

  switch(velAct.toInt()){
    case 0:
      if(velAct != ""){
        tft.fillRect(70,45,50,30, ST77XX_BLACK);
        tft.setCursor(70, 45);
        tft.print("N");
      }
    break;

    case 1:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("1");
      tft.setTextSize(1);
      tft.setCursor(85, 45);
      tft.print("era");
    break;

    case 2:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("2");
      tft.setTextSize(1);
      tft.setCursor(90, 45);
      tft.print("da");
    break;

    case 3:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("3");
      tft.setTextSize(1);
      tft.setCursor(90, 45);
      tft.print("era");
    break;

    case 4:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("4");
      tft.setTextSize(1);
      tft.setCursor(85, 45);
      tft.print("ta");
    break;

    case 5:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("5");
      tft.setTextSize(1);
      tft.setCursor(90, 45);
      tft.print("ta");
    break;

    case 6:
      tft.fillRect(70,45,50,30, ST77XX_BLACK);
      tft.setCursor(70, 45);
      tft.print("6");
      tft.setTextSize(1);
      tft.setCursor(90, 45);
      tft.print("ta");
    break;
  }
}//FIN CAMBIAR VELP

//FUNCION PARA PINTAR EN PANTALLA FLECHA DE BAJA DE VELOCIDADES
void flechaBaja(){
  tft.setTextSize(1);
  tft.setCursor(5, 5);
  tft.print("Bajando...");
}//FIN FLECHA BAJA

//FUNCION PARA PINTAR EN PANTALLA FLECHA DE SUBIDA DE VELOCIDADES
void flechaSube(){
  tft.setTextSize(1);
  tft.setCursor(5, 5);
  tft.print("Subiendo...");
}

//FUNCION PARA PINTAR ENCEDIDO/APAGADO DEL ALERON
void aleronP(){
  tft.setCursor(90, 5);
  tft.setTextSize(1);
  
  if(confA =="1"){
    tft.setTextColor(ST77XX_GREEN);
    tft.println("Aleron");
  }else if(confA =="0"){
    tft.setTextColor(ST77XX_RED);
    tft.println("Aleron");
  }
  delay(150);
}