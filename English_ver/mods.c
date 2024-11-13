#include "mods.h"
#include "text_tools.h"

#define BASE_LEN_WORDS 16

bool is_vowel(char symb){

    char letters[] = "aeiouyAEIOUY";

    if(strchr(letters, symb) != NULL)
        return true;

    return false;

}

void cnt_vowel(Sentence* checkd_sent){
    int cnt = 0;

    for(int i = 0; i < strlen(checkd_sent->string); i++){
        if(is_vowel(checkd_sent->string[i]))
            cnt++;
    }

    checkd_sent->size = cnt;
}

// Функция, которая разделяет сроку на слова и соединяет в единый динамичечкий массив
Sentence* split_sentense(char* sent, int mod){

    char *word;
    char separations[200];

    /*
        Изначально планировалось, что del должен передаваться в функцию, но при каких то предложениях, почему то
        именно функция strtok меняла значения в del, что старнно, пришлось идти на такие ухщрения.

        Как работает?
        Есть два режима - 1 просто разделения на слова, 0 - разделение на символы
        Создаётся копия изначалной строки, для того чтобы не менять изначальную
        Затем мы при помощи функции strtook считываем слова из строки и заносим в динамический массив.
        Возвращаем указатель на новую строку.

    */

    if(mod==1){
        strcpy(separations, " ,.\n\t");
    }else{
        strcpy(separations, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890~!@#$^&*-()_+{}:''<>\"");
    }
    
    char sentence_copy[strlen(sent)]; 
    strcpy(sentence_copy, sent);

    word = strtok(sentence_copy, separations);

    int cnt_wds =  0;
    int lim_words = BASE_LEN_WORDS;
    Sentence*  words = malloc(BASE_LEN_WORDS * sizeof(Sentence));

    chk_crr_memall(words);

    words[cnt_wds].string = malloc(strlen(word)*sizeof(char));

    chk_crr_memall(words[cnt_wds].string);

    strcpy(words[cnt_wds++].string, word);

    word = strtok(NULL, separations); 
    while (word != NULL) {  

        if(cnt_wds + 1 > lim_words){
            lim_words*=2;
            words = realloc(words, lim_words* sizeof(Sentence));
            chk_crr_memall(words);
        }
        words[cnt_wds].string = malloc(strlen(word)*sizeof(char));

        chk_crr_memall(words[cnt_wds].string);
        strcpy(words[cnt_wds++].string, word);
        word = strtok(NULL, separations); 
    }

    // Размер массива будем хранить в первом элементе, тк они не используются
    words[0].size = cnt_wds;
    return words;
        
}

// Функция для сортировки массива слов по количеству гласных
int compare(const void* a, const void* b) {
    Sentence* word_a = (Sentence*)a;
    Sentence* word_b = (Sentence*)b;
    return word_a->size - word_b->size;
}


void mod1(Text *text){

    /*
        Сначала разбиваем строку на слова, затем проходимся циклом по каждому слову, сравнивая его с маской
        И затем соответственно выводим маску
    */


    for(int i = 0; i < text->count; i++){

        Sentence*  words = split_sentense(text->sentences[i].string, 1);
        int cnt_wds =  words[0].size;
    
        char mask[strlen(words[0].string) + 2];

        strcpy(mask, words[0].string);

        int mask_sb = strlen(mask);

        for(int j = 1; j < cnt_wds; j++){
            if(strlen(words[j].string) < mask_sb){
                mask_sb = strlen(words[j].string);

                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != words[j].string[i])
                        mask[i] = '?';
                }

                mask[mask_sb] = '*';
                mask[mask_sb + 1] = '\0';
            } else if (strlen(words[j].string) == mask_sb){
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] !=words[j].string[i])
                        mask[i] = '?';
                }
            } else{
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != words[j].string[i])
                        mask[i] = '?';
                }

                mask[mask_sb ] = '*';
                mask[mask_sb + 1] = '\0';
            }
        }   
        
        puts(mask);
    }
}

void mod2(Text *text){

    /*
        Разделяет предложение на слова, и затем сразу проверяет Является ли первая буква заглавной
        Если да то меняет флаг.
        Затем после проверки каждого слова программа если состояние флага поменялось, удаляет текущее предложение

        split_sentence не используется, так как не нужен динамический массив.
    */
    
    for(int i = 0; i < text->count; i++){

        const char delimiters[] = " ,.\n\t"; // Разделители
        char *word;

        char sentence_copy[strlen(text->sentences[i].string)];
        strcpy(sentence_copy, text->sentences[i].string);

        word = strtok(sentence_copy, delimiters);

        int flag = false;

        while (word != NULL) {  

            if(islower(word[0])){
                flag = true;
                break;
            }
                
            word = strtok(NULL, delimiters); 
        }

        if(flag){

            // Сдвигаем предложения и очищаем память которую занимает удалённое предложение
            free(text->sentences[i].string);
            for(int k = i; k < text->count; k++){
                text->sentences[k] = text->sentences[k + 1];
            }
            i--;
            text->count--;
        }
    }
}

void mod3(Text *text){
    /*
        Будем использовать струкруту Sentense как word, где size - это будет количество грасных букв
        
        Сначала мы создаём динамический массив, считываем туда каждое слово, попутно считая гласные буквы, 
        затем мы сортируем массив по количеству гласных букв в каждом слове, а затем мы формируем
        новое предложение, добавляя попутно к словам знаки препинания.

        Знаки препинания мы достаём при помощи strtook в отдельный массив, как разделители используем буквы.
    */

    for(int i = 0; i < text->count; i++){
        // Создаём и заполняем динамический массив


        Sentence*  words = split_sentense(text->sentences[i].string, 1);
        int cnt_wds =  words[0].size;

        // Считаем количество гласных букв для каждого слова
        cnt_vowel(&words[cnt_wds - 1]);

        for(int k = 0; k < cnt_wds; k++)
            cnt_vowel(&words[k]);
        

        // Сортировка слов по количеству гласных
        qsort(words, cnt_wds, sizeof(Sentence), compare);

        // Получаем знаки препинания      
        Sentence* punctuation_marks = split_sentense(text->sentences[i].string, 0);
        int cnt_marks = punctuation_marks[0].size;


        // Считаем итоговую длину для отсортированной строки

        int total_length = 1;  // Начальная длина для символа '\0' в конце
        for (int j = 0; j < cnt_wds; j++) {
            total_length += strlen(words[j].string);
            if (j < cnt_marks) {
                total_length += strlen(punctuation_marks[j].string);
            }
        }

        // Выделяем память для base
        char *base = (char *)malloc(total_length * sizeof(char));
        
        base[0] = '\0';  // Инициализируем пустую строку

        // Формируем строку base, объединяя слова и знаки препинания
        for (int j = 0; j < cnt_wds; j++) {
            strcat(base, words[j].string);
            if (j < cnt_marks) {
                strcat(base, punctuation_marks[j].string);
            }
        }

       text->sentences[i].string = base;

        // Освобождаем память
        for(int l = 0; l < cnt_wds; l++){
            free(words[l].string);

        }
        free(words);

        for(int m = 0; m < cnt_marks; m++){
            free(punctuation_marks[m].string);
        }
        free(punctuation_marks);

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

        Sentence*  words = split_sentense(text->sentences[i].string, 1);
        int cnt_wds =  words[0].size;

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

