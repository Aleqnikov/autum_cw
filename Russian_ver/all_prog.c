#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

#include <wchar.h>
#include <locale.h>
#include <wctype.h>

#define BASE_LEN_WORDS 16


#define SIZE_BUFFER   11
#define START_TEXT_SIZE 32  

#define BASE_LEN_TEXT 10
#define BASE_LEN_SENT 50

typedef struct{
	wchar_t* string;
    int size; 
}Sentence;


typedef struct{
	Sentence *sentences;
    int count;       
    int capacity; 
}Text;



/*
    Предполагается, что в других местах программы будет использоваться только read_text.

    Все они второстепенные и нужны для модификаций главной функции, которая собственно и читает текст. Она должная быть в каждом режиме для 
    чтения текста. - read_text
    Функции данного файла преднозначены для первичной обработки текста, введенного в консоль.
    Сначала функция first_read_text считывает ввод в поток stdin и заполняет массив, динамически его расширяя.
    Затем полученный текст мы конвертируем из простого массива, в структуру Text, у которой есть поля - sentences
    которое является массивом структур sentence, в которых и хранятся предложения, count - количество предложений которые 
    уже есть в тексте, и capacity - текущая ёмкость текста.
    А затем просходит удаление лишних пробелов перед началом предложения.

    После этого функция dell_double удаляет предложения, которые уже есть в тексте.
    Она же в свою очередь оперирует функцией check_double, которая посимвольно сравнивает два введённых предложения

    В свою очередь очистка памяти должна проходить после того как отработает модуль
*/

