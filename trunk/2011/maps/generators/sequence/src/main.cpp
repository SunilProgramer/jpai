//todo: rewrite in c++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int PLAYERS_COUNT = 10;
void PrintHelp()
{
	printf("Usage: map_simple <output_file> <width> <height> <limit> <wsize> [count=width*height]\n");
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (argc < 6)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	int limit = atoi(argv[4]);
	int wsize = atoi(argv[5]);
	int count = w*h;
	if (argc > 6)
		count = atoi(argv[6]);
	FILE *fo = fopen(argv[1], "w");
	fprintf(fo, "%d %d %d %d %d\n", w, h, limit, wsize, count);

	for (int i = 0; i < PLAYERS_COUNT; i++)
		for (int j = 0; j < count; j++)
			fprintf(fo, "%d%s", j%limit + 1, j == count-1?"\n":" ");
	
	for (int i = 0; i < w * h; i++)
	{
		fprintf(fo, "0 0");
		if ((i + 1) % w == 0)
			fprintf(fo, "\n");
		else
		fprintf(fo, " ");
	}
	fclose(fo);
	return 0;
}
