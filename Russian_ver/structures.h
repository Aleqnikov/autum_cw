#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <wchar.h>
#include <stdbool.h>

typedef struct{
	wchar_t* string;
    int size; 
}Sentence;


typedef struct{
	Sentence *sentences;
    int count;       
    int capacity; 
    bool dot_in_last_sent;
}Text;

#endif // STRUCTURES_H