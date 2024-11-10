#include <stdio.h>

#include "mods.h"
#include "text_tools.h"
#include "structures.h"




int main(void){
	printf("Course work for option 5.3, created by Ivan Aleinikov.\n");

	int mode;
	scanf("%d", &mode);
    getchar();
	
    Text result;

	switch (mode)
	{
	case 1:
		
    	read_text(&result);

		mod1(&result);

		free_text(&result);
		break;
	case 2:
		read_text(&result);

		mod2(&result);

		text_output(&result);

		free_text(&result);
		break;
	case 3:
		
		break;
	case 4:
		read_text(&result);

		mod4(&result);


		free_text(&result);
		break;
	case 5:
		mod5();
		break;
	
	default:
		printf("Error: ошибка ввода, неправильный режим.");
	}

	return 0;
}
