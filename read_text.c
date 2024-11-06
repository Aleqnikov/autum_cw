
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define SIZE   11
#define START_TEXT_SIZE 32  


void read_text(char **text) {
    size_t size = START_TEXT_SIZE; 
    size_t len = 0;             
    *text = malloc(size);       

    if (*text == NULL) {
        fprintf(stderr, "Ошибка при выделении памяти.\n");
        exit(EXIT_FAILURE);
    }

    char local_string[256]; 
    while (1) {
        if (fgets(local_string, sizeof(local_string), stdin) == NULL) {
            break; 
        }

        // Проверка для конца текста
        if (strcmp(local_string, "\n") == 0) {
            if (len > 0 && (*text)[len - 1] == '\n') {
                break; 
            }
        }

        // Если длина текста превышает размер массива, увеличиваем массив
        if (len + strlen(local_string) >= size) {
            size *= 2; 
            *text = realloc(*text, size);

            if (*text == NULL) {
                fprintf(stderr, "Ошибка при выделении памяти.\n");
                exit(EXIT_FAILURE);
            }
        }

        strcpy(*text + len, local_string);
        len += strlen(local_string);
    }

    // Устанавливаем финальный символ конца строки, если необходимо
    if (len > 0 && (*text)[len - 1] != '\n') {
        (*text)[len] = '\0';
    } else {
        (*text)[len - 1] = '\0'; 
    }
}