#include "mods.h"
#include "text_tools.h"


#define BASE_LEN_WORDS 16


void mod1(Text *text){

    for(int i = 0; i < text->count; i++){

        const char delimiters[] = " ,.\n\t"; // Разделители
        char *word;

        word = strtok(text->sentences[i].string, delimiters);

        char mask[strlen(word) + 2];

        strcpy(mask, word);

        int mask_sb = strlen(mask);
        word = strtok(NULL, delimiters); 

        while (word != NULL) {  
            /*
                Проверяет длину нового слова, если длина меньше нынешней маски 
                то длина маски становится длиной этого слова, затем поэлементно 
                сравниваем их, и в конец добавляем спец символы. Если их длина 
                равна то просто сравнимаем, а иначе добавляем доп символ в конец
                и проверям только первое какое то кол во символов
            */ 

            if(strlen(word) < mask_sb){
                mask_sb = strlen(word);

                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != word[i])
                        mask[i] = '?';
                }

                mask[mask_sb] = '*';
                mask[mask_sb + 1] = '\0';
            } else if (strlen(word) == mask_sb){
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != word[i])
                        mask[i] = '?';
                }
            } else{
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != word[i])
                        mask[i] = '?';
                }

                mask[mask_sb ] = '*';
                mask[mask_sb + 1] = '\0';
            }

    
            word = strtok(NULL, delimiters); 
        }
        
        puts(mask);
    }
}

void mod2(Text *text){
    
    for(int i = 0; i < text->count; i++){

        const char delimiters[] = " ,.\n\t"; // Разделители
        char *word;

        char sentence_copy[strlen(text->sentences[i].string)];
        strcpy(sentence_copy, text->sentences[i].string);

        word = strtok(sentence_copy, delimiters);

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

void mod4(Text *text){
    /*
        Динамический массив слов создаём, слово - новая структура, которая нигде
        больше не используется.
        Затем мы проходимся по этому списку, сверяя каждое конкретное слово с другим, 
        посимвольно, Если слово единственно, то удаляем его, и в конце концов мы узнаем
        количество единственных слов.
    */

   for(int i = 0; i < text->count; i++){
        // Создаём и заполняем динамический массив

        const char delimiters[] = " ,.\n\t"; // Разделители
        char *word;

        char sentence_copy[strlen(text->sentences[i].string)];
        strcpy(sentence_copy, text->sentences[i].string);

        word = strtok(sentence_copy, delimiters);

        int cnt_wds =  0;
        int lim_words = BASE_LEN_WORDS;
        Sentence*  words = malloc(BASE_LEN_WORDS * sizeof(Sentence));

        chk_crr_memall(words);

        words[cnt_wds].string = malloc(strlen(word)*sizeof(char));

        chk_crr_memall(words[cnt_wds].string);

        strcpy(words[cnt_wds++].string, word);

        word = strtok(NULL, delimiters); 
        while (word != NULL) {  

            if(cnt_wds + 1 > lim_words){
                lim_words*=2;
                words = realloc(words, lim_words* sizeof(Sentence));
                chk_crr_memall(words);
            }

            words[cnt_wds].string = malloc(strlen(word)*sizeof(char));
            chk_crr_memall(words[cnt_wds].string);
            strcpy(words[cnt_wds++].string, word);

            word = strtok(NULL, delimiters); 
        }

        int result = cnt_wds;
        for(int j = 0; j < cnt_wds; j++){
            int flag = 1;

            for(int k = 0; k < cnt_wds; k++){
                if(j != k && check_double(words[j], words[k])){
                    flag = 0;
                    break;
                }
            }
            result-=flag;

        }

        printf("Количество одинаковых слов: %d\n", result);


        // Освобождаем память
        for(int m = 0; m < cnt_wds; m++)
            free(words[m].string);
        free(words);
    }


}
void mod5() {
    printf("1) Для каждого предложения вывести строку образец, удовлетворяющую каждому слову в предложении. Строка условия содержит: символы, ? - 1 или больше любых символов, в начале и конце образца могут быть символы * - обозначающие 0 или больше символов. Например, для слов Аристотель и Артишок, строка образец будет иметь вид Ар???о?*.\n2) Удалить все предложения, в которых нет заглавных букв в начале слова.\n3) Отсортировать слова в предложении по количеству гласных букв в слове.\n4) Для каждого предложения вывести количество одинаковых слов в строке в формате Количество одинаковых слов: <число>, где <число> – кол-во одинаковых слов. Пример: курсовая сдана, курсовая сдана сдана. -> Количество одинаковых слов: 5 (потому что 2 раза курсовая и 3 раза сдана). Слова сравнивать без учёта регистра\n");
}

