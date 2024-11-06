#include "structures.h"

void free_text(Text *text);
void del_tabulation(Text *text);
void first_read_text(char** text);
void convert_text(char** text, Text* cnv_txt);
void read_text(Text* cnv_txt);