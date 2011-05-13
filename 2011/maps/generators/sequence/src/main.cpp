//todo: rewrite in c++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
using namespace std;
const int PLAYERS_COUNT = 10;
const int RADIUS = 4;
void PrintHelp()
{
	printf("Usage: map_sequence <output_file> <width> <height> <limit> <wsize> <small> <peaks> <peaks_count> [count=width*height]\n");
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	if (argc < 9)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	int limit = atoi(argv[4]);
	int wsize = atoi(argv[5]);
	int small = atoi(argv[6]);
	int peaks = atoi(argv[7]);
	int peaks_count = atoi(argv[8]);
	int count = w*h;
	if (argc > 9)
		count = atoi(argv[9]);
	FILE *fo = fopen(argv[1], "w");
	fprintf(fo, "%d %d %d %d %d\n", w, h, limit, wsize, count);

	for (int i = 0; i < PLAYERS_COUNT; i++)
	{
		vector<int> v;
		v.resize(count);
		for (int j = 0; j < count; j++)
			v[j]  = rand()%small + 1;
		for (int j = 0; j < peaks_count; j++)
		{
			int peak_pos = rand()%count;
			//printf("%d\n", peak_pos);
			for (int k = peak_pos-RADIUS; k < peak_pos + RADIUS && k < count; k++)
			{
				v[k] = min(max(rand()%int(peaks*(1.0f - abs((float)(k - peak_pos)/RADIUS)) + 1), 1), 20);
			}
		}
		for (int j = 0; j < count; j++)
			fprintf(fo, "%d%s", v[j], j == count-1?"\n":" ");
	}
	fclose(fo);
	return 0;
}
