#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "structures.h"

void free_text(Text *text);
void del_tabulation(Text *text);
void first_read_text(char** text);
void convert_text(char** text, Text* cnv_txt);
void read_text(Text* cnv_txt);
bool check_double(Sentence checkd_sent, Sentence sent2);
void del_double(Text *text);