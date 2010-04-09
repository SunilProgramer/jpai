#include <stdio.h>
#include <stdlib.h>
void PrintHelp()
{
	printf("Usage: pc_random <output_file> <width> <height>\n");
}
int main( int argc, char *argv[])
{
	if (argc != 4)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	FILE *fo = fopen(argv[1], "w");
	fprintf(fo, "%d %d\n", w, h);
	
	for (int i = 0; i < w * h; i++)
	{
		fprintf(fo, "%d", rand()%16);
		if ((i + 1) % w == 0)
			fprintf(fo, "\n");
		else
		fprintf(fo, " ");
	}
	fclose(fo);
	return 0;
}
