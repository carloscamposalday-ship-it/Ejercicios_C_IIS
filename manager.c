#include <stdio.h>
#include <string.h>

#define MAX_MIEMBROS 5
#define ENERGIA_CONCIERTO 10
#define FANS_POR_POPULARIDAD 10

typedef struct {
    char nombre[30];
    int bonus;
    int vigencia;
} Cancion;

typedef struct {
    char nombre[30];
    int popularidad;
    int energia;
    int energia_max;
    int fans;
    Cancion *cancion;
} Idol;

typedef struct {
    char nombre[30];
    Idol miembros[MAX_MIEMBROS];
    int num_miembros;
    Idol *lider;
} Grupo;

void mostrar(const Idol *idol) {
    if (idol == NULL) return;

    printf("Nombre: %s\n", idol->nombre);
    printf("Popularida: %d\n", idol->popularidad);
    printf("Energia: %d/%d\n", idol->energia, idol->energia_max);
    printf("Fans: %d\n", idol->fans);
    if (idol->cancion != NULL) {
        printf("Cancion: %s (Bonus: %d, Vigencia: %d)\n",
               idol->cancion->nombre, idol->cancion->bonus, idol->cancion->vigencia);
    } else {
        printf("Cancion: Ninguna\n");
    }
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
    i.energia = limitar(i.energia - 5, 0, i.energia_max);
    printf("Dentro de ensayar_por_valor -> Popularidad: %d, Energia: %d\n", i.popularidad, i.energia);
}

void ensayar(Idol *i) {
    if (i == NULL) return;
    
    if (!esta_activa(i)) {
        printf(" %s esta agotada! No puede ensayar.\n", i->nombre);
        return;
    }

    i->popularidad += 5;
    i->energia = limitar(i->energia - 5, 0, i->energia_max);
}

void descansar(Idol *i) {
    if (i == NULL) return;

    i->energia = limitar(i->energia + 5, 0, i->energia_max);
}

int colaboracion(Idol *a, Idol *b) {
    if (a == NULL || b == NULL || a == b) {
        return 0;
    }

    a->popularidad += 10;
    b->popularidad += 10;
    a->energia = limitar(a->energia - 15, 0, a->energia_max);
    b->energia = limitar(b->energia - 15, 0, b->energia_max);

    printf("Colaboracion exitosa entre %s y %s!\n", a->nombre, b->nombre);
    return 1;
}

void asignar_cancion(Idol *i, Cancion *c) {
    if (i == NULL) return;
    i->cancion = c;
}

int popularidad_total(const Idol *i) {
    if (i == NULL) return 0;
    if (i->cancion != NULL && i->cancion->vigencia > 0) {
        return i->popularidad + i->cancion->bonus;
    }
    return i->popularidad;
}

void inicializar_grupo(Grupo *g, const char *nombre) {
    if (g == NULL) return;
    strncpy(g->nombre, nombre, sizeof(g->nombre) - 1);
    g->nombre[sizeof(g->nombre) - 1] = '\0';
    g->num_miembros = 0;
    g->lider = NULL;
}

int agregar_miembro(Grupo *g, const Idol *i) {
    if (g == NULL || i == NULL) return 0;
    if (g->num_miembros >= MAX_MIEMBROS) return 0;
    g->miembros[g->num_miembros] = *i;
    g->num_miembros++;
    return 1;
}

int asignar_lider(Grupo *g, int indice) {
    if (g == NULL) return 0;
    if (indice < 0 || indice >= g->num_miembros) return 0;
    g->lider = g->miembros + indice;
    return 1;
}

int contar_activas(const Grupo *g) {
    int total = 0;
    if (g == NULL) return 0;
    for (int k = 0; k < g->num_miembros; k++) {
        if (esta_activa(g->miembros + k)) total++;
    }
    return total;
}

/* Recibe Grupo * (sin const) porque devuelve un Idol * modificable
   que apunta dentro del arreglo del grupo. */
Idol *idol_mas_cansada(Grupo *g) {
    if (g == NULL || g->num_miembros == 0) return NULL;

    Idol *peor = g->miembros;
    for (int k = 1; k < g->num_miembros; k++) {
        if ((g->miembros + k)->energia < peor->energia) {
            peor = g->miembros + k;
        }
    }
    return peor;
}

void mostrar_grupo(const Grupo *g) {
    if (g == NULL) return;
    printf("Grupo: %s (%d integrantes)\n", g->nombre, g->num_miembros);
    for (int k = 0; k < g->num_miembros; k++) {
        printf("--- Integrante %d ---\n", k + 1);
        mostrar(g->miembros + k);
    }
}

