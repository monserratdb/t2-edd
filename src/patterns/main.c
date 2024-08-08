#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Verificación de argumentos
static bool check_arguments(int argc, char **argv) {
    if (argc != 3) {
        printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
        printf("Donde:\n");
        printf("\tINPUT es la ruta del archivo de input\n");
        printf("\tOUTPUT es la ruta del archivo de output\n");
        exit(1);
    }
    return true;
}

int calcular_altura(int nodos) {
    int altura = 0;
    while ((1 << altura) <= nodos) {
        altura++;
    }
    return altura - 1;
}

// comparar 
bool comparar_subarboles(char* arbol, int pos_arbol, char* consulta, int pos_consulta, int altura, int tam_arbol, int tam_consulta) {
    if (altura < 0) return true;
    if (pos_arbol >= tam_arbol || pos_consulta >= tam_consulta) return false; // nollegar a accesos fuera de límites
    if (arbol[pos_arbol] != consulta[pos_consulta]) return false;

    int izq_arbol = 2 * pos_arbol + 1;
    int der_arbol = 2 * pos_arbol + 2;
    int izq_consulta = 2 * pos_consulta + 1;
    int der_consulta = 2 * pos_consulta + 2;

    return comparar_subarboles(arbol, izq_arbol, consulta, izq_consulta, altura - 1, tam_arbol, tam_consulta) &&
           comparar_subarboles(arbol, der_arbol, consulta, der_consulta, altura - 1, tam_arbol, tam_consulta);
}


int main(int argc, char **argv) {
    check_arguments(argc, argv);

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    int N_NODOS;
    fscanf(input_file, "%d\n", &N_NODOS);

    char* arbol = (char*)malloc((N_NODOS + 1) * sizeof(char));
    fscanf(input_file, "%s\n", arbol);
    arbol[N_NODOS] = '\0'; // asegurarse de que la cadena esté terminada

    int N_CONSULTAS;
    fscanf(input_file, "%d\n", &N_CONSULTAS);

    for (int i = 0; i < N_CONSULTAS; i++) {
        int M_NODOS_CONSULTA;
        fscanf(input_file, "%d\n", &M_NODOS_CONSULTA);
        char* consulta = (char*)malloc((M_NODOS_CONSULTA + 1) * sizeof(char));
        fscanf(input_file, "%s\n", consulta);
        consulta[M_NODOS_CONSULTA] = '\0'; //que la cadena esté terminada

        int altura_consulta = calcular_altura(M_NODOS_CONSULTA);
        bool encontrado = false;
        bool first = true; // controlar el espacio

        //bscar subárbol usando comparación directa 
        for (int j = 0; j <= N_NODOS - M_NODOS_CONSULTA; j++) {
            if (arbol[j] == consulta[0] && comparar_subarboles(arbol, j, consulta, 0, altura_consulta, N_NODOS, M_NODOS_CONSULTA)) {
                if (!first) {
                    fprintf(output_file, " ");
                }
                fprintf(output_file, "%d", j);
                first = false;
                encontrado = true;
            }
        }
        if (!encontrado) {
            fprintf(output_file, "-1");
        }
        fprintf(output_file, "\n");

        free(consulta);
    }

    free(arbol);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
