#include <SoftwareSerial.h>
#include <Servo.h>

#define trigPin 7
#define echoPin 8

SoftwareSerial mySerial(10, 11); // RX, TX
Servo miServo;
long duracion, distancia1, angulo ;
int salir = 0, recibido = 0;
int medidaSuelo = 0;
int k = 0;
long int aux  = 0;
int aux2 = 0;
char cadena[1];
int medidaEncontrada, medidaPrevia, amplitudPositiva = 130, amplitudNegativa = 59, distanciaHipotetica = 0;
int maximoEnfoque = 30;
int salida = 0;
int izquierda=2;
int derecha=3;
int adelante=0;
int detras=1;
int contador = 14;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  miServo.attach(12);
}

void loop() { // run over and over
  unsigned long time;

  miServo.write(100);
  char c;
  if(!recibido){
    while(c = mySerial.read() != '1' && !recibido){
      delay(200);
    }
  }
  recibido = 1;
  
  time = millis();
  
  if(time <= 2500){
    aux = medirDefecto();
    if(aux > 65){
      k--;
    }else aux2+=aux;
  }else{
    if(k > 0){
      medidaSuelo = aux2/k;
      //Serial.println(String(medidaSuelo) + "DEFECTO DISTANCIA SUELO") ;
      k = 0;
    }
    medidaSuelo = 50;
    //Primer Barrido
    for(int i=amplitudNegativa; i < amplitudPositiva; i+=10){
      miServo.write(i);
      medidaEncontrada = medir();
      //distanciaHipotetica = medidaSuelo / cos(i-100);
      
      if(medidaEncontrada < medidaSuelo){
        amplitudPositiva = ((i + maximoEnfoque) > amplitudPositiva ? amplitudPositiva :  i + maximoEnfoque);
        amplitudNegativa = ((i - maximoEnfoque) < amplitudNegativa ? amplitudNegativa :  i - maximoEnfoque);
        maximoEnfoque -= 5;
        
        Serial.println("Amplitud Negativa " + String(amplitudNegativa) + " Amplitud Positiva" + String(amplitudPositiva) + " Enfoque " + String(maximoEnfoque));
      }
      else {
        contador--;
        if (contador == 0) reinicio();
      }
      /*
      if(abs(abs(medidaEncontrada - distanciaHipotetica) - abs(medidaPrevia - distanciaHipotetica)) > 3 ){
        amplitudPositiva = (i-maximoEnfoque > amplitudNegativa ? i-maximoEnfoque : amplitudNegativa);
        amplitudNegativa = (i+maximoEnfoque < amplitudPositiva ? i+maximoEnfoque : amplitudPositiva);
        maximoEnfoque -= 5;
        Serial.println("Amplitud Negativa " + String(amplitudNegativa) + " Amplitud Positiva" + String(amplitudPositiva) + " Enfoque " + String(maximoEnfoque));
      }*/
      medidaPrevia = medidaEncontrada;
      //Serial.println(String(distancia1) + " cm.");
      delay(150);
    }

    //Si está enfocado, avisamos al pic
    
    if(maximoEnfoque <= 0){
      enviarPic(amplitudPositiva-amplitudNegativa);
      reinicio();
    }

    //Segundo Barrido
    for(int i=amplitudPositiva; i > amplitudNegativa; i-=10){
      miServo.write(i);
  
      if(medidaEncontrada < medidaSuelo){
        amplitudPositiva = ((i + maximoEnfoque) > amplitudPositiva ? amplitudPositiva :  i + maximoEnfoque);
        amplitudNegativa = ((i - maximoEnfoque) < amplitudNegativa ? amplitudNegativa :  i - maximoEnfoque);
        maximoEnfoque -= 5;
        Serial.println("Amplitud Negativa " + String(amplitudNegativa) + " Amplitud Positiva" + String(amplitudPositiva) + " Enfoque " + String(maximoEnfoque));
      }
      else {
        contador--;
        if (contador == 0) reinicio();
      }
      //Serial.println(String(distancia1) + " cm(INVERSO).") ;
      delay(150);  
    }
  }
}

int medir(){
    digitalWrite(trigPin, LOW);        // Nos aseguramos de que el trigger está desactivado
    delayMicroseconds(10);              // Para asegurarnos de que el trigger esta LOW
    digitalWrite(trigPin, HIGH);       // Activamos el pulso de salida
    delayMicroseconds(30);             // Esperamos 10µs. El pulso sigue active este tiempo
    digitalWrite(trigPin, LOW);        // Cortamos el pulso y a esperar el echo
    duracion = pulseIn(echoPin, HIGH) ;
    distancia1 = duracion / 2 / 29.1  ;

    return distancia1;
}

int medirDefecto(){
  digitalWrite(trigPin, LOW);        // Nos aseguramos de que el trigger está desactivado
  delayMicroseconds(2);              // Para asegurarnos de que el trigger esta LOW
  digitalWrite(trigPin, HIGH);       // Activamos el pulso de salida
  delayMicroseconds(15);             // Esperamos 10µs. El pulso sigue active este tiempo
  digitalWrite(trigPin, LOW);        // Cortamos el pulso y a esperar el echo
  duracion = pulseIn(echoPin, HIGH) ;
  distancia1 = duracion * 0.017  ;
  k++;
  delay(100);
  return distancia1;
}

void enviarPic(int angulo){
    char c;

  int Tgiro=abs(angulo-100)*(10000/360);

  if( angulo>100){
      -//Serial.println("D "+String(derecha)+" T "+ String(Tgiro));
      Serial.println(derecha);
      mySerial.println(derecha);
  }
  if( angulo<100){
      //Serial.println("D "+String(izquierda)+" T "+ String(Tgiro));
      Serial.println(izquierda);
      mySerial.println(izquierda);
  }
  Serial.println(String(angulo) + " MEDIDA FINAL") ;
  
  delay(1000);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  
  Serial.println("");
  Serial.println("He salido");
  delay(1000);
}

void reinicio(){
    amplitudNegativa = 59;
    amplitudPositiva = 130;
    maximoEnfoque = 30;
    contador = 14;
}

