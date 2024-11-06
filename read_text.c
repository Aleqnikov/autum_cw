#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "read_text.h"

#define SIZE   11
#define START_TEXT_SIZE 32  

#define BASE_LEN_TEXT 10
#define BASE_LEN_SENT 50

/*
    Все они второстепенные и нужны для модификаций главной функции, которая собственно и читает текст. Она должная быть в каждом режиме для 
    чтения текста. - read_text
    Функции данного файла преднозначены для первичной обработки текста, введенного в консоль.
    Сначала функция first_read_text считывает ввод в поток stdin и заполняет массив, динамически его расширяя.
    Затем полученный текст мы конвертируем из простого массива, в структуру Text, у которой есть поля - sentences
    которое является массивом структур sentence, в которых и хранятся предложения, count - количество предложений которые 
    уже есть в тексте, и capacity - текущая ёмкость текста.
    А затем просходит удаление лишних пробелов перед началом предложения.

    В свою очередь очистка памяти должна проходить после того как отработает модуль
*/


// Проверяет правильно ли выделена память
void chk_crr_memall(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Ошибка: память не была выделена (NULL указатель).\n");
        exit(EXIT_FAILURE); // Завершите программу с ошибкой
    } 
}

void free_text(Text *text) {
    for (size_t i = 0; i < text->count + 1; i++) {
        puts(text->sentences[i].string); // Временно, для отладки
        free(text->sentences[i].string); // Освобождаем каждую строку
    }
    free(text->sentences); // Освобождаем массив предложений
}

// Удаляет лишние символы до начала предложения
void del_tabulation(Text *text){

    for(int i = 0; i < text->count + 1; i++){
        int shift = 0;
        while(isspace(text->sentences[i].string[shift]) != 0)
            shift++;
        for(int j = 0; j < strlen(text->sentences[i].string); j++) 
            text->sentences[i].string[j] = text->sentences[i].string[j+shift];  
    }  
}

// Первичное считывание текста из stdin
void first_read_text(char** text) {
    size_t size = START_TEXT_SIZE; 
    int len = 0;             
    *text = malloc(size);       

    chk_crr_memall(*text);

    // Строка - буфер
    char local_string[SIZE]; 

    while (true) {
        if (fgets(local_string, sizeof(local_string), stdin) == NULL)
            break; 
        
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

            chk_crr_memall(*text);
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


void convert_text(char** text, Text* cnv_txt){

    // Инициализируем массив указателей на предложения
    cnv_txt->sentences = malloc(BASE_LEN_TEXT * sizeof(Sentence));

    // Инициализируем превую строку
    cnv_txt->sentences[0].string = malloc(BASE_LEN_SENT * sizeof(char)); 
     
    cnv_txt->count = 0;
    cnv_txt->capacity = BASE_LEN_TEXT;

    int local_len_sent = 0;
    int limit_len_sent = BASE_LEN_SENT;


    for(int i = 0; (*text)[i] != '\0'; i++){
        

        // Проверяем есть ли место в предложении под символ, если нет то динамически расширяем
        if(local_len_sent + 2 >= limit_len_sent){
            limit_len_sent*= 2;
            cnv_txt->sentences[cnv_txt->count].string = realloc(cnv_txt->sentences[cnv_txt->count].string, limit_len_sent);

            chk_crr_memall(cnv_txt->sentences[cnv_txt->count].string);
        }

        // Проверяем, есть ли место в тексте, если нет то динамически расширяем
        if(cnv_txt->count + 2>= cnv_txt->capacity){
            cnv_txt->capacity*= 2;
            cnv_txt->sentences = realloc(cnv_txt->sentences, cnv_txt->capacity* sizeof(Sentence));

            chk_crr_memall(cnv_txt->sentences);
        }


        if((*text)[i] == '.'){
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent + 1] = '\0';

            // Если конец предложения, то обнуляем прошлые переменные
            local_len_sent = 0;
            limit_len_sent = BASE_LEN_SENT;
            cnv_txt->count++;
            cnv_txt->sentences[cnv_txt->count].string = malloc(BASE_LEN_SENT * sizeof(char)); 
        }else{
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            local_len_sent++;
        }
    }

    // Если последняя строка не заканчивается точной, то мы последним символом заканчиваем строку
    cnv_txt->sentences[cnv_txt->count].string[local_len_sent + 1] = '\0';

}

// Собирает все функции воедино и апперирует ими
void read_text(Text* cnv_txt){

    char *text = NULL;

    first_read_text(&text);
    convert_text(&text, cnv_txt);
    del_tabulation(cnv_txt);

    free(text);
}