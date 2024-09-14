#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Funcion

int main(int argc, char *argv[]) {
    
    // ----1. Sobre salida estandar y a error ---- clear
    
    // int age = 15;   // %d
    // char sex = 'F'; // %c
    // char name[] = "Chimoltrufia"; // %s
    // int list_ints[3];

    // list_ints[0] = 1;
    // list_ints[1] = 2;
    // list_ints[2] = 3;
    // list_ints[3] = 3;

    // // Salida estándar stdout
    // printf("Soy la %s (sexo: %d) y tengo %d años\n", name, age, age);
    // fprintf(stderr, "Esto es un error\n");
    // float peso = 2.3;         // %f
    // double cosa = 1.602;  // %lf
    // printf("peso = %.1f - cosa = %lf\n", peso, cosa);

    int age = 15;   // %d
    char sex = 'F'; // %c
    char name[] = "Chimoltrufia"; // %s
    printf("Soy la %s (sexo: %c) y tengo %d años\n", name, sex, age);
    fprintf(stderr, "Esto es un error\n");
    float peso = 2.3;         // %f
    double cosa = 1.602;  // %lf
    printf("peso = %f - cosa = %lf\n", peso, cosa);
    return 0;
}

