#include "mbed.h"
#include <string>

//static BufferedSerial serial_port(USBTX, USBRX);
BufferedSerial serial_port(PA_2, PA_3);

//Pines Pot-Digital
DigitalOut CS(PA_10); //D2  
DigitalOut UD(PB_3);  //D3
DigitalOut INC(PB_5); //D4

//definicion hilos
Thread hilo_rx_serie;
Thread hilo_mover_pote;

//Prototipo de funciones
//void cero(void);  //Llevar el PD a la posicion cero
void mover(void); //Desplazar el PD
void rx_serie(void); //Recepcion serial

//Variables
int valor_potRX=100;
int valor_potAT=0;
char mensaje[20]={0};

// main() runs in its own thread in the OS
int main()
{
serial_port.write("Hola.! Arranque del programa.\n\r",31);
hilo_rx_serie.start(rx_serie);
hilo_mover_pote.start(mover);

    while (true) {
      
    }
}

void mover(void)
{
int d;
while (true) {
    if( valor_potRX > valor_potAT)
    {
        d=valor_potRX-valor_potAT;
        UD = 1;
        CS = 0;
        wait_us (1);
        for (int o=0; o<d; o++)
            {
            INC = 0;
            wait_us (1);
            INC=1;
            wait_us (1);
            } 
        INC = 1; 
        wait_us (1);
        CS = 1;
        valor_potAT=valor_potRX;
    }
    if( valor_potRX < valor_potAT)
    {
        d=valor_potAT-valor_potRX;
        UD = 0;
        CS = 0;
        wait_us (1);
        for (int o=0; o<d; o++)
            {
            INC = 0;
            wait_us (1);
            INC=1;
            wait_us (1);
            } 
        INC = 1; 
        wait_us (1);
        CS = 1;
        valor_potAT=valor_potRX;
    }
    ThisThread::sleep_for(500ms);
}
}

void rx_serie(void)
{
char men_in[4];

    while(true)
    {
        for(int i=0; i<5;i++)
            {
            men_in[i]  = NULL;
            } 
        if (serial_port.readable())
        {
            serial_port.read(men_in,3); //recibo 3 caracteres
            valor_potRX = stoi(men_in);
            serial_port.write("Ok.\n\r",5);
        } 
        else{
            sprintf(mensaje,"En el paso: %d\n\r",valor_potRX);
            serial_port.write(mensaje,16);
        }
        ThisThread::sleep_for(1000ms);
    }
}

/*
void cero(void)      // Dejar en 0 el potenciometro
{
CS = 1;
INC = 1;

CS = 0;
wait_us(1);
UD = 0;
for (int n=0; n<100; n++)
    {
    INC = 0;
    wait_us (1);
    INC=1;
    wait_us (1);
    }
INC = 1; 
wait_us (1);
CS = 1;
}*/