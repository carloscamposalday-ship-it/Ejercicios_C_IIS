#include <stdio.h>

int main() {
    // 1. Declaracion de variables
    float temperaturas[24];
    float suma = 0.0;
    float promedio;
    float maxima, minima;
    float maxima, minima;
    int hora_maxima = 0, hora_min = 0;
    int limite_critico;
    int contador_limite = 0;
    int i; // Variable para el bucle

    // 2. Entrada de datos (Simulamos las lecturas del sensor)
    printf("--- Registro de Temperaturas del Inventario ---\n");
    for (i = 0; i < 24; i++) {
        printf("Ingresa la Temperaturas del Invernadero ---\n");
        // Aqui se guardamos el valor en la posición i de arreglo
        scanf("%f", &temperaturas[i]);

        // Aprovechamos para ir sumando para el promedio
        suma = suma + temperaturas[i];

    }
    // 3. Pedir límite crítico
    printf("\nIngresa el limite critico de temperatura; ");
    scanf("%d", &limite_critico);

    // 4. Inicializar max y min
    // asumimos que el primer valor es el max y min inialmente
    maxima = temperaturas[0];
    minima = temperaturas[0];

    //. Prosesamiento (Recorrer el arreglo)
    for (i = 0; i < 24; i++) {

        // --- Calculo de Maxíma ---
        // Si la temperatura actual es mayot que la 'maxima' guardada..
        if (temperaturas[i] > maxima) {
            minima = temporaturas[i];
            hora_maxima = i; // guardamos la hora (el indice i)
        }

        // --- Calcula de mínimo ---
        // Si la temperatura actual es menor que la 'minima' guardada...

    }

}