/* Adafruit IO Example Using WiFiManager
 *
 * This is a simple Adafruit feed subscribe example that uses
 * WiFiManager to handle setup of WiFi credentials and connecting
 * to the network instead of defining the WiFI SSID and password
 * explicitly in the code.
 *
 * To use this example, add your Adafruit IO Username and Key
 * and setup a feed called "myfeed".  When you manually add data
 * to the feed on io.adafruit.com, you'll see that data written to
 * the serial output.
 *
 * Brad Black - 2022
 *
 */

#include <WiFiManager.h>
#include "AdafruitIO_WiFi.h"

#include <FirebaseESP32.h> //Incluye la librería para comunicarse con Firebase

#define FIREBASE_HOST "db-tdg-redmeteo-default-rtdb.firebaseio.com" // Establece la URL del proyecto de Firebase
#define FIREBASE_AUTH "UUvr5k1bH0kfwuFA7HspfBPmRXUOhlywCpgHPKp0" // Establece el token de autenticación de Firebase

#define IO_USERNAME  "ElianC"
#define IO_KEY       "aio_gcsX07g1bH8PimxVWfaAX2pKcPIv"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "", "");
//AdafruitIO_Feed *humedad = io.feed("estacio-2.humedad");
//AdafruitIO_Feed *Temperatura = io.feed("estacio-2.temperatura");
AdafruitIO_Feed *temperatura = io.feed("temperatura");//FEED O VARAIBLES
AdafruitIO_Feed *humedad = io.feed("humedad");
AdafruitIO_Feed *co2 = io.feed("co2");
AdafruitIO_Feed *pm1 = io.feed("pm1");
AdafruitIO_Feed *pm10 = io.feed("pm10");
AdafruitIO_Feed *pm25 = io.feed("pm2-5");
AdafruitIO_Feed *uv = io.feed("uv");
AdafruitIO_Feed *bateria = io.feed("bateria");
AdafruitIO_Feed *ubi = io.feed("ubicacion-e-princi");

float hum=0;
float tem=0;
float PM1=0;
float PM25=0;
float PM10=0;
float UV=0;
unsigned int Co2=0;
unsigned int Bateria=0;

String rute1= "Datos"; //Carpeta donde se guardaran los datos en Firebase
WiFiManager wifiManager;
FirebaseData firebaseData; //Crea una inst
void handleMessage(AdafruitIO_Data *data)
{

  Serial.print("received <- ");
  Serial.println(data->toString());

} // handleMessage

void setup()

{
  Serial.begin(115200); // Initialize serial port for debugging.
  delay(500);

  // wifiManager.resetSettings();  //uncomment to reset the WiFi settings

  wifiManager.setClass("invert");          // enable "dark mode" for the config portal
  wifiManager.setConfigPortalTimeout(500); // auto close configportal after n seconds
  wifiManager.setAPClientCheck(true);      // avoid timeout if client connected to softap

  if (!wifiManager.autoConnect("WiFi_Simulada"))   // connect to wifi with existing setting or start config
  {
    Serial.println("failed to connect and hit timeout");
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("Connected to WiFi.");

    Serial.printf("Connecting to Adafruit IO with User: %s Key: %s.\n", IO_USERNAME, IO_KEY);
    io.connect();
     Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Inicia la conexión con Firebase
/*
    myfeed->onMessage(handleMessage);

    myfeed->get();

    // wait for a connection
*/
    while ((io.status() < AIO_CONNECTED))
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println("Connected to Adafruit IO.");
  }

} // setup()

void loop()
{

  io.run();
  Serial.print("Humedad: ");
  Serial.println(hum);
  Serial.print("temperatura: ");
  Serial.println(tem);
  temperatura->save(tem);
  humedad->save(hum,7,-73);
  co2->save(Co2);
  pm1->save(PM1);
  pm10->save(PM10);
  pm25->save(PM25);
  uv->save(UV);
  bateria->save(Bateria);
  ubi->save("Estacion Meteo AirWatch",7.1,-73.1);
  float latid=(float) Firebase.getString(firebaseData,rute1+"/Latitud");
   float  longi=(float) Firebase.getString(firebaseData,rute1+"/Longitud");
    Serial.print(latid);
    Serial.print(", ");
    Serial.println(longi);
  hum=random(0,100);
  tem=random(0,100);
  Co2=random(0,100);
  PM1=random(0,30);
  PM10=random(0,100);
  PM25=random(0,70);
  UV=random(0,10);
  Bateria=random(0,100);
  delay(60000);
} // loop()
