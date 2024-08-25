#include <pic14/pic12f675.h>

typedef unsigned int word; 
//word __at 0x2007 __CONFIG = (_BODEN_OFF & _MCLRE_OFF & _PWRTE_OFF); // Configuración de los fuses
word __at 0x2007 __CONFIG = (_BODEN_OFF & _MCLRE_OFF & _PWRTE_OFF & _WDT_OFF); // Configuración de los fuses con el Watchdog Timer desactivado

#define _XTAL_FREQ 48000000
void delay_ms(unsigned int ms); // Declaración de la función delay_ms

void main(void) {
    TRISIO = 0x00;  // Poner todos los pines como salidas
    GPIO = 0x00;    // Poner pines en bajo
    ANSEL = 0x00;   // Deshabilitar entradas analógicas
    unsigned int time = 10;  // Tiempo de retardo

    // Loop infinito
    while (1) {
        GPIO = (0b0100001);// como resultado un 1
        delay_ms(time);
        
        GPIO = (0b0010001);// como resultado un 9

        delay_ms(time);
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;

    // Ajuste de tiempo para 1 ms a 4 MHz
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 250; j++);  // Ajusta este valor si es necesario
   
   __asm nop __endasm;  // Instrucción NOP para asegurar precisión en el retardo
   
   
    }


}
