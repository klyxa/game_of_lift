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

struct xy{
	int x;
	int y;
};


int bor_granse_tjek(struct bor br, int x, int y)
{
	if(x < 0-br.pading)
		return 0;
	if(y < 0-br.pading)
		return 0;
	if(x > br.len_x+br.pading)
		return 0;
	if(y > br.len_y+br.pading)
		return 0;
	return 1;
}

int* bor_inedx(struct bor br, int x, int y)
{
	return br.bor[br.index_bor] + (br.len_x+br.pading*2)*(y + br.pading) + x + br.pading;
}

int* bor_inedx_alt(struct bor br, int x, int y)
{
	return br.bor[(br.index_bor + 1) % 2] + (br.len_x+br.pading*2)*(y + br.pading) + x + br.pading;
}

struct xy bor_inedx_invers(struct bor br, int index)
{
	struct xy i;// = {-1, -1};
	i.y = index / br.len_x;
	i.x = index % br.len_x;
	return i;
}

int init(struct bor *pbr, int len_x, int len_y)
{
	int pad = 1;
	struct bor br = {len_x, len_y, pad ,0 , {(int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int)), (int*)malloc((len_x + pad*2)*((len_y + pad*2))*sizeof(int))}};
	if (br.bor[0] == NULL || br.bor[1] == NULL) {fprintf(stderr,"faild of alocade bord"); return 1;}
	*pbr = br;
	return 0;
}

void read_ascci_pbm_img_stream (struct bor br , FILE *fp)
{
	for(int y = 0; y < br.len_y; y++)
		for(int x = 0; x < br.len_x; x++)
		{
			int h;
			do
			{
				h = fgetc(fp);
				//putchar(h);
			}while(h != '0' && h != '1' && h != EOF);	
			*bor_inedx(br, x, y) = (h == '0');
		}
}

// img in p4 .pbm
void read_bin_pbm_img_stream (struct bor br , FILE *fp)
{
	int ch;
	int i = 0;
	for(int y = 0; y < br.len_y; y++)
	{
		ch = fgetc(fp);
		i = 0;
		for(int x = 0; x < br.len_x -  1; x++)
		{
			*bor_inedx(br, x, y) = 0 < (ch & (1 << (7 - i)));

			i++;
			if(i == 8)
			{
				ch = fgetc(fp);
				i = 0;
			}
		}
	}

}

int lode_fra_img(struct bor *br ,char *file_name)
{
	printf("%s\n",file_name);

	FILE *fp = fopen(file_name,"r");
	if(fp == NULL) {fprintf(stderr,"kumme ikke finde %s \n", file_name); return 1;}
	int type = 0;
	int aug;
	do
	{
		aug = fscanf(fp, "P%d",&type);
	}while(aug != 1 && aug != EOF);
	if(aug == EOF) {fprintf(stderr,"ugyldi Portable BitMap(.pbm) kunne ikke finde typpen fx som P1 og P4 mm. i filen %s \n", file_name); return 2;}

	printf("P%d  \n-d\n",type);

	//find img size
	int x = 0;
	int y = 0;
	
	char *s = NULL;
        size_t len = 0;
	ssize_t nread;

	while ((nread = getdelim(&s, &len, '\n', fp)) != EOF)
		if(s[0] != '#')
			if (sscanf(s, "%d %d", &x, &y) == 2)
				  break;
	free(s);
	if(nread == EOF) {fprintf(stderr,"ugyldi Portable BitMap(.pbm) kunne ikke finde størlse fx som 1920 1080 og 100 100 mm. i filen %s \n", file_name); return 2;}

	if( init(br, x, y) ){fprintf(stderr,"faild to inishiale brat størlse %d %d i filen %s \n ", x, y, file_name); return 3;}

	switch (type)
	{
		case 1:
			//printf("P1, the values 0 and 1 (white & black) ascii \nstørlse %d %d\n", x, y);
			read_ascci_pbm_img_stream(*br, fp);
			break;
		case 4:
			//printf("P4, the values 0 and 1 (white & black) Binary\n");
			read_bin_pbm_img_stream(*br, fp);
			//printf("størlse %d %d\n", x, y);
			break;

		case 2:
		case 3:
		case 5:
		case 6:
		case 7:

		default:
			printf("denne typpe er ikke implatedser i nu\nP%d\n %d %d\n",type, x, y);
			return 4;
	}
	
	return 0;
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
	free(buff);
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
			if( 1 == *bor_inedx(br, x, y))
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

int main(int argc, char *argv[])
{
	struct bor br;
	if(argc > 1)
	{
		if(lode_fra_img(&br, argv[1]))
			return 1;
	}
	else
	{
		if( init(&br, 192, 108) )
		{
			fprintf(stderr,"faild to inishiale");
			return 1;
		}
		bor_to_noise(br);

	}
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
