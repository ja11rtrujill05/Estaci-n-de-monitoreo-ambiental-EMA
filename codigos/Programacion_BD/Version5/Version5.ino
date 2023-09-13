//---------Archivo de configuracion-----------//
#include "confi.h"

//-----------COORDENADAS-------------------//

float latid;
float longi;

//--------------Puertos,dirreciones y oled------------//

#define rx 3 //Sensor PM - Cable Verde
#define tx 2 //Sensor PM - Cable Amarillo
#define TXC 9 //Sensr Co2
#define RXC 10 // Sensor Co2
#define VEML6075_ADDR   0x10
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

//----------objetos de liberias--------//
SoftwareSerial mySerial(RXC, TXC); //El puerto serial que va a permitir la comunicacion entre el ordenador el sensor de CO2 estará fijando 
SoftwareSerial ZHSerial(rx, tx); // RX, TX
SD_ZH03B ZH03B( ZHSerial, SD_ZH03B::SENSOR_ZH03B );  // same as the line above
HTU21D myHumidity; // create object
DFRobot_VEML6075_IIC VEML6075(&Wire, VEML6075_ADDR);  // create object
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MHZ19 myMHZ19;
WiFiManager wm; //Crea una instancia de la clase WiFiManager para manejar la conexión Wi-Fi
FirebaseData firebaseData; //Crea una instancia de la clase FirebaseData para manejar la conexión con Firebase

//----------------Variables-----------------//
int PM1;
int PM25;
int PM10;
int bat=0;
unsigned int PM=0;
String N="";
float humd;
float Temp;
static int CO2=0; //variable CO2=0
/*
  uint16_t    UvaRaw;        // read UVA raw
  uint16_t    UvbRaw;        // read UVB raw
  uint16_t    comp1Raw; // read COMP1 raw
  uint16_t    comp2Raw;   // read COMP2 raw
*/
float Uva;                   // get UVA
float Uvb;                   // get UVB
float Uvi;            // get UV index
String rute1= "Datos"; //Carpeta donde se guardaran los datos en Firebase
//-----------------Logos------------------//

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


//----------------------Inicio Setup----------------------//

void setup() {
  Serial.begin(115200); //Inicia la comunicación serial a 115200 baudios
  
  //-------Comunicacion Sensor PM-----//
  
  ZHSerial.begin(9600);                                                                                
  ZH03B.setMode( SD_ZH03B::IU_MODE );
  
  //----------OLED-------------------//
  
  delay(100); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  //display.setContrast (0); // dim display
  display.display();
  delay(500);
  
  //-----Muestra Logo ERA ----------//
  
  display.clearDisplay();
  display.drawBitmap(0,0, LOGObmp, 128, 64, SH110X_WHITE);
  display.display();
  delay(500);
  display.clearDisplay();
  
  //---------Configuracion WIFI-----------//
  
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
     res = wm.autoConnect("ESP32C3_WifiConfi"); // anonymous ap
  // res = wm.autoConnect("ESP32_Confi","password"); // password protected ap
      if(!res) {
        Serial.println("Failed to connect");
        //ESP.restart();
      }//Fin if 
      else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
      }//Fin else
    
  //--------Configuracion FireBase----------//

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Inicia la conexión con Firebase

  //------------Configuracion CO2----------//
  
  mySerial.begin(9600);
  myMHZ19.begin(mySerial);
  myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))

  //-------Configuracion Temperatura y Humedad-------//
  
  while(!Serial);
  myHumidity.begin();
  Serial.println();

  //------------Configuracion UV---------------//
  
  while(VEML6075.begin() != true) {
    Serial.println("VEML6075 begin faild");
    delay(500);
  }
  Serial.println("VEML6075 begin successed");

  //--------Configuracion Adafruid IO---------//
  
  Serial.printf("Connecting to Adafruit IO with User: %s Key: %s.\n", IO_USERNAME, IO_KEY);
  io.connect();
  while ((io.status() < AIO_CONNECTED))
    {
      Serial.print(".");
      delay(500);
    }
   Serial.println("Connected to Adafruit IO.");
   
}//---------Fin Setup-----------//

//------------Inicio LOOP----------//

void loop() {
  /*
  UvaRaw = VEML6075.readUvaRaw();         // read UVA raw
  UvbRaw = VEML6075.readUvbRaw();         // read UVB raw
  comp1Raw = VEML6075.readUvComp1Raw();   // read COMP1 raw
  comp2Raw = VEML6075.readUvComp2Raw();   // read COMP2 raw
*/
    //-----------Obtencion Data Co2-------//
    
    CO2 = myMHZ19.getCO2(); //Toma el Valor de ppm
    
    //-----------Obtencion Data PM-------//
  
    ZH03B.readData();
    PM1=ZH03B.getPM1_0();
    PM25=ZH03B.getPM2_5();
    PM10=ZH03B.getPM10_0();

    //--------------Obtencion UV-----------//
  
    Uva = VEML6075.getUva();                    // get UVA
    Uvb = VEML6075.getUvb();                    // get UVB
    Uvi = VEML6075.getUvi(Uva, Uvb);            // get UV index

    //Obtencion Temperatura y Humedad--------//
  
    humd = myHumidity.readHumidity();
    Temp = myHumidity.readTemperature();

    //----------Envio de Datos FireBase------------//
  
      N=String(humd,2); //se usa para enviar Cadenas en vez de float
    Firebase.setString(firebaseData,rute1+"/humedad",N); //Humedad
    Firebase.setInt(firebaseData,rute1+"/PM1", PM1); //Revisar PM 1
    Firebase.setInt(firebaseData,rute1+"/PM2_5", PM25); //Revisar PM 2.5
    Firebase.setInt(firebaseData,rute1+"/PM10", PM10); //Revisar PM 10
    Firebase.setInt(firebaseData,rute1+"/CO2",CO2); //CO2
      N=String(Uvi,2);
    Firebase.setString(firebaseData,rute1+  "/UV",N); // "Rediacion UV"
      N=String(Temp,2);
    Firebase.setString(firebaseData,rute1+  "/temperatura",N);// "temperatura"
    
    //----------Toma los Datos de Coordenadas-------//

    latid=(float) Firebase.getString(firebaseData,rute1+"/Latitud");
    longi=(float) Firebase.getString(firebaseData,rute1+"/Longitud");
    
    //---------Envio de Datos Adafruid IO-------//
    
    temperatura->save(Temp);
    humedad->save(humd);
    co2->save(CO2);
    pm1->save(PM1);
    pm10->save(PM10);
    pm25->save(PM25);
    uv->save(Uvi);
    bateria->save(bat);
    ubi->save("Estacion Meteo AirWatch",latid,longi);
    bat=random(0,100);

    //---------Imprimir en OLED-----------------//
    
    display.clearDisplay();
    testdrawchar();

    //--------------Tiempo de envio de datos------------//
    
    delay(60000);
    
}//----------Fin LOOP------------------//


// Funciones

void testdrawchar() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.write("Co2:");
  display.print(CO2);
  display.println();
  display.write("PM1:");
  display.print(PM1);
  display.println();
  display.write("PM2.5:");
  display.print(PM25);
  display.println();
  display.write("PM10:");
  display.print(PM10);
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
  display.write("Ubi:");
  display.print(latid,2);
  display.print(",");
  display.print(longi,2);
  display.display();
  delay(1);
}
