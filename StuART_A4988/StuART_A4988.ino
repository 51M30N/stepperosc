/*
ADAPTED BY 51M30N
contact@lacon.dev
*/


#include <Wire.h>
///////////////////////////////////////////////////////////////////////////////////////////////////PAR LÀ LE NOM DU RECEPTEUR
const char* DEVICE_ID = "StuART_A4988/0";                                  ////note ici le nom du code et de la version ESP8266_$capteur-$version
const char* PREFIX = "/CTRL/";
String completeAddress = String(PREFIX) + String(DEVICE_ID);
const char* DEVICE_NAME = completeAddress.c_str();
//OSCErrorCode error;
#include "secret_ssidlocal.h" //
#include "StuART_OSClocal.h" //
int freq = 25; // en ms, vitesse de rafraîchissement

///////////////////// def de l a4988
#include <AccelStepper.h>
// A4988 GPIO config
#define dirPin_1 D1 // D4 //Z
#define stepPin_1 D2 // D7
#define dirPin_2 D3 //Y
#define stepPin_2 D4
#define MS1 D5 // A0
#define MS2 D6 // A1
#define MS3 D7 // A2
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper[2] = {AccelStepper(motorInterfaceType, stepPin_1, dirPin_1), AccelStepper(motorInterfaceType, stepPin_2, dirPin_2)};
// variables // stepper
int Acceleration = 100 ; //faible valeur acceleration douce // valeur par default
int MaxSpeed = 500; // valeur par default
byte MS_state;
const bool MS[5][3] = {
     {0, 0, 0}, // [0] FULL STEP
     {1, 0, 0}, // [1] 1/2
     {0, 1, 0}, // [2] 1/4
     {1, 1, 0}, // [3] 1/8
     {1, 1, 1}  // [4] 1/16
     };
byte CONTROL_state;
long CONTROL_rotation[3] = {1, 1, 1};

#include "a4988fonctions.h" //
  
void setup() { ///////////////////////////////SETUP///////////////////////
  
// SETUP BUILTINLED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led
  
/// SERIAL INITIALISATION
  Serial.begin(115200);
  while (!Serial)
  delay(10); // will pause Zero, Leonardo, etc until serial console opens
   
  wificonnection(); //connection au réseau wifi connu
  openOSCport();    // ouverture du port OSC
  HelloWorld();     // Envoi d'une message helloword à l'IP_CIBLE

///////////////////setup de l'A4988
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  // stepper setup  VALEURS PAR DEFAULT
  for (int i = 0; i < 2; i++) {stepper[i].setMaxSpeed(MaxSpeed); }
  for (int i = 0; i < 2; i++) {stepper[i].setAcceleration(Acceleration); }
  set_resolution(2);
  set_zero();                  // ZERO
  // RESET DE DEMARRAGE
  reset_();
  // AND PLAY
}
///////////////////////////////END SETUP///////////////////////






///////////////////////////////LOOP///////////////////////
void loop() {  
oscreceiver(); // écoute du port OSC d'entree
//count_time(); // calcul du temps écoulé depuis la dernière mesure
//program_count_time(); // calcul du temps écoulé depuis le dernir tour dans le program
// WAIT A MOMENT  
    // delay(TIME_LOOP);
    // delay(freq); 
    
// Run to target position with set speed and acceleration/deceleration:
    for (int i = 0; i <= 2; i++) {stepper[i].run(); }
/*    if (program == 0){      // programm == 0 => pas de program
    }
    else{
      //send_program();
    }*/
} 
///////////////////////////////END LOOP///////////////////////



void oscreceiver(){                                 //RECEPTION OSC
  OSCMessage bundle;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {                               // ROUTE OSC
      bundle.dispatch("/led", led);                         // /led 0-1 : turn on-off ledbuildin
      bundle.dispatch("/IPCIBLE", ipcible);                 // /IPCIBLE $1 $2 $3 $4
      bundle.dispatch("/POOL_REQUEST", PoolREQUEST);        // reply osc information
      bundle.dispatch("/helloworld", reply_helloworld);     // reply osc information
      bundle.dispatch("/speed", osc_speed);                 // /speed [int] : vitesse de rotation                 // arg2-facultatif (0=> moteur1 1=> moteur2 empty=> moteurs1&2)
      bundle.dispatch("/acc", osc_acc);                     // /acc [int] : acceletration                         // arg2-facultatif (0=> moteur1 1=> moteur2 empty=> moteurs1&2)
      bundle.dispatch("/speedacc", osc_speedacc);           // /speedacc [int] [int] : vitesse accelaration       // arg2-facultatif (0=> moteur1 1=> moteur2 empty=> moteurs1&2)
      bundle.dispatch("/moveto", osc_moveto);               // /moveto [int] : deplacement moteur                 // arg2-facultatif (0=> moteur1 1=> moteur2 empty=> moteurs1&2)
      bundle.dispatch("/resolution", osc_resolution);       // /resolution [0-4] : 1/1 1/2 1/4 1/8 1/16
      bundle.dispatch("/setzero", osc_setzero);             // /setzero : initialisation de la position zero
    } 
    else {
      /* error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error); */
    }
  }
 }                                                  //END RECEPTION OSC
 


//////////////////////////////////////////////////////////////// END OSC receiver et fonctions associés
