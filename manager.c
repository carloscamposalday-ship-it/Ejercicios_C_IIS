#include <stdio.h>

typedef struct {
    char nombre[30];
    int popularidad;
    int energia;
    int energia_max;
    int fans;
} Idol;

void mostrar(const Idol *idol) {
    if (idol == NULL) return;

    printf("Nombre: %s\n", idol->nombre);
    printf("Popularida: %d\n", idol->popularidad);
    printf("Energia: %d/%d\n", idol->energia, idol->energia_max);
    printf("Fans: %d\n", idol->fans);
}

int limitar(int valor, int minimo, int maximo) {
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

int esta_activa_energia(int energia) {
    if (energia > 0) {
        return 1;
    } else {
        return 0;
    }
}

int esta_activa(const Idol *i) {
    if (i == NULL) return 0;
    return esta_activa_energia(i->energia);
}

void ensayar_por_valor(Idol i) {
    i.popularidad += 5;
    i.energia = limitar(i.energia - 20, 0, i.energia_max);
    printf("Dentro de ensayar_por_valor -> Popularidad: %d, Energia: %d\n", i.popularidad, i.energia);
}

void ensayar(Idol *i) {
    if (i == NULL) return;
    
    if (!esta_activa(i)) {
        printf(" %s esta agotada! No puede ensayar.\n", i->nombre);
        return;
    }

    i->popularidad += 5;
    i->energia = limitar(i->energia - 20, 0, i->energia_max);
}

void descansar(Idol *i) {
    if (i == NULL) return;

    i->energia = limitar(i->energia + 20, 0, i->energia_max);
}

int main() {
    Idol jennie = {"jennie", 50, 100, 100, 12000};
    Idol *p = &jennie;

    printf("%p\n", (void *)p);
    printf("%p\n", (void *)&jennie);

    mostrar(&jennie);

    printf("\n Paso por valor vs Paso por apuntador ===\n");
    printf("1. Llamando a ensayar_por_valor(jennie):\n");
    ensayar_por_valor(jennie);
    printf("Despues de la funcion (en main): ");
    mostrar(&jennie);

    printf("\n2. Llamando a ensayar(&jennie) (por apuntador):\n");
    ensayar(&jennie);
    printf("Despues de la funcion (en main): ");
    mostrar(&jennie);

    printf("\n=== Pruebas Idol Activa ===\n");
    printf("esta_activa_energia(0) = %d\n", esta_activa_energia(0));
    printf("esta_activa_energia(1) = %d\n", esta_activa_energia(1));
    printf("Activa al inicio? %d\n", esta_activa(&jennie));

    printf("\n=== 20 Ensayos Seguidos ===\n");
    for (int k = 1; k <= 20; k++) {
        printf("Ensayo %d:\n", k);
        ensayar(&jennie);
        mostrar(&jennie);
        printf("Activa? %d\n\n", esta_activa(&jennie));
    }

    printf("=== 20 Descansos Seguidos ===\n");
    for (int k = 1; k <= 20; k++) {
        printf("Descanso %d:\n", k);
        descansar(&jennie);
        mostrar(&jennie);
        printf("Activa? %d\n\n", esta_activa(&jennie));
    }

    return 0;
}