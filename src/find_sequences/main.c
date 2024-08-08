#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PRIME 101  // Un número primo para el cálculo del hash

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

//calcular el hash inicial de una cadena
unsigned long calculate_hash(const char *str, int length) {
  unsigned long hash = 0;
  for (int i = 0; i < length; i++) {
    hash = (hash * PRIME + str[i]);
  }
  return hash;
}

//actualizar
unsigned long recalculate_hash(const char *str, int old_index, int new_index, unsigned long old_hash, unsigned long high_order) {
  old_hash = (old_hash - str[old_index] * high_order) * PRIME + str[new_index];
  return old_hash;
}

int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  if (input_file == NULL || output_file == NULL) {
    fprintf(stderr, "Error al abrir los archivos.\n");
    exit(1);
  }

  int TAMANO_DOCUMENTO;
  fscanf(input_file, "%d", &TAMANO_DOCUMENTO);

  // Leer el documento
  char *documento = (char *)malloc((TAMANO_DOCUMENTO + 1) * sizeof(char));  // +1 para el carácter nulo
  fscanf(input_file, "%s", documento);

  int largo_cadena, cantidad_consultas;
  fscanf(input_file, "%d", &largo_cadena);
  fscanf(input_file, "%d", &cantidad_consultas);

  // Leer las cadenas de consulta y buscarlas en el documento
  for (int q = 0; q < cantidad_consultas; q++) {
    char *cadena = (char *)malloc((largo_cadena + 1) * sizeof(char));  // +1 para el carácter nulo
    fscanf(input_file, "%s", cadena);

    // calcular el hash
    unsigned long hash_cadena = calculate_hash(cadena, largo_cadena);
    unsigned long hash_subcadena = calculate_hash(documento, largo_cadena);
    unsigned long high_order = 1;
    for (int i = 0; i < largo_cadena - 1; i++) {
      high_order = (high_order * PRIME);
    }

    fprintf(output_file, "Consulta %d\n", q);
    bool found = false;
    for (int i = 0; i <= TAMANO_DOCUMENTO - largo_cadena; i++) {
      if (hash_cadena == hash_subcadena) {
        // check de caracteres
        if (strncmp(documento + i, cadena, largo_cadena) == 0) {
          fprintf(output_file, "%d\n", i);
          found = true;
        }
      }
      // calcular siguiente hash
      if (i < TAMANO_DOCUMENTO - largo_cadena) {
        hash_subcadena = recalculate_hash(documento, i, i + largo_cadena, hash_subcadena, high_order);
      }
    }
    if (!found) {
      fprintf(output_file, "");  // No imprimir línea en blanco
    }
    free(cadena);
  }

  /* Cerrar archivos y liberar memoria */
  fclose(input_file);
  fclose(output_file);
  free(documento);

  return 0;
}