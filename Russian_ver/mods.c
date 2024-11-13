#include "mods.h"
#include "text_tools.h"

#define BASE_LEN_WORDS 16


bool is_vowel(wchar_t symb){

    wchar_t letters[] = L"aeiouyAEIOUYаоуиэыяюеёАОУИЭЫЯЮЕЁ";

    if(wcsrchr(letters, symb) != NULL)
        return true;

    return false;

}

void cnt_vowel(Sentence* checkd_sent){
    int cnt = 0;

    for(int i = 0; i < wcslen(checkd_sent->string); i++){
        if(is_vowel(checkd_sent->string[i]))
            cnt++;
    }

    checkd_sent->size = cnt;
}

Sentence* split_sentense(wchar_t* sent, int mod) {
    wchar_t *word;
    wchar_t separations[200];

    if (mod == 1) {
        wcscpy(separations, L" ,.\n\t");
    } else {
        wcscpy(separations, L"ёйцукенгшщзхъфывапролджэячсмитьбюЁЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890~!@#$^&*-()_+{}:''<>\"");
    }

    wchar_t *token;
    wchar_t sentence_copy[wcslen(sent) + 1];  // +1 для \0
    wcscpy(sentence_copy, sent);

    word = wcstok(sentence_copy, separations, &token);

    int cnt_wds = 0;
    int lim_words = BASE_LEN_WORDS;
    Sentence* words = malloc(lim_words * sizeof(Sentence));
    chk_crr_memall(words);

    words[cnt_wds].string = malloc((wcslen(word) + 1) * sizeof(wchar_t));
    chk_crr_memall(words[cnt_wds].string);
    wcscpy(words[cnt_wds++].string, word);

    word = wcstok(NULL, separations, &token);
    while (word != NULL) {
        if (cnt_wds + 1 > lim_words) {
            lim_words *= 2;
            Sentence *temp = realloc(words, lim_words * sizeof(Sentence));
            
                
            chk_crr_memall(words);
                
            words = temp;
        }

        words[cnt_wds].string = malloc((wcslen(word) + 1) * sizeof(wchar_t));
        chk_crr_memall(words[cnt_wds].string);
        wcscpy(words[cnt_wds++].string, word);
        word = wcstok(NULL, separations, &token);
    }

    // Устанавливаем размер массива в первый элемент
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
    
        wchar_t mask[wcslen(words[0].string) + 2];

        wcscpy(mask, words[0].string);

        int mask_sb = wcslen(mask);

        for(int j = 1; j < cnt_wds; j++){
            if(wcslen(words[j].string) < mask_sb){
                mask_sb = wcslen(words[j].string);

                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != words[j].string[i])
                        mask[i] = L'?';
                }

                mask[mask_sb] = L'*';
                mask[mask_sb + 1] = L'\0';
            } else if (wcslen(words[j].string) == mask_sb){
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] !=words[j].string[i])
                        mask[i] = L'?';
                }
            } else{
                for(int i = 0; i < mask_sb; i++){
                    if(mask[i] != words[j].string[i])
                        mask[i] = L'?';
                }

                mask[mask_sb ] = L'*';
                mask[mask_sb + 1] = L'\0';
            }
        }   
        
        for(int l = 0; l < cnt_wds; l++){
            free(words[l].string);

        }
        free(words);
        
        wprintf(L"%ls\n",mask);
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

        const wchar_t delimiters[] = L" ,.\n\t"; // Разделители
        wchar_t *word;

        wchar_t *token;

        wchar_t sentence_copy[wcslen(text->sentences[i].string)];
        wcscpy(sentence_copy, text->sentences[i].string);

        word = wcstok(sentence_copy, delimiters, &token);

        int flag = false;

        while (word != NULL) {  

            if(towupper(word[0]) != word[0]){
                flag = true;
                break;
            }
                
            word = wcstok(NULL, delimiters, &token); 
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
            total_length += wcslen(words[j].string);
            if (j < cnt_marks) {
                total_length += wcslen(punctuation_marks[j].string);
            }
        }

        // Выделяем память для base
        wchar_t *base = (wchar_t *)malloc(total_length * sizeof(wchar_t));
        
        base[0] = '\0';  // Инициализируем пустую строку

        // Формируем строку base, объединяя слова и знаки препинания
        for (int j = 0; j < cnt_wds; j++) {
            wcscat(base, words[j].string);
            if (j < cnt_marks) {
                wcscat(base, punctuation_marks[j].string);
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

        wprintf(L"Количество одинаковых слов: %d\n", result);

        // Освобождаем память
        for(int m = 0; m < cnt_wds; m++)
            free(words[m].string);
        free(words);
    }


}

void mod5() {
    wprintf(L"1) Для каждого предложения вывести строку образец, удовлетворяющую каждому слову в предложении. Строка условия содержит: символы, ? - 1 или больше любых символов, в начале и конце образца могут быть символы * - обозначающие 0 или больше символов. Например, для слов Аристотель и Артишок, строка образец будет иметь вид Ар???о?*.\n2) Удалить все предложения, в которых нет заглавных букв в начале слова.\n3) Отсортировать слова в предложении по количеству гласных букв в слове.\n4) Для каждого предложения вывести количество одинаковых слов в строке в формате Количество одинаковых слов: <число>, где <число> – кол-во одинаковых слов. Пример: курсовая сдана, курсовая сдана сдана. -> Количество одинаковых слов: 5 (потому что 2 раза курсовая и 3 раза сдана). Слова сравнивать без учёта регистра\n");
}
