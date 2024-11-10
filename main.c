#include <stdio.h>

#include "mods.h"
#include "text_tools.h"
#include "structures.h"




int main(void){
	printf("Course work for option 5.3, created by Ivan Aleinikov.\n");


	int mode;
	scanf("%d", &mode);
    getchar();

    

	switch (mode)
	{
	case 1:
        Text result;
        read_text(&result);

		free_text(&result);
		break;
	case 2:
		
		break;
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:
		mod5();
		break;
	
	default:
		printf("Error: ошибка ввода, неправильный режим.");
	}

	return 0;
}
