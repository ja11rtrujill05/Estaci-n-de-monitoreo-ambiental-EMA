//Código sensor C02 2BachB 
//Lucía Jiménez Pérez 

//Librerias:
#include <Wire.h> 
#include <MHZ19.h>   // Sensor CO2                                     
#include <SoftwareSerial.h> // la libreria controla el puerto serial entre el módulo de co2 y el ordenador 


//Definimos los pines 
#define RX_PIN 18  //Pines del sensor CO2                                        
#define TX_PIN 19  // Pines del sensor CO2                                        
#define BAUDRATE 9600  //Frecuencia a la que se va a abrir el puerto serial                                    


MHZ19 myMHZ19;  //Definimos el nombre del sensor de co2                                           
SoftwareSerial mySerial(RX_PIN, TX_PIN); //El puerto serial que va a permitir la comunicacion entre el ordenador el sensor de CO2 estará fijando 
//en los pines RX_PIN, TX_PIN

//Lucía Jiménez
void setup() {
  
Serial.begin(9600); //Empieza el serial
  mySerial.begin(BAUDRATE);    // empieza el serial  de comunicación                             
  myMHZ19.begin(mySerial);      //inicia el sensor de CO2                           

  myMHZ19.autoCalibration(); //Autocalibración del sensor                             

}

void loop() { //Ciclo
 
static int CO2=0; //variable CO2=0
CO2 = myMHZ19.getCO2();   //Para obtener los datos de CO2
Serial.print("CO2 (ppm): "); // Puerto serial del ordenador , para que aparezca el texto
Serial.println(CO2); // Aparecen las mediciones de CO2 en el puerto serial
delay(1000); //Espera un 1 segundo
}
