#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include "../../../../ai/inc/cell.h" //!
using namespace std;

void PrintHelp()
{
	printf("Usage: spiral <file_name> <width> <height>\n");
}
const int hor = 5;
const int vert = 10;
const int upright = 12;
const int rightdown = 6;
const int downleft = 3;
const int leftup = 9;

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		PrintHelp();
		return 0;
	}
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);
	ofstream fo(argv[1], ios::out);
	fo << w << " " << h << endl;
	int limit = min(w/2, h/2);
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int out = 0; 
			if ( x == y-1 && x < limit)
				out = rightdown;
			if ( w - x == h - y && x >= w - limit - ((w%2) || (h%2)))
				out = leftup;
			if ( x == h - y - 1 && x < limit)
				out = upright;
			if ( w - x - 1 == y && x >= w - limit - ((w%2) || (h%2)))
				out = downleft;
			if (!out)
			{
				if (y <= h/2 && ((x > y - 1) && (w - x - 1) > y))
					out = hor;
				if (y >= h/2 && ((x > h - y - 1) && w - x > h - y))
					out = hor;
				if (x < w/2 && ((x < y - 1) && (x < h - y - 1)))
					out = vert;
				if (x >= w/2 && ((w - x - 1) < y && w - x < h - y))
					out = vert;
			}
			fo << out << " ";
		}
		fo << endl;
	}
	fo.close();
	return 0;
}
