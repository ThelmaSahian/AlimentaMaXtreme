#include <Servo.h>
#include "HX711.h"

#define DOUT  A1
#define CLK  A0

HX711 balanza(DOUT, CLK);

Servo myservo;  

// posicion del servo en angulo 0
int pos = 0;   

// pines ultrasonico
const int pinecho = 8;
const int pintrigger = 10;
const int pinled = 13;

// VARIABLES PARA CALCULOS
unsigned int tiempo, distancia;

void setup() {
  // declara el pin 9 para ejecutar
  myservo.attach(9); 
  
  // PREPARAR LA COMUNICACION SERIAL
  Serial.begin(9600);
  
  // CONFIGURAR PINES DE ENTRADA Y SALIDA
  pinMode(pinecho, INPUT);
  pinMode(pintrigger, OUTPUT);
  pinMode(13, OUTPUT);

  // celda de carga
 
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  balanza.set_scale(1750); // Establecemos la escala
  balanza.tare(-20);  //El peso actual es considerado Tara.
}

void loop() {
  // ENVIAR PULSO DE DISPARO EN EL PIN "TRIGGER"
  digitalWrite(pintrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pintrigger, HIGH);
  // EL PULSO DURA AL MENOS 10 uS EN ESTADO ALTO
  delayMicroseconds(10);
  digitalWrite(pintrigger, LOW);
 
  // MEDIR EL TIEMPO EN ESTADO ALTO DEL PIN "ECHO" EL PULSO ES PROPORCIONAL A LA DISTANCIA MEDIDA
  tiempo = pulseIn(pinecho, HIGH);
 
  // LA VELOCIDAD DEL SONIDO ES DE 340 M/S O 29 MICROSEGUNDOS POR CENTIMETRO
  // DIVIDIMOS EL TIEMPO DEL PULSO ENTRE 58, TIEMPO QUE TARDA RECORRER IDA Y VUELTA UN CENTIMETRO LA ONDA SONORA
  distancia = tiempo / 58;
 
  // ENVIAR EL RESULTADO AL MONITOR SERIAL
  Serial.print(distancia);
  Serial.println(" cm");
  delay(200);
 
  // Lee el gramaje del plato de comida junto con croquetas e imprime el resultado
  Serial.print("Gramaje de croquetas: ");
  int croquetas = balanza.get_units(20);
  Serial.print(croquetas);
  Serial.println(" g");
  delay(500);

    // Si hay croquetas en el dispensador y hay menos de 300g de croquetas en el plato de comida
    if (distancia <= 20 && croquetas <=100) {
       digitalWrite(13, LOW);

       //Se enciende servomotor y se abre puerta de dispensador
      for (pos = 0; pos <= 90; pos += 1) { //comienza el ciclo en 0 grados hasta 90
        
      // in steps of 1 degree
      myservo.write(pos);              
      delay(50); //  el tiempo de ejecucion 
      }
  
    // ENCENDER EL LED CUANDO SE CUMPLA CON CIERTA DISTANCIA
  } else {
    digitalWrite(13, HIGH);
    delay(100);
  }
  delay(50); // tiempo de pausa
  for (pos = 90; pos >= 0; pos -= 1) { //cierra ciclo de 90 grados a su pocicion inicial
    myservo.write(pos);              
    delay(50); // tiempo de ejecucion para regresar a su posicion inicial                        
  }
  delay(50);
//exit(0); //detiene el ciclo
  }
