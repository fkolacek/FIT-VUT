#include "../errors.h"
#include "../gc.h"
#include "../strings.h"
#define COUNT 10

const char* MESSAGES[] = {
	[0]	= "Zero",
	[2] = "One",
	[3] = "Two",
	[4] = "Three",
	[5] = "Four",
	[6] = "Six",
	[7] = "Seven",
	[8] = "Eight",
	[9] = "Nine"	
};

int main(int argc, char* argv[]){
	
	STRING tmp[COUNT];
	int i;
	
	for(i = 0; i < COUNT; i++)
		strInit(&(tmp[i]));

	printf("Init done\n");
	
	for(i = 0; i < COUNT; i++){
		printf("Turn: %d\n", i);
		strInitRaw(&(tmp[i]), MESSAGES[i]);
		printf("End turn %d\n", i);
	}
	
	for(i = 0; i < COUNT; i++)
		strFree(&(tmp[i]));
	
	printf("Free done\n");
	
	return 1;
}