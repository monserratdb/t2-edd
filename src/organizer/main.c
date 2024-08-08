#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKAGE_SIZE 3

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

bool can_place_package(int **compartment, int rows, int cols, int package[PACKAGE_SIZE][PACKAGE_SIZE], int start_row, int start_col) {
    for (int i = 0; i < PACKAGE_SIZE; i++) {
        for (int j = 0; j < PACKAGE_SIZE; j++) {
            if (package[i][j] == 1) {
                if (start_row + i >= rows || start_col + j >= cols || compartment[start_row + i][start_col + j] != 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void place_package(int **compartment, int package[PACKAGE_SIZE][PACKAGE_SIZE], int start_row, int start_col, char package_id) {
    for (int i = 0; i < PACKAGE_SIZE; i++) {
        for (int j = 0; j < PACKAGE_SIZE; j++) {
            if (package[i][j] == 1) {
                compartment[start_row + i][start_col + j] = package_id;
            }
        }
    }
}

void remove_package(int **compartment, int package[PACKAGE_SIZE][PACKAGE_SIZE], int start_row, int start_col) {
    for (int i = 0; i < PACKAGE_SIZE; i++) {
        for (int j = 0; j < PACKAGE_SIZE; j++) {
            if (package[i][j] == 1) {
                compartment[start_row + i][start_col + j] = 1;
            }
        }
    }
}

bool backtrack(int **compartment, int rows, int cols, char *package_ids, int packages[][PACKAGE_SIZE][PACKAGE_SIZE], int num_packages, int index) {
    if (index == num_packages) {
        return true;
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (can_place_package(compartment, rows, cols, packages[index], row, col)) {
                place_package(compartment, packages[index], row, col, package_ids[index]);
                if (backtrack(compartment, rows, cols, package_ids, packages, num_packages, index + 1)) {
                    return true;
                }
                remove_package(compartment, packages[index], row, col);
            }
        }
    }

    return false;
}

int main(int argc, char **argv) {
    check_arguments(argc, argv);

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    int rows, cols;
    fscanf(input_file, "%d", &rows);
    fscanf(input_file, "%d", &cols);

    int **compartment = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        compartment[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            fscanf(input_file, "%d", &compartment[i][j]);
        }
    }

    int num_packages;
    fscanf(input_file, "%d", &num_packages);

    char package_ids[num_packages];
    int packages[num_packages][PACKAGE_SIZE][PACKAGE_SIZE];

    for (int i = 0; i < num_packages; i++) {
        char package_buffer_id[2];
        fscanf(input_file, "%s\n", package_buffer_id);
        package_ids[i] = package_buffer_id[0];

        for (int j = 0; j < PACKAGE_SIZE; j++) {
            for (int k = 0; k < PACKAGE_SIZE; k++) {
                fscanf(input_file, "%d", &packages[i][j][k]);
            }
        }
    }

    if (!backtrack(compartment, rows, cols, package_ids, packages, num_packages, 0)) {
        printf("No se pudo colocar todos los paquetes\n");
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (compartment[i][j] == 1) {
                fprintf(output_file, "1");
            } else if (compartment[i][j] == 0) {
                fprintf(output_file, "0");
            } else {
                fprintf(output_file, "%c", compartment[i][j]);
            }
            if (j < cols - 1) {
                fprintf(output_file, " ");
            }
        }
        fprintf(output_file, "\n");
    }

    // Liberar memoria
    for (int i = 0; i < rows; i++) {
        free(compartment[i]);
    }
    free(compartment);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