bool check_double(Sentence checkd_sent, Sentence sent2){

    /*
    Несколько проверок, первая - если отличается длина, то сразу нет
    А затем посимволно, и если хоть один символ отличается, то нет
    */

   if(wcslen(checkd_sent.string) != wcslen(sent2.string))
        return false;
   

    for(int i = 0; i < wcslen(checkd_sent.string); i++){
        if(towlower(checkd_sent.string[i]) != towlower(sent2.string[i]))
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
                free(text->sentences[i].string);
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

// Проверяет правильно ли выделена память
void chk_crr_memall(void *ptr) {
    if (ptr == NULL) {
        wprintf(L"Ошибка: память не была выделена (NULL указатель).\n");
        exit(EXIT_FAILURE); // Завершите программу с ошибкой
    } 
}

// Вывод текста
void text_output(Text *text){
    for (size_t i = 0; i < text->count; i++) 
        wprintf(L"%ls\n", text->sentences[i].string);
}
// Освобождает выделенную память

void free_text(Text *text) {
    for (size_t i = 0; i < text->count; i++) {
        free(text->sentences[i].string); // Освобождаем каждую строку 
    }
    free(text->sentences); // Освобождаем массив предложений
}

// Удаляет лишние символы до начала предложения
void del_tabulation(Text *text){

    for(int i = 0; i < text->count; i++){
        int shift = 0;
        while(iswspace(text->sentences[i].string[shift]) != 0)
            shift++;
        for(int j = 0; j < wcslen(text->sentences[i].string); j++) 
            text->sentences[i].string[j] = text->sentences[i].string[j+shift];  
    }  
}

// Первичное считывание текста из stdin

void first_read_text(wchar_t** text) {
    /*
        Создаёт строку, затем считывает порционно из потока stdin, проверяет, есть ли \n\n,
        если есть завершает считывание, если нет, то проверяет, есть ли место в строке, если его нет,
        то увеличивает размер, иначе заносит данные внутрь.
    */

    size_t size = START_TEXT_SIZE; 
    int len = 0;             
    *text = malloc(size * sizeof(wchar_t));       
    chk_crr_memall(*text);

    // Строка - буфер
    wchar_t local_string[SIZE_BUFFER]; 

    while (true) {
        if (fgetws(local_string, SIZE_BUFFER, stdin) == NULL)
            break; 
        
        // Проверка для конца текста
        if (wcscmp(local_string, L"\n") == 0) {
            if (len > 0 && (*text)[len - 1] == L'\n') {
                break; 
            }
        }

        // Если длина текста превышает размер массива, увеличиваем массив
        if (len + wcslen(local_string) >= size) {
            size *= 2; 
            wchar_t *temporarily = realloc(*text, size * sizeof(wchar_t));
            chk_crr_memall(temporarily);
            *text = temporarily;
        }

        // Копируем строку в выделенную память
        wcscpy(*text + len, local_string);
        len += wcslen(local_string);
    }

    // Устанавливаем финальный символ конца строки, если необходимо
    if (len > 0 && (*text)[len - 1] != L'\n') {
        (*text)[len] = L'\0';
    } else if (len > 0) {
        (*text)[len - 1] = L'\0'; 
    }
}


void convert_text(wchar_t** text, Text* cnv_txt){

    /*
        Первым делом проверям, есть ли точка в последнем предложении.
        Сначала в поле структуры sentences выделяет место под массив структур sentences.
        затем инициализирует первую строку. Создаёт локальные переменные.
        После этого поэтапно считывает символы из первоначально введенного массива, проверят его.
        Проверяет есть ли место предложении, есть ли место в тексте для нового предложения, и только потом
        если символ является точкой, то завершает предложение и идет на следующее, если символ не точка, то 
        просто заносит его в текущее предложение.
    */

   // Проверяем есть ли точка
    bool dot_in_st = (*(wcsrchr(*text, L'\0') - 1)) != L'.';

    // Инициализируем массив указателей на предложения
    cnv_txt->sentences = malloc(BASE_LEN_TEXT * sizeof(Sentence));

    // Инициализируем превую строку
    cnv_txt->sentences[0].string = malloc(BASE_LEN_SENT * sizeof(wchar_t)); 
     
    cnv_txt->count = 0;
    cnv_txt->capacity = BASE_LEN_TEXT;
    cnv_txt->sentences[0].size = BASE_LEN_SENT;

    int local_len_sent = 0;

    for(int i = 0; (*text)[i] != L'\0'; i++){
        

        // Проверяем есть ли место в предложении под символ, если нет то динамически расширяем
        if(local_len_sent + 2 >= cnv_txt->sentences[cnv_txt->count].size ){
            cnv_txt->sentences[cnv_txt->count].size*= 2;
            cnv_txt->sentences[cnv_txt->count].string = realloc(cnv_txt->sentences[cnv_txt->count].string, cnv_txt->sentences[cnv_txt->count].size * sizeof(wchar_t));

            chk_crr_memall(cnv_txt->sentences[cnv_txt->count].string);
        }

        // Проверяем, есть ли место в тексте, если нет то динамически расширяем
        if(cnv_txt->count + 2>= cnv_txt->capacity){
            cnv_txt->capacity*= 2;
            cnv_txt->sentences = realloc(cnv_txt->sentences, cnv_txt->capacity* sizeof(Sentence));

            chk_crr_memall(cnv_txt->sentences);
        }


        if((*text)[i] == L'.'){
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = L'.';
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent + 1] = L'\0';

            // Если конец предложения, то обнуляем прошлые переменные
            local_len_sent = 0;

            cnv_txt->count++;
            cnv_txt->sentences[cnv_txt->count].size = BASE_LEN_SENT;
            cnv_txt->sentences[cnv_txt->count].string = malloc(BASE_LEN_SENT * sizeof(wchar_t)); 
        }else{
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            local_len_sent++;
        }
    }
    


    if(dot_in_st){
        // Последнее предложение тоже нужно учесть, если оно не заканчивается точкой     
        cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = L'.';
        cnv_txt->sentences[cnv_txt->count].string[local_len_sent  + 1] = L'\0';

        cnv_txt->count++; 
    } else {
        free(cnv_txt->sentences[cnv_txt->count].string);  // Убираем пустую строку, если она создана
    }

}

// Собирает все функции воедино и апперирует ими
void read_text(Text* cnv_txt){

    wchar_t *text = NULL;

    // Обработка текста и чтение его
    first_read_text(&text);

    convert_text(&text, cnv_txt);
    del_tabulation(cnv_txt);
    del_double(cnv_txt);

    free(text);
}

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


int main(void){
    setlocale(LC_ALL, "");
	wprintf(L"Course work for option 5.3, created by Ivan Aleinikov.\n");

	int mode;
	wscanf(L"%d", &mode);
    getchar();
	
    Text result;

	switch (mode)
	{
	case 1:
		
    	read_text(&result);

		mod1(&result);

		free_text(&result);
		break;
	case 2:
		read_text(&result);

		mod2(&result);

		text_output(&result);

		free_text(&result);
		break;
	case 3:
		read_text(&result);
		mod3(&result);

		text_output(&result);

		free_text(&result);
		break;
	case 4:
		read_text(&result);

		mod4(&result);


		free_text(&result);
		break;
	case 5:
		mod5();
		break;
	
	default:
		wprintf(L"Error: ошибка ввода, неправильный режим.\n");
	}

	return 0;
}