#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct{
	char* string;
}Sentence;


typedef struct{
	Sentence *sentences;
    int count;       
    int capacity; 
}Text;


#endif // STRUCTURES_H