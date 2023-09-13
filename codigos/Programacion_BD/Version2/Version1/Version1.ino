// LIBRERIAS
#include <SoftwareSerial.h>
#include <WiFiManager.h> //Incluye la librería para manejar la conexión Wi-Fi
#include <FirebaseESP32.h> //Incluye la librería para comunicarse con Firebase
#include <MHZ19.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DFRobot_VEML6075.h>
#include "SparkFunHTU21D.h"

//#define RX_PIN D0
//#define TX_PIN D1

#define VEML6075_ADDR   0x10
//Create an instance of the object
HTU21D myHumidity;

DFRobot_VEML6075_IIC VEML6075(&Wire, VEML6075_ADDR);  // create object

// OLED

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM LOGObmp[] =
{// 'fondo ERA', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x87, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0xff, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0xff, 0xff, 0xf8, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x14, 0x0f, 0xfd, 0xff, 0x80, 0x78, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x17, 0x8f, 0xf9, 0xff, 0xc0, 0xf8, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1c, 0xfe, 0x03, 0x83, 0xc0, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0x81, 0xc1, 0xfc, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x06, 0x1c, 0x03, 0x83, 0xc3, 0xdc, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 0xf3, 0x87, 0x83, 0x9c, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0b, 0x3f, 0xe7, 0xff, 0x07, 0x1c, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x06, 0x3f, 0xe7, 0xfe, 0x0f, 0x1e, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x07, 0x00, 0x0e, 0x1e, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x0f, 0x3c, 0x1d, 0xee, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x73, 0x78, 0x0e, 0x1c, 0x3b, 0xee, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x9e, 0x70, 0x0e, 0x1e, 0x78, 0x0e, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0xee, 0x0e, 0x70, 0x0e, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xde, 0x0f, 0xe0, 0x0f, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x03, 0xfc, 0x03, 0xf7, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xef, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe1, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// CO2

MHZ19 myMHZ19;
//SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial

// Base de Datos

// Establece la URL del proyecto de Firebase

  #define FIREBASE_HOST "db-tdg-redmeteo-default-rtdb.firebaseio.com"

// Establece el token de autenticación de Firebase
  #define FIREBASE_AUTH "UUvr5k1bH0kfwuFA7HspfBPmRXUOhlywCpgHPKp0" 

// Variales Globales 
unsigned int PM=0;
//float n=0;
String N="";
//unsigned int CO2=0;
//float Temp=0;

  float humd;
  float Temp;
/*
  uint16_t    UvaRaw;        // read UVA raw
  uint16_t    UvbRaw;        // read UVB raw
  uint16_t    comp1Raw; // read COMP1 raw
  uint16_t    comp2Raw;   // read COMP2 raw
*/
  float       Uva;                   // get UVA
  float       Uvb;                   // get UVB
  float       Uvi;            // get UV index

// WIFI MANAGER

WiFiManager wm; //Crea una instancia de la clase WiFiManager para manejar la conexión Wi-Fi
//Crea una instancia de la clase FirebaseData para manejar la conexión con Firebase
FirebaseData firebaseData; 
String rute1= "Datos"; //Carpeta donde se guardaran los datos en Firebase


void setup() {
  Serial.begin(115200); //Inicia la comunicación serial a 115200 baudios
 // Serial1.begin(9600);
  
// Configuracion OLED
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  delay(500);

  // Clear the buffer.
  display.clearDisplay();
  display.drawBitmap(0,0, LOGObmp, 128, 64, SH110X_WHITE);
  display.display();
  delay(500);
  display.clearDisplay();
  
// Configuracion WIFI
  
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
     res = wm.autoConnect("ESP32C3_WifiConfi"); // anonymous ap
  // res = wm.autoConnect("ESP32_Confi","password"); // password protected ap
  if(!res) {
    Serial.println("Failed to connect");
    //ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
    }
    
// Configuracion Base de Datos

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Inicia la conexión con Firebase

// Configuracion CO2
  //mySerial.begin(9600);
  //myMHZ19.begin(Serial1);
  //myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))

  while(!Serial);
  myHumidity.begin();
  Serial.println();
  while(VEML6075.begin() != true) {
    Serial.println("VEML6075 begin faild");
    delay(2000);
  }
  Serial.println("VEML6075 begin successed");
  
}
void loop() {
//    N=String(n,2);
  /*
  UvaRaw = VEML6075.readUvaRaw();         // read UVA raw
  UvbRaw = VEML6075.readUvbRaw();         // read UVB raw
  comp1Raw = VEML6075.readUvComp1Raw();   // read COMP1 raw
  comp2Raw = VEML6075.readUvComp2Raw();   // read COMP2 raw
*/
  Uva = VEML6075.getUva();                    // get UVA
  Uvb = VEML6075.getUvb();                    // get UVB
  Uvi = VEML6075.getUvi(Uva, Uvb);            // get UV index
  humd = myHumidity.readHumidity();
  Temp = myHumidity.readTemperature();
  
    N=String(humd,2);
    Firebase.setString(firebaseData,rute1+"/humedad",humd); //Escribe un valor entero en Firebase con la clave "humedad"
    Firebase.setInt(firebaseData,rute1+"/PM1",PM++); //Escribe un valor entero en Firebase con la clave "PM_Simulado"
    Firebase.setInt(firebaseData,rute1+"/CO2",PM++); //Escribe un valor entero en Firebase con la clave "CO2"
    N=String(Uvi,2);
    Firebase.setString(firebaseData,rute1+  "/UV",N); //Escribe un valor entero en Firebase con la clave "Rediacion UV"
     N=String(Temp,2);
    Firebase.setString(firebaseData,rute1+  "/temperatura",Temp); //Escribe un valor entero en Firebase con la clave "temperatura"
    display.clearDisplay();
    testdrawchar();
    delay(10000); //Espera 5 segundos antes de volver a escribir en Firebase
}


// Funciones

void testdrawchar() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.write("Co2:");
  display.print(PM);
  display.println();
  display.write("PM1:");
  display.print(PM);
  display.println();
  display.write("PM2.5:");
  display.print(PM);
  display.println();
  display.write("PM10:");
  display.print(PM);
  display.println();
  display.write("Tem:");
  display.print(Temp);
  display.println();
  display.write("Hum:");
  display.print(humd);
  display.println();
  display.write("UV:");
  display.print(Uvi);
  display.println();
  display.write("Otro:");
  display.print(PM);
  display.display();
  delay(1);
}
