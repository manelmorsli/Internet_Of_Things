#include <DHT.h>
#include <Servo.h>
#include <Stepper.h>


#define MQTT_SERVER "nom_serveur" // Remplacer par l'adresse de votre serveur MQTT
#define MQTT_PORT 1883
#define MQTT_USERNAME "mosquitto"
#define MQTT_PASSWORD "mosquitto"

//Declaration des DHTs 
#define DHTPIN_Air 2
#define DHTPIN_SOL 4 
#define DHTTYPE DHT11


DHT dht_Air(DHTPIN_Air, DHTTYPE);
DHT dht_Sol(DHTPIN_SOL, DHTTYPE);



//Déclaration detecteur d'insectes (motion/souns)
int pirState = LOW;
const int soundSensorPin = A0;
bool Bugs = false;

//Déclaration du photoresistor
const int photoresistorPin = A12;

//Déclartion pour l'ouverture du doom
Servo Doom;    //servo moteur
int position = 0; //pos initiale
bool doom = true ;
  

//Déclaration pour l'irrigation
int NbPas = 2048;
Stepper moteur(NbPas, 28, 26, 24, 22);
bool Irrigation = false; 

// Déclaration des Marge Max/Min
#define TEMP_Max 24
#define TEMP_Min 24
#define HUMID_Max 60
#define HUMID_Min 50
#define LIGHT_Max 70
#define LIGHT_Min 40


bool tempAnomaly = false;
bool lightAnomaly = false;


void setup() {

 Serial.begin(9600);

 //les LEDs chauffante
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);

 //les LEDs pour la luminosite
 pinMode(11, OUTPUT);
 pinMode(10, OUTPUT);

 //Pin pour le moteur du ventilateur
 pinMode(6, OUTPUT);
  //moteur eteint initialement 

 //Déclaration des DHTs
 dht_Air.begin();
 dht_Sol.begin();

 //Déclaration du detecteur de mouvement 
 pinMode(3, INPUT); 

 //Pin du servo moteur (DOOM)
 Doom.attach(5);

 //PINs du stepper motor (IRRIGARION)
 pinMode(22,OUTPUT);  pinMode(24,OUTPUT);
 pinMode(26,OUTPUT);  pinMode(28,OUTPUT);
 moteur.setSpeed(10); //vitesse du moteur
  
 
}



/*******************************************************************************TEMPERATURE DE L'AIR****************************************************************************/
void Verify_temp(float temp) {
  
    if(temp > TEMP_Max){
     analogWrite(6,250);
      
    } else{analogWrite(6,0);}

    if (temp < TEMP_Min){
     tempAnomaly = true;
    } else {
       tempAnomaly = false;}
  
}



/*******************************************************************************HUMIDITE DE L'AIR****************************************************************************/

void Verify_hum(float hum, DHT dht) {

  if( hum< HUMID_Min){
     moteur.step(2048);
  }
 
}


/*******************************************************************************DETECTEUR D'INSECTE****************************************************************************/

void Verify_Bugs(int Motion, int sound) {
  if ( (Motion == HIGH ) && ((sound<100)|| (sound>80)) ) {
    if (pirState == LOW) {
      
      pirState = HIGH;
      Bugs = true; 
    }
  } else {
    if (pirState == HIGH) {
      pirState = LOW;
    }else {Bugs = false; }
  }
}



/*******************************************************************************LUMINOSITE*********************************************************************************/

void Verify_Light(int luminosite, int light, bool &anomalyFlag) {
  if ((luminosite > LIGHT_Max) || (luminosite < LIGHT_Min)) {
    if (luminosite < LIGHT_Min){
    anomalyFlag = true;
  } else {
    anomalyFlag = false;}
  }
}


/*******************************************************************************DECLANCHEMENT DES LEDs****************************************************************************/
void handleAnomalies() {
  if (tempAnomaly) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    
  }

  

  if (lightAnomaly) {
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
  } else {
   digitalWrite(11, LOW);
   digitalWrite(10, LOW);
   }
  }

void Doom_Controller(bool doom){
 
 if ((doom == true) && (position= 0)){
     for (position = 0; position <= 120; position += 1) { 
    
       Doom.write(position);              
       doom = false;                      
  }}else if ((doom == true) && (position= 120)){
        
        for (position = 120; position >= 0; position -= 1) { 
            Doom.write(position);              
            doom = false; 
  }

}}

void Irrigation_controller(bool wtr){

  if (wtr == true){
     moteur.step(2048);
  }
}



void loop() {

  float temperature_Air = dht_Air.readTemperature();
  float humid_Air = dht_Air.readHumidity();
  float humid_Sol = dht_Sol.readHumidity();
  int Motion = digitalRead(3);
  int soundValue = digitalRead(soundSensorPin);
  int luminosite = analogRead(photoresistorPin);
  int light = map(luminosite, 0, 1023, 0, 1023);
  
  
 
 

 
  Serial.print(temperature_Air);
  Serial.print(",");
  Serial.print(humid_Air);
  Serial.print(",");
  Serial.print(humid_Sol);
  Serial.print(",");
  Serial.print(light);
  Serial.print(",");
  Serial.print(Bugs);
  Serial.print(",");
  Serial.print(doom);
  Serial.print(",");
  Serial.print(Irrigation);
  Serial.print(",");
  

  
  

  Verify_temp(temperature_Air);
  Verify_Bugs(Motion,soundValue);
  Verify_Light(luminosite, light, lightAnomaly);
  Verify_hum(humid_Air, dht_Air);
  Verify_hum(humid_Sol, dht_Sol);
  Doom_Controller(doom);
  Irrigation_controller(Irrigation);
  handleAnomalies();
 
  
  
  
  

  delay(100);
}