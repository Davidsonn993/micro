#include <pic14/pic12f683.h> 

typedef unsigned int word; 
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _PWRTE_OFF & _WDT_OFF); // Configuración de los fuses con el Watchdog Timer desactivado

#define _XTAL_FREQ 48000000
void delay_ms(unsigned int ms); // Declaración de la función delay_ms
void display_number(unsigned char decenas, unsigned char unidades );
unsigned char simple_random(); // Nueva función de generación de números aleatorios



void main(void) {
    TRISIO = 0b00001000;  // Configurar GPIO3 como entrada y los demás pines como salidas (TRISIO bit 3 a 1)
    GPIO = 0x00;          // Poner todos los pines en bajo
    ANSEL = 0x00;         // Deshabilitar entradas analógicas

    unsigned int numero=99 ;  // Número predeterminado para mostrar
    unsigned char decenas, unidades; // Variables para almacenar decenas y unidades
   
    
    while (1) {
         // Leer el estado del botón en GPIO3
        if (GP3) {  // Si GPIO3 está en alto (botón presionado)
            numero = simple_random();
        }

        // Separa decenas y unidades
        decenas = numero / 10;
        unidades = numero % 10;

        // Mostrar el número en el display por 3 segundos
        display_number(decenas, unidades);
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

void display_number(unsigned char decenas, unsigned char unidades ) {
    unsigned int elapsed_time = 0;  // Tiempo transcurrido
    unsigned int time = 10;  // Tiempo de retardo para multiplexación

    // Mapea las decenas y unidades a los códigos GPIO según tu configuración
    unsigned char decenas_gpio = (decenas == 1) ? 0b0100001 :
                                 (decenas == 2) ? 0b0100010 :
                                 (decenas == 3) ? 0b0100011 :
                                 (decenas == 4) ? 0b0100100 :
                                 (decenas == 5) ? 0b0100101 :
                                 (decenas == 6) ? 0b0100110 :
                                 (decenas == 7) ? 0b0100111 :
                                 (decenas == 8) ? 0b0110000 :
                                 0b0110001;  // Para decenas 9

    unsigned char unidades_gpio = (unidades == 0) ? 0b0000000 :
                                  (unidades == 1) ? 0b0000001 :
                                  (unidades == 2) ? 0b0000010 :
                                  (unidades == 3) ? 0b0000011 :
                                  (unidades == 4) ? 0b0000100 :
                                  (unidades == 5) ? 0b0000101 :
                                  (unidades == 6) ? 0b0000110 :
                                  (unidades == 7) ? 0b0000111 :
                                  (unidades == 8) ? 0b0001000 :
                                  0b0001001;  // Para unidades 9

   
       // Mostrar decenas
        GPIO = decenas_gpio;
        delay_ms(time);
      
 	// Mostrar unida
        GPIO = unidades_gpio;
        delay_ms(time);
     
    }

// Nueva función para generar números pseudoaleatorios
unsigned char simple_random() {
    static unsigned char seed = 123;  // Semilla inicial, puede ser cualquier número
    seed = (seed * 5 + 3) & 0xFF;     // Generador simple de números pseudoaleatorios
    return seed % 100;                // Limitar el resultado al rango de 0-99
}
