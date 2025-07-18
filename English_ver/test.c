
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE   11
#define START_TEXT_SIZE 32  

#define BASE_LEN_TEXT 10
#define BASE_LEN_SENT 50

typedef struct{
	char* string;
}Sentence;


typedef struct{
	Sentence *sentences;
    int count;       
    int capacity; 
}Text;

bool check_double(Sentence checkd_sent, Sentence sent2){

    /*
    Несколько проверок, первая - если отличается длина, то сразу нет
    А затем посимволно, и если хоть один символ отличается, то нет
    */

   if(strlen(checkd_sent.string) != strlen(sent2.string))
        return false;
   

    for(int i = 0; i < strlen(checkd_sent.string); i++){
        if(tolower(checkd_sent.string[i]) != tolower(sent2.string[i]))
            return false;
    }

    return true;
}


void del_double(Text *text){
    /*
    Пробегает по всем предложениям, сравнивая с теми, что находятся сверху, если они равны, то сдвигает всё вверх.
    */

   for(int i = 0; i < text->count; i++){
        for(int j = 0; j < i; j++){
            
            if(check_double(text->sentences[i], text->sentences[j])){
                
                // Сдвигаем предложения
                for(int k = i; k < text->count; k++){
                    text->sentences[k] = text->sentences[k + 1];
                }
                i--;
                text->count--;
                break;
            }
        }
   }  
}

void mod1(Text *text){
    /*
        Пробегаемся по каждому предложению.
        Затем строем строку. И выводим её
    */

    for(int i = 0; i < text->count; i++){
        const char delimiters[] = " ,."; // Разделители
        char *token;


        char strss[strlen(text->sentences[i].string)];
        strcpy(strss, text->sentences[i].string);

        token = strtok(strss, delimiters);

        char mask[strlen(token) + 3];

        strcpy(mask, token);

        int mask_sb = strlen(mask);

        token = strtok(NULL, delimiters); 
        while (token != NULL) {
            printf("|%s|\n", token);
            
            
            if(strlen(token) < mask_sb){
                mask_sb = strlen(token);
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != token[i])
                        mask[i] = '?';
                }

                mask[mask_sb] = '*';
                mask[mask_sb + 1] = '\0';
            } else if (strlen(token) == mask_sb){
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != token[i])
                        mask[i] = '?';
                }
            } else{
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != token[i])
                        mask[i] = '?';
                }

                mask[mask_sb ] = '*';
                mask[mask_sb + 1] = '\0';
            }
            

            token = strtok(NULL, delimiters); 
        }

        puts(mask);
    }
}

void free_text(Text *text) {
    for (size_t i = 0; i < text->count; i++) {
        puts(text->sentences[i].string);
        free(text->sentences[i].string); // Освобождаем каждую строку
    }
    free(text->sentences); // Освобождаем массив предложений
}

// Delete tabs and space before first symbol
void del_tabulation(Text *text){

    for(int i = 0; i < text->count; i++){
        int shift = 0;
        while(isspace(text->sentences[i].string[shift]) != 0)
            shift++;
        for(int j = 0; j < strlen(text->sentences[i].string); j++) 
            text->sentences[i].string[j] = text->sentences[i].string[j+shift];  
    }  
}

int read_text(char** text) {
    size_t size = START_TEXT_SIZE; 
    int len = 0;             
    *text = malloc(size);       

    if (*text == NULL) {
        fprintf(stderr, "Ошибка при выделении памяти.\n");
        exit(EXIT_FAILURE);
    }

    char local_string[SIZE]; 
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
    return len;
}


void convert_text(char** text, Text* cnv_txt){


    // Проверяем есть ли точка
    bool dot = (*(strchr(*text, '\0') - 1)) != '.';
    // Инициализируем массив указателей на предложения
    cnv_txt->sentences = malloc(BASE_LEN_TEXT * sizeof(Sentence));

    // Инициализируем превую строку
    cnv_txt->sentences[0].string = malloc(BASE_LEN_SENT * sizeof(char)); 
     
    cnv_txt->count = 0;
    cnv_txt->capacity = BASE_LEN_TEXT;

    int local_len_sent = 0;
    int limit_len_sent = BASE_LEN_SENT;


    for(int i = 0; (*text)[i] != '\0'; i++){
        
        if(local_len_sent + 2 >= limit_len_sent){
            limit_len_sent*= 2;
            cnv_txt->sentences[cnv_txt->count].string = realloc(cnv_txt->sentences[cnv_txt->count].string, limit_len_sent);
        }

        if(cnv_txt->count + 2>= cnv_txt->capacity){
            cnv_txt->capacity*= 2;
            cnv_txt->sentences = realloc(cnv_txt->sentences, cnv_txt->capacity* sizeof(Sentence));
        }

        if((*text)[i] == '.'){
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent + 1] = '\0';

            local_len_sent = 0;
            limit_len_sent = BASE_LEN_SENT;
            cnv_txt->count++;
            cnv_txt->sentences[cnv_txt->count].string = malloc(BASE_LEN_SENT * sizeof(char)); 
        }else{
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            local_len_sent++;
        }
    }


    if(dot){
        // Последнее предложение тоже нужно учесть, если оно не заканчивается точкой     
        cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = '.';
        cnv_txt->sentences[cnv_txt->count].string[local_len_sent  + 1] = '\0';

        cnv_txt->count++; 
    }

    


}


void mod2(Text *text){

    
    for(int i = 0; i < text->count; i++){

        const char delimiters[] = " ,.\n\t"; // Разделители
        char *word;

        word = strtok(text->sentences[i].string, delimiters);

        int flag = false;

        while (word != NULL) {  

            if(islower(word[0]))
                flag = true;
            word = strtok(NULL, delimiters); 
        }

        if(flag){
            free(text->sentences[i].string);

            for(int k = i; k < text->count; k++){
                text->sentences[k] = text->sentences[k + 1];
            }
            i--;
            text->count--;
        }
    }
}

// Вывод текста
void text_output(Text *text){
    for (size_t i = 0; i < text->count; i++) 
        printf("%s|", text->sentences[i].string);
}

int main(void){
	printf("Course work for option 5.3, created by Ivan Aleinikov.\n");


	int mode;
	scanf("%d", &mode);
    getchar();

    char *text = NULL;

    Text result;
	switch (mode)
	{
	case 1:
		int len_text = read_text(&text);

        
        convert_text(&text, &result);
        del_tabulation(&result);
        del_double(&result);

        
        free_text(&result);
		break;
	case 2:
		read_text(&result);

		mod2(&result);

		text_output(&result);

		free_text(&result);
		break;
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:
		break;
	
	default:
		printf("Error: ошибка ввода, неправильный режим.");
	}


	return 0;
}
