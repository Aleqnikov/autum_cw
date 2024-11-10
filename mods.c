#include "mods.h"
#include "text_tools.h"



void mod1(Text *text){
    /*
        Пробегаемся по каждому предложению.
        Затем строем строку. И выводим её
    */

    for(int i = 0; i < text->count; i++){
        const char delimiters[] = " ,."; // Разделители
        char *token;

        token = strtok(text->sentences[i].string, delimiters);

        char mask[strlen(token) + 3];

        strcpy(mask, token);

        int mask_sb = strlen(mask);

        token = strtok(NULL, delimiters); 
        while (token != NULL) {         
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

void mod5() {
    printf("1) Для каждого предложения вывести строку образец, удовлетворяющую каждому слову в предложении. Строка условия содержит: символы, ? - 1 или больше любых символов, в начале и конце образца могут быть символы * - обозначающие 0 или больше символов. Например, для слов Аристотель и Артишок, строка образец будет иметь вид Ар???о?*.\n2) Удалить все предложения, в которых нет заглавных букв в начале слова.\n3) Отсортировать слова в предложении по количеству гласных букв в слове.\n4) Для каждого предложения вывести количество одинаковых слов в строке в формате Количество одинаковых слов: <число>, где <число> – кол-во одинаковых слов. Пример: курсовая сдана, курсовая сдана сдана. -> Количество одинаковых слов: 5 (потому что 2 раза курсовая и 3 раза сдана). Слова сравнивать без учёта регистра\n");
}

