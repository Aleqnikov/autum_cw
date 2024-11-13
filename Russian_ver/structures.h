#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <wchar.h>
typedef struct{
	wchar_t* string;
    int size; 
}Sentence;


typedef struct{
	Sentence *sentences;
    int count;       
    int capacity; 
}Text;

#endif // STRUCTURES_H