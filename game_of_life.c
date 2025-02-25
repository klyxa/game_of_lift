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
	return br.bor[br.index_bor] + (br.len_x+br.pading)*y + x + br.pading;
}

int* bor_inedx_alt(struct bor br, int x, int y)
{
	return br.bor[(br.index_bor + 1) % 2] + (br.len_x+br.pading)*y + x + br.pading;
}

void bor_to_noise(struct bor br)
{
	for(int y = 0; y < br.len_y; y++)
		for(int x = 0; x < br.len_x; x++)
			*bor_inedx(br, x, y) = rand() % 2;
}

void bor_print(struct bor br)
{
	for(int i = 0; i < 1;i++)
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
	int len_x = 10;
	int len_y = 10;
	int pad = 1;
	struct bor br = {len_x, len_y, pad ,0 , {(int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int)), (int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int))}};
	if (br.bor[0] == NULL || br.bor[1] == NULL) {fprintf(stderr,"faild of alocade bord"); return 1;}
	*pbr = br;
	return 0;
}

void bor_trin(struct bor br)
{
	for(int y = 0; y < br.len_y; y++)
		for(int x = 0; x < br.len_x; x++)
		{
			int na = 0; // antal naborg
			na += *bor_inedx(br, x-1, y-1);
			na += *bor_inedx(br, x-1, y);
			na += *bor_inedx(br, x-1, y+1);
			na += *bor_inedx(br, x, y-1);
			na += *bor_inedx(br, x, y);
			na += *bor_inedx(br, x, y+1);
			na += *bor_inedx(br, x+1, y-1);
			na += *bor_inedx(br, x+1, y);
			na += *bor_inedx(br, x+1, y+1);
			if(na == 3)
				*bor_inedx_alt(br, x, y) = 1;
			else if(na == 4)
				*bor_inedx_alt(br, x, y) = 1;
			else 
				*bor_inedx_alt(br, x, y) = 0;

		}
}

int main()
{
	printf("hello\n");
	struct bor br;
	if( init(&br) ) {fprintf(stderr,"faild to inishiale"); return 1;}
	*bor_inedx(br, 2,2) = 1;
	*bor_inedx(br, 2,3) = 1;
	*bor_inedx(br, 2,4) = 1;

	bor_print(br);
	printf("redy to work\n");
	for(int i = 0; i < 10; i++)
	{
		bor_trin(br);
		bor_print(br);
		br.index_bor = (br.index_bor + 1) % 2;
	}	
	return 0;
}
