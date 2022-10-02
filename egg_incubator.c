//*******************************************Librerias ********************************

#include <stdio.h>
#include <Adafruit_PCD8544.h>

//*********************************Definicion e inicializacion de los pines********************************

// Pines para el controlador PID
const int punto_operacion=A3; //Punto de operación
float diferencia;
float rango;
float porcentaje_humedad;
float porcentaje_humedad_nuevo;

// Pines para el termistor
const int termistor_principal=A4;
int tension_v=0;
const float resistor_1=10000;
float term_resistor;
float term_resistor_log;
float temperature_sh;
float X = 1.009249522e-03;
float Y = 2.378405444e-04;
float Z = 2.019202697e-07;

// Pines para los LEDS de alerta de temperatura
const int Led_alto=2;
const int Led_bajo=3;


const int dht11_humedad= A5; // Pin para el sensor de Humedad

const int rx_tx_com=13; // Pin para la comunicacion con la computadora

const int resistor_calienta = 9; // Pin para el esistor para calentar

Adafruit_PCD8544 pantalla_led = Adafruit_PCD8544(7, 5, 6, 4, 8); // Pantalla PCD8544