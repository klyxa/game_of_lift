#include <stdio.h>
#include <stdlib.h>

int min(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

struct bor{
	int len_x;
	int len_y;
	int pading;
	int index_bor;
	int *bor[2];
};

int* bor_inedx(struct bor br, int x, int y)
{
	if(x > br.len_x) fprintf(stderr, "x i to hige");
	if(y > br.len_y) fprintf(stderr, "y i to hige");
	return br.bor[br.index_bor] + (br.len_x+br.pading)*y + x + br.pading;
}

void bor_print(struct bor br)
{
	for(int i = 0; i < 2;i++)
	{
		for(int y = 0; y < br.len_y; y++)
		{
			for(int x = 0; x < br.len_x; x++)
				printf("%d", *bor_inedx(br, x, y));
			printf("\n");
		}
		printf("\nandet bro\n");
	}
}

void bor_print_index(struct bor br)
{
	printf("(x,y)\n");
	for(int y = 0; y < br.len_y; y++)
	{
		for(int x = 0; x < br.len_x; x++)
			printf("(%d,%d)\t", x, y);
		printf("\n");
	}
}

int init(struct bor *pbr)
{
	int len_x = 20;
	int len_y = 10;
	int pad = 1;
	struct bor br = {len_x, len_y, pad ,0 , {(int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int)), (int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int))}};
	if (br.bor[0] == NULL || br.bor[1] == NULL) {fprintf(stderr,"faild of alocade bord"); return 1;}
	*pbr = br;
	return 0;
}

int main()
{
	printf("hello\n");
	struct bor br;
	if( init(&br) ) {fprintf(stderr,"faild to inishiale"); return 1;}
	for(int i = 0; i < min(br.len_x, br.len_y); i++)
		*bor_inedx(br, i, i) = i;
	for(int i = 0; i < br.len_x; i++)
		*bor_inedx(br, i, 0) =1;
	bor_print(br);
        bor_print_index(br);
	printf("redy to work\n");
	return 0;
}
