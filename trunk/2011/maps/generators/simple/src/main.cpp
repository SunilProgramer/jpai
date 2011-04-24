//todo: rewrite in c++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void PrintHelp()
{
	printf("Usage: map_simple <output_file> <width> <height> [limit=20]\n");
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (argc < 4)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	int limit = 20;
	if (argc > 4)
		limit = atoi(argv[4]);
	FILE *fo = fopen(argv[1], "w");
	fprintf(fo, "%d %d\n", w, h, limit, 0);
	
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
