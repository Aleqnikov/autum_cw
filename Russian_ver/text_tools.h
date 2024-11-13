#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <wchar.h>
#include <locale.h>
#include <wctype.h>


#include "structures.h"

void free_text(Text *text);
void del_tabulation(Text *text);
void first_read_text(wchar_t** text);
void convert_text(wchar_t** text, Text* cnv_txt);
void read_text(Text* cnv_txt);
bool check_double(Sentence checkd_sent, Sentence sent2);
void del_double(Text *text);
void text_output(Text *text);
void chk_crr_memall(void *ptr);