#define BASE_LEN_TEXT 10
#define BASE_LEN_SENT 50

typedef struct{
	char string[BASE_LEN_SENT];
}Sentence;


typedef struct{
	Sentence* sentences[BASE_LEN_TEXT];
}Text;

void read_text(char **text);

