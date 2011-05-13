//todo: rewrite in c++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;
const int PLAYERS_COUNT = 10;
void PrintHelp()
{
	printf("Usage: map_simple <output_file> <width> <height> <limit> <wsize> <player_offset> <[count=width*height]\n");
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (argc < 7)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	int limit = atoi(argv[4]);
	int wsize = atoi(argv[5]);
	int player_offset = atoi(argv[6]);
	int count = w*h;
	if (argc > 7)
		count = atoi(argv[7]);
	FILE *fo = fopen(argv[1], "w");
	fprintf(fo, "%d %d %d %d %d\n", w, h, PLAYERS_COUNT, wsize, count);
	vector<int> v;
	v.resize(count);
	for (int i = 0; i < PLAYERS_COUNT; i++)
		for (int j = 0; j < count; j++)
		{
			int d = rand()%limit + 1;
			if (!i)
				v[j] = d;
			fprintf(fo, "%d%s", player_offset?v[(j + player_offset*i)%count]:d, j == count-1?"\n":" ");
		}
	fclose(fo);
	return 0;
}
