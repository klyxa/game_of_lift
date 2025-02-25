#include <stdio.h>

struct {
	int len_x;
	int len_y;
	int ind_bord;
	int bord[2][len_x][len_y];
}

int init()
{
	int Len_x = 20;
	int len_y = 10;

	return 0;
}

int main()
{
	printf("hello\n");
	if( init() )
		fprintf(stderr,"faild to inishiale");

	printf("redy to work\n");
	return 0;
}