/* Formula de fans:   fans += popularidad_total(idol) * 10
   Gasto de energia:  ENERGIA_CONCIERTO (10) por idol activa, con limitar.
   Vigencia: la cancion de cada idol activa pierde UNA presentacion por
   concierto. Si una cancion es compartida por 2 integrantes activas,
   pierde 2 presentaciones en ese concierto (una por cada integrante
   activa que la tiene). La vigencia nunca baja de 0. */
void dar_concierto(Grupo *g) {
    if (g == NULL) return;

    for (int k = 0; k < g->num_miembros; k++) {
        Idol *idol = g->miembros + k;

        if (!esta_activa(idol)) continue;

        idol->fans += popularidad_total(idol) * FANS_POR_POPULARIDAD;
        idol->energia = limitar(idol->energia - ENERGIA_CONCIERTO, 0, idol->energia_max);

        if (idol->cancion != NULL && idol->cancion->vigencia > 0) {
            idol->cancion->vigencia--;
            if (idol->cancion->vigencia == 0) {
                printf("La cancion %s ya no esta vigente.\n", idol->cancion->nombre);
            }
        }
    }
}

/* g->miembros[i] equivale a *(g->miembros + i) porque el nombre de un
   arreglo equivale a la direccion de su primer elemento, y sumar 1 a
   un apuntador a Idol avanza un elemento completo (sizeof(Idol) bytes),
   no un solo byte. */
void demostrar_aritmetica(const Grupo *g) {
    if (g == NULL) return;

    printf("g->miembros (nombre del arreglo)  = %p\n", (const void *)g->miembros);
    printf("&g->miembros[0]                   = %p\n", (const void *)&g->miembros[0]);

    for (int k = 0; k < g->num_miembros; k++) {
        printf("i=%d  &g->miembros[i] = %p | g->miembros + i = %p | %s\n",
               k,
               (const void *)&g->miembros[k],
               (const void *)(g->miembros + k),
               (&g->miembros[k] == g->miembros + k) ? "IGUALES" : "DISTINTAS");
    }

    if (g->num_miembros >= 2) {
        long salto = (const char *)(g->miembros + 1) - (const char *)g->miembros;
        printf("Sumar 1 al apuntador avanza %ld bytes (sizeof(Idol) = %zu)\n",
               salto, sizeof(Idol));
    }
}

void mostrar_cancion_lider(const Grupo *g) {
    if (g == NULL) {
        printf("No hay grupo.\n");
        return;
    }
    if (g->lider == NULL) {
        printf("El grupo %s no tiene lider.\n", g->nombre);
        return;
    }
    if (g->lider->cancion == NULL) {
        printf("La lider %s no tiene cancion asignada.\n", g->lider->nombre);
        return;
    }
    printf("Cancion de la lider %s: %s\n", g->lider->nombre, g->lider->cancion->nombre);
}

void preparar_presentacion(Grupo *g) {
    if (g == NULL) return;

    Idol *cansada = idol_mas_cansada(g);
    if (cansada != NULL) {
        descansar(cansada);
    }

    dar_concierto(g);

    printf("=== Resumen de la presentacion: %s ===\n", g->nombre);
    for (int k = 0; k < g->num_miembros; k++) {
        const Idol *idol = g->miembros + k;
        printf("- %s | Energia: %d/%d | Fans: %d\n",
               idol->nombre, idol->energia, idol->energia_max, idol->fans);
    }
    printf("Integrantes activas: %d de %d\n", contar_activas(g), g->num_miembros);
    mostrar_cancion_lider(g);
}

int main() {
    Idol jennie = {"jennie", 50, 100, 100, 12000, NULL};
    Idol rose = {"rose", 45, 100, 100, 11000, NULL};
    Idol *p = &jennie;

    Cancion discos[2] = {
        {"Starlight", 10, 3},
        {"Nova", 5, 6}
    };

    printf("%p\n", (void *)p);
    printf("%p\n", (void *)&jennie);

    mostrar(&jennie);

    printf("\n=== Demostracion HU-08: Paso por valor vs Paso por apuntador ===\n");
    printf("1. Llamando a ensayar_por_valor(jennie):\n");
    ensayar_por_valor(jennie);
    printf("Despues de la funcion (en main): ");
    mostrar(&jennie);

    printf("\n2. Llamando a ensayar(&jennie) (por apuntador):\n");
    ensayar(&jennie);
    printf("Despues de la funcion (en main): ");
    mostrar(&jennie);

    printf("\n=== Pruebas HU-07 ===\n");
    printf("esta_activa_energia(0) = %d\n", esta_activa_energia(0));
    printf("esta_activa_energia(1) = %d\n", esta_activa_energia(1));
    printf("Activa al inicio? %d\n", esta_activa(&jennie));

    printf("\n=== Pruebas HU-09: Colaboracion Segura ===\n");
    printf("Colaboracion misma persona (&jennie, &jennie): %d\n", colaboracion(&jennie, &jennie));
    printf("Colaboracion con NULL (NULL, &rose): %d\n", colaboracion(NULL, &rose));
    printf("Colaboracion valida (&jennie, &rose): %d\n", colaboracion(&jennie, &rose));

    printf("\n=== Pruebas HU-10: Dinámica por Turnos ===\n");
    Idol *turno = &jennie;
    Idol *otra = &rose;

    while (esta_activa(turno) && esta_activa(otra)) {
        ensayar(turno);
        Idol *aux = turno;
        turno = otra;
        otra = aux;
    }

    if (esta_activa(&jennie)) {
        printf("Jennie conserva energia!\n");
    } else if (esta_activa(&rose)) {
        printf("Rose conserva energia!\n");
    } else {
        printf("Ambas se agotaron!\n");
    }

    printf("\n=== Pruebas HU-12, HU-13 y HU-14: Canciones ===\n");
    asignar_cancion(&jennie, &discos[0]);
    asignar_cancion(&rose, &discos[0]);
    printf("Jennie tras asignar Starlight:\n");
    mostrar(&jennie);
    printf("Rose tras compartir Starlight:\n");
    mostrar(&rose);

    asignar_cancion(&jennie, NULL);
    printf("\nJennie tras desasignar cancion (NULL):\n");
    mostrar(&jennie);

    jennie.energia = 100;
    rose.energia = 100;

    printf("\n=== 20 Ensayos Seguidos (Jennie y Rose) ===\n");
    for (int k = 1; k <= 20; k++) {
        printf("--- Ensayo %d ---\n", k);
        
        printf("[Jennie]\n");
        ensayar(&jennie);
        mostrar(&jennie);
        printf("Activa? %d\n", esta_activa(&jennie));

        printf("[Rose]\n");
        ensayar(&rose);
        mostrar(&rose);
        printf("Activa? %d\n\n", esta_activa(&rose));
    }

    printf("=== 20 Descansos Seguidos (Jennie y Rose) ===\n");
    for (int k = 1; k <= 20; k++) {
        printf("--- Descanso %d ---\n", k);

        printf("[Jennie]\n");
        descansar(&jennie);
        mostrar(&jennie);
        printf("Activa? %d\n", esta_activa(&jennie));

        printf("[Rose]\n");
        descansar(&rose);
        mostrar(&rose);
        printf("Activa? %d\n\n", esta_activa(&rose));
    }

    jennie.popularidad = 50;
    rose.popularidad = 45;
    jennie.energia = 100;
    rose.energia = 100;
    jennie.fans = 12000;
    rose.fans = 11000;
    discos[0].vigencia = 3;
    asignar_cancion(&jennie, &discos[0]);
    asignar_cancion(&rose, &discos[0]);

    Grupo grupo;
    inicializar_grupo(&grupo, "Blackpink");
    agregar_miembro(&grupo, &jennie);
    agregar_miembro(&grupo, &rose);
    asignar_lider(&grupo, 0);

    printf("\n=== Pruebas HU-21: Conciertos (cancion compartida, vigencia 3) ===\n");
    for (int k = 1; k <= 4; k++) {
        printf("\n--- Concierto %d ---\n", k);
        dar_concierto(&grupo);
        mostrar_grupo(&grupo);
    }
    printf("\nConcierto con grupo NULL (no debe fallar):\n");
    dar_concierto(NULL);
    printf("OK\n");

    printf("\n=== Pruebas HU-22: g->miembros[i] vs *(g->miembros + i) ===\n");
    demostrar_aritmetica(&grupo);

    printf("\n=== Pruebas HU-23: mostrar_cancion_lider (3 escenarios) ===\n");
    printf("1) Lider con cancion:\n");
    mostrar_cancion_lider(&grupo);

    grupo.lider->cancion = NULL;
    printf("2) Lider sin cancion:\n");
    mostrar_cancion_lider(&grupo);

    Grupo sin_lider;
    inicializar_grupo(&sin_lider, "Sin Lider");
    printf("3) Grupo sin lider:\n");
    mostrar_cancion_lider(&sin_lider);
    printf("4) Grupo NULL:\n");
    mostrar_cancion_lider(NULL);

    printf("\n=== Pruebas HU-24: preparar_presentacion varias veces ===\n");
    discos[1].vigencia = 6;
    asignar_cancion(grupo.lider, &discos[1]);
    for (int k = 1; k <= 12; k++) {
        printf("\n--- Presentacion %d ---\n", k);
        preparar_presentacion(&grupo);
    }

    printf("\nPresentacion de grupo sin lider ni integrantes:\n");
    preparar_presentacion(&sin_lider);
    printf("\nPresentacion con grupo NULL:\n");
    preparar_presentacion(NULL);
    printf("OK\n");

    return 0;
}