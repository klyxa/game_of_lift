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
	return br.bor[br.index_bor] + (br.len_x+br.pading*2)*(y + br.pading) + x + br.pading;
}

int* bor_inedx_alt(struct bor br, int x, int y)
{
	return br.bor[(br.index_bor + 1) % 2] + (br.len_x+br.pading*2)*(y + br.pading) + x + br.pading;
}

int gem_til_img(struct bor br ,char *fil_sti, int i)
{
	char file_name[30]; 
	sprintf(file_name, fil_sti, i);
	FILE *fp = fopen(file_name,"wb");
	if(fp == NULL) {fprintf(stderr,"fejel i at skrive til %s \n", file_name); return 1;}

	fprintf(fp,"P4\n%d %d\n",br.len_x ,br.len_y);

 	size_t buff_size =  br.len_x*br.len_y;
	unsigned char *buff = (unsigned  char*)malloc(buff_size);

	for(int y = 0; y < br.len_y; y++)
		for(int x = 0; x < br.len_x; x++)
		{
			int index = (br.len_x*y + x)/8;
			buff[index] = buff[index] << 1;
			if(*bor_inedx(br, x, y))
				buff[index]++;
		}
	fwrite(buff, sizeof(unsigned char), buff_size/sizeof(unsigned char), fp);
	fclose(fp);
	return 0;
}

int gem_til_img_ascci(struct bor br ,char *fil_sti, int i)
{
	char file_name[20]; 
	sprintf(file_name, fil_sti, i);
	FILE *fp = fopen(file_name,"w");
	if(fp == NULL) {fprintf(stderr,"fejel i at skrive til %s \n", file_name); return 1;}
	fprintf(fp,"P1\n%d %d\n",br.len_x ,br.len_y);
	for(int y = 0; y < br.len_y; y++)
		for(int x = 0; x < br.len_x; x++)
			fprintf(fp,"%d", *bor_inedx(br, x, y));
	fclose(fp);
	return 0;
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

int init(struct bor *pbr, int len_x, int len_y)
{
	int pad = 1;
	struct bor br = {len_x, len_y, pad ,0 , {(int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int)), (int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int))}};
	if (br.bor[0] == NULL || br.bor[1] == NULL) {fprintf(stderr,"faild of alocade bord"); return 1;}
	*pbr = br;
	return 0;
}

void bor_free_bor(struct bor *br)
{
	free(br->bor[0]);
	free(br->bor[1]);
	br->bor[0] = NULL;
	br->bor[1] = NULL;
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
			na += *bor_inedx(br, x, y+1);
			na += *bor_inedx(br, x+1, y-1);
			na += *bor_inedx(br, x+1, y);
			na += *bor_inedx(br, x+1, y+1);
			if( *bor_inedx(br, x, y))
			{
				if(na == 2)
					*bor_inedx_alt(br, x, y) = 1;
				else if(na == 3)
					*bor_inedx_alt(br, x, y) = 1;
				else 
					*bor_inedx_alt(br, x, y) = 0;
			}
			else
			{
				if(na == 3)
					*bor_inedx_alt(br, x, y) = 1;
				else 
					*bor_inedx_alt(br, x, y) = 0;
			}

		}
}

int main()
{
	struct bor br;
	if( init(&br, 192, 108) )
	{
		fprintf(stderr,"faild to inishiale");
		return 1;
	}
	bor_to_noise(br);

	int n = 100;
	for(int i = 0; i < n; i++)
	{
		printf("%d af %d \r", i, n);
    	fflush(stdout);
		bor_trin(br);
		gem_til_img(br, "img/img_%d.pbm", i);
		br.index_bor = (br.index_bor + 1) % 2;
	}	
	printf("\n");
	bor_free_bor(&br);
	return 0;
}
