#include "text_tools.h"

#define SIZE_BUFFER   11
#define START_TEXT_SIZE 32  

#define BASE_LEN_TEXT 10
#define BASE_LEN_SENT 50

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
        wprintf(L"Error: память не была выделена (NULL указатель).\n");
        exit(EXIT_FAILURE); // Завершите программу с ошибкой
    } 
}

// Вывод текста
void text_output(Text *text){
    if(text->dot_in_last_sent)
        text->sentences[text->count - 1].string[wcslen(text->sentences[text->count - 1].string) - 1] = L'\0';
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

    // Переменная для проверки корректности выделенной памяти
    wchar_t* tmp_t = malloc(size * sizeof(wchar_t));   
    chk_crr_memall(tmp_t);    
    *text = tmp_t;
    

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
            tmp_t = realloc(*text, size * sizeof(wchar_t));
            chk_crr_memall(tmp_t);
            *text = tmp_t;
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
    cnv_txt->dot_in_last_sent =  (*(wcsrchr(*text, L'\0') - 1)) != L'.';

    Sentence* temp = malloc(BASE_LEN_TEXT * sizeof(Sentence));

    chk_crr_memall(temp);
    // Инициализируем массив указателей на предложения
    cnv_txt->sentences = temp;

    
    wchar_t* tmp_w = malloc(BASE_LEN_SENT * sizeof(wchar_t)); 
    chk_crr_memall(tmp_w);
    // Инициализируем превую строку
    cnv_txt->sentences[0].string = tmp_w;

    
     
    cnv_txt->count = 0;
    cnv_txt->capacity = BASE_LEN_TEXT;
    cnv_txt->sentences[0].size = BASE_LEN_SENT;

    int local_len_sent = 0;

    for(int i = 0; (*text)[i] != L'\0'; i++){
        

        // Проверяем есть ли место в предложении под символ, если нет то динамически расширяем
        if(local_len_sent + 2 >= cnv_txt->sentences[cnv_txt->count].size ){
            cnv_txt->sentences[cnv_txt->count].size*= 2;

            tmp_w = realloc(cnv_txt->sentences[cnv_txt->count].string, cnv_txt->sentences[cnv_txt->count].size * sizeof(wchar_t));
            chk_crr_memall(cnv_txt->sentences[cnv_txt->count].string);

            cnv_txt->sentences[cnv_txt->count].string = tmp_w;  
        }

        // Проверяем, есть ли место в тексте, если нет то динамически расширяем
        if(cnv_txt->count + 2>= cnv_txt->capacity){
            cnv_txt->capacity*= 2;
            temp = realloc(cnv_txt->sentences, cnv_txt->capacity* sizeof(Sentence));

            chk_crr_memall(temp);

            cnv_txt->sentences = temp;    
        }


        if((*text)[i] == L'.'){
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = L'.';
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent + 1] = L'\0';

            // Если конец предложения, то обнуляем прошлые переменные
            local_len_sent = 0;

            cnv_txt->count++;
            cnv_txt->sentences[cnv_txt->count].size = BASE_LEN_SENT;
            tmp_w = malloc(BASE_LEN_SENT * sizeof(wchar_t)); 
            chk_crr_memall(tmp_w);

            cnv_txt->sentences[cnv_txt->count].string = tmp_w;
            
        }else{
            cnv_txt->sentences[cnv_txt->count].string[local_len_sent] = (*text)[i];
            local_len_sent++;
        }
    }
    


    if(cnv_txt->dot_in_last_sent){
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
