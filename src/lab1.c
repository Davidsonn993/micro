#include <pic14/pic12f683.h> 

typedef unsigned int word; 
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _PWRTE_OFF & _WDT_OFF); // Configuración de los fuses con el Watchdog Timer desactivado

#define _XTAL_FREQ 48000000     //declaramos la frecuencia con la que vamos a trabajar
void delay_ms(unsigned int ms); // declaracion de la funcion delay_ms
void display_number(unsigned char decenas, unsigned char unidades );
unsigned char simple_random(); // función de generación de números aleatorios



void main(void) {
    TRISIO = 0b00001000;  // Configurar GPIO3 como entrada y los demás pines como salidas (TRISIO bit 3 a 1)
    GPIO = 0x00;          // Poner todos los pines en bajo
    ANSEL = 0x00;         // Deshabilitar entradas analógicas

    unsigned int numero;  // Número predeterminado para mostrar
    unsigned char decenas, unidades; // Variables para almacenar decenas y unidades
    unsigned char boton_contador = 0;  // Contador de presiones de botón
    unsigned char boton_presionado = 0;  // Estado del botón: 0 = no presionado, 1 = presionado 

while (1) {
        // Leer el estado del botón en GPIO3
        if (GP3 && !boton_presionado) {  // Si GPIO3 está en alto (botón presionado) y no estaba presionado antes
            delay_ms(50);  // Pequeño retardo para debounce
            
            // Comprobar nuevamente el estado del botón después del retardo
            if (GP3) {  // Si aún está presionado, confirmar lo
                boton_presionado =1;  //  presionado
                boton_contador++;

                if (boton_contador <= 10) {
                    numero = simple_random();  // Generar número aleatorio
                } else {
                    numero =99 ;  // Mostrar 99 si el contador excede 10
                    boton_contador = 0;  // Reiniciar el contador
                }
            }
        }

        if (!GP3) {  // Si GPIO3 está en bajo (botón no presionado)
            boton_presionado = 0;  // Marcar como no presionado
        }

        // Separar decenas y unidades
        decenas = numero / 10;
        unidades = numero % 10;

        // Mostrar el número en el display
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
                                  0b0010001;  // Para unidades 9

   
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
