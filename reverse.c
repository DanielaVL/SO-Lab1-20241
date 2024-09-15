#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void read_lines(FILE *input_file, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr);
void write_lines(FILE *output_file, char **lines, size_t size);
void reverse_stdin_stdout(char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr);
void reverse_file_stdout(const char *input_filename, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr);
void reverse_file_file(const char *input_filename, const char *output_filename, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr);

int main(int argc, char *argv[]) {
    char **lines = NULL;
    size_t size = 0;
    size_t capacity = 0;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc == 1) {
        // Sin argumentos: Leer de stdin y escribir en stdout
        reverse_stdin_stdout(&lines, &size, &capacity);
    } else if (argc == 2) {
        // Un argumento: Leer de archivo y escribir en stdout
        reverse_file_stdout(argv[1], &lines, &size, &capacity);
    } else if (argc == 3) {
        // Dos argumentos: Leer de archivo y escribir en archivo
        reverse_file_file(argv[1], argv[2], &lines, &size, &capacity);
    }

    // Liberar memoria
    for (size_t i = 0; i < size; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}

void reverse_stdin_stdout(char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr) {
    read_lines(stdin, lines_ptr, size_ptr, capacity_ptr);
    write_lines(stdout, *lines_ptr, *size_ptr);
}

void reverse_file_stdout(const char *input_filename, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr) {
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
        exit(1);
    }

    read_lines(input_file, lines_ptr, size_ptr, capacity_ptr);
    write_lines(stdout, *lines_ptr, *size_ptr);

    fclose(input_file);
}

void reverse_file_file(const char *input_filename, const char *output_filename, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr) {
    // Verificar si los archivos de entrada y salida son iguales
    struct stat input_stat, output_stat;

    // Obtener información del archivo de entrada
    if (stat(input_filename, &input_stat) != 0) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
        exit(1);
    }

    // Obtener información del archivo de salida
    if (stat(output_filename, &output_stat) != 0) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", output_filename);
        exit(1);
    }

    // Verificar si los archivos son el mismo
    if (input_stat.st_ino == output_stat.st_ino && input_stat.st_dev == output_stat.st_dev) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
        exit(1);
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", output_filename);
        fclose(input_file);
        exit(1);
    }

    read_lines(input_file, lines_ptr, size_ptr, capacity_ptr);
    write_lines(output_file, *lines_ptr, *size_ptr);

    fclose(input_file);
    fclose(output_file);
}

void read_lines(FILE *input_file, char ***lines_ptr, size_t *size_ptr, size_t *capacity_ptr) {
    const size_t INCREMENT_SIZE = 10;
    char **lines = *lines_ptr;
    size_t size = *size_ptr;
    size_t capacity = *capacity_ptr;

    char *line = NULL; 
    size_t len = 0;
    ssize_t read;

    /*
    documentación para getline:
    https://www.it.uc3m.es/pbasanta/asng/course_notes/input_output_getline_es.html
    */

    while ((read = getline(&line, &len, input_file)) != -1) {
        // Verificar si el array está lleno
        if (size == capacity) {
            capacity += INCREMENT_SIZE;  // Incrementar la capacidad en bloques de 10
            char **temp = realloc(lines, capacity * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "malloc failed\n");
                free(line);
                // Liberar las líneas ya almacenadas
                for (size_t i = 0; i < size; i++) {
                    free(lines[i]);
                }
                free(lines);
                exit(1);
            }
            lines = temp;
        }
        // Almacenar una copia de la línea
        lines[size] = strdup(line);
        if (lines[size] == NULL) {
            fprintf(stderr, "malloc failed\n");
            free(line);
            // Liberar las líneas ya almacenadas
            for (size_t i = 0; i < size; i++) {
                free(lines[i]);
            }
            free(lines);
            exit(1);
        }
        size++;
    }
    free(line);  // Liberar el buffer utilizado por getline

    // Actualizar los punteros y tamaños
    *lines_ptr = lines;
    *size_ptr = size;
    *capacity_ptr = capacity;
}

void write_lines(FILE *output_file, char **lines, size_t size) {
    for (ssize_t i = size - 1; i >= 0; i--) {
        fprintf(output_file, "%s", lines[i]);
    }
}
