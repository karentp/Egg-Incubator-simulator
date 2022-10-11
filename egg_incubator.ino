//*******************************************Librerias ********************************

#include <stdio.h>
#include <util/delay.h>
#include <Adafruit_PCD8544.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>
//*********************************Definicion e inicializacion de los pines********************************//

// Pines para el controlador PID
const int PIP=A3; //Punto de operación
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
const int Led_bajo=4;


const int dht11_humedad= A5; // Pin para el sensor de Humedad

const int rx_tx_com=13; // Pin para la comunicacion con la computadora

const int resistor_calienta = 9; // Pin para el esistor para calentar

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 8); // Pantalla PCD8544


void setup() {
  /// Se inicia el puerto serial
  Serial.begin(9600); 
  ///Configuracion de la pantalla de display
  display.begin();
  display.setContrast(50); 
  display.display(); 
  delay(500);
  display.clearDisplay();   
  display.setTextSize(1);  
  display.setTextColor(BLACK); 
  ///Configurar pines de interes como una salida
  pinMode(Led_alto,OUTPUT);
  pinMode(Led_bajo,OUTPUT);
  pinMode(rx_tx_com, INPUT);
 
}

float resistor_temperatura(float temp_objetivo){
	
	if (temp_objetivo < 32){
		if(temperature_sh >= 30){
			analogWrite(resistor_calienta, roundf(0));
			return 0;
		}
		else{
			analogWrite(resistor_calienta, roundf(30*(255/80)));
			return 30*100/80;
		}
	}
	else if (temp_objetivo < 42){ 
		if (temperature_sh<=42.0){	
			analogWrite(resistor_calienta, roundf(42*(255/80))); 
			return 42*100/80;												
		}else{
			analogWrite(resistor_calienta, roundf(0));
			return 0;
		}
	}
	else{
		if(temp_objetivo < temperature_sh){
			analogWrite(resistor_calienta, roundf(0));
			return 0;
		}else{
			analogWrite(resistor_calienta, roundf(temp_objetivo*(255/80)));
			return temp_objetivo*100/80;
		}
	}
}


void alerta(){

 ///Funcion para controlar los LEDs y dar alertas segun la temperatura

 // Alerta cuando la temperatura esta por debajo de los 32 grados. Leds azules.
 if(temperature_sh<32.0){  
    digitalWrite(Led_bajo, HIGH);
	digitalWrite(Led_alto, LOW);
    
 }

// Alerta cuando la temperatura esta por encima de los 42 grados. Leds rojos.
 else if(temperature_sh>42.0){ 
	digitalWrite(Led_alto, HIGH);
	digitalWrite(Led_bajo, LOW);

	}

//No hay alerta porque la temperatura esta en el rango.
 else{ 
	digitalWrite(Led_bajo, LOW);
	digitalWrite(Led_alto, LOW);

	}
}



float termistor_dar_temperatura(){

 //Funcion para leer la entrada del termistor y obtener la temperatura

 tension_v= analogRead(termistor_principal);
 term_resistor = resistor_1 * (1023.0 / (float)tension_v - 1.0);
 term_resistor_log = log(term_resistor);
 temperature_sh = (1.0 / (X + Y*term_resistor_log + Z*term_resistor_log*term_resistor_log*term_resistor_log));
 temperature_sh = temperature_sh - 273.15;
 return temperature_sh;
}



void loop() { 
	int control = 0;
	porcentaje_humedad = (analogRead(A3));
	alerta();
	display.print("Temperatura es: \n");
	display.print(termistor_dar_temperatura());
	display.print("C"); 
	display.print("\n");
	display.print("Humedad es:");
	display.print(analogRead(dht11_humedad)/10.23);
	display.print("%"); 
	display.print("\n");
	display.print("Punto de Op. es:");
	display.print(porcentaje_humedad/10.23); 
	display.print("\n");
	display.print("Intervalo es:");
	display.print(rango);
	display.display();
	delay(500);
	display.clearDisplay();
	if(digitalRead(rx_tx_com)==LOW){ 
		if(control==0){
			Serial.print("Temperatura,Humedad");
			Serial.print("\n");
		}
		else{		
    		Serial.print(temperature_sh);
    		Serial.println(",");
    		Serial.print(analogRead(dht11_humedad)/10.23);
    		Serial.println("%");
			Serial.print("\n");
		}
	}
	
	
	termistor_dar_temperatura();

	///Controlador PID
	porcentaje_humedad_nuevo = porcentaje_humedad/5.1;
	diferencia = porcentaje_humedad_nuevo - temperature_sh;
	if (diferencia != 0){ 
		rango = resistor_temperatura(temperature_sh+diferencia); 
	}else {																	
		rango = resistor_temperatura(0);		
	}
  
}
