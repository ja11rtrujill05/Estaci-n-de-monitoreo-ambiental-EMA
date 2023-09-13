#ifndef confi_h
#define confi_h
//--------- Incluir librerias----------//

// LIBRERIAS
#include <SoftwareSerial.h> //Crear Serial por Software
#include <WiFiManager.h> //Incluye la librería para manejar la conexión Wi-Fi
#include <FirebaseESP32.h> //Incluye la librería para comunicarse con Firebase
#include <MHZ19.h> //Libreria para el CO2
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //Adafruit IO
#include <Adafruit_SH110X.h> //OLED
#include <DFRobot_VEML6075.h> //UV
#include "SparkFunHTU21D.h" //temperatura y humedad
#include <SD_ZH03B.h> //PM
#include "AdafruitIO_WiFi.h" //Base de datos Adafruit

//-----------ADAFRUIT IO----------------//

#define IO_USERNAME  "ElianC" // Usuario IO
#define IO_KEY       "aio_gcsX07g1bH8PimxVWfaAX2pKcPIv" //KEY de acceso
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "", ""); //CONEXCION A ADAFRUIT
AdafruitIO_Feed *temperatura = io.feed("temperatura");//FEED O VARAIBLES
AdafruitIO_Feed *humedad = io.feed("humedad");
AdafruitIO_Feed *co2 = io.feed("co2");
AdafruitIO_Feed *pm1 = io.feed("pm1");
AdafruitIO_Feed *pm10 = io.feed("pm10");
AdafruitIO_Feed *pm25 = io.feed("pm2-5");
AdafruitIO_Feed *uv = io.feed("uv");
AdafruitIO_Feed *bateria = io.feed("bateria");
AdafruitIO_Feed *ubi = io.feed("ubicacion-e-princi");

//--------------FireBase/RealTimeDataBase-----------//

#define FIREBASE_HOST "db-tdg-redmeteo-default-rtdb.firebaseio.com" // Establece la URL del proyecto de Firebase
#define FIREBASE_AUTH "UUvr5k1bH0kfwuFA7HspfBPmRXUOhlywCpgHPKp0" // Establece el token de autenticación de Firebase

#endif


  
