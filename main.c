#include <stdio.h>

#include "mods.h"
#include "read_text.h"




int main(void){
	printf("Course work for option 5.3, created by Ivan Aleinikov.\n");


	int mode;
	scanf("%d", &mode);
    getchar();

    char *text = NULL;

	switch (mode)
	{
	case 1:
		read_text(&text);
        puts(text);
		break;
	case 2:
		
		break;
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:
		break;
	
	default:
		printf("Error: ошибка ввода, неправильный режим.");
	}


	return 0;
}
