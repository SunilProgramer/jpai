#include <fstream>
#include <time.h>
#include <stdlib.h>
#include "../../inc/cell.h"
using namespace std;

struct TPoint
{
	int x, y;
};

int main()
{
	int tmp = time(NULL);
	ifstream fi("input.txt", ios::in);
	ofstream fo("output.txt", ios::out);
	int w, h, l;
	fi >> w >> h >> l >> l;
	TPoint *tmparr = new TPoint[w*h];
	l = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Cell t;
			fi>>t;
			if (!t.isEmpty() && !t.Fixed)
			{
				tmparr[l].x = x + 1;
				tmparr[l++].y = y + 1;
			}
			tmp = (tmp + t.Signature())%1000000; 
		}
	}
	srand(tmp);
	if (l != 0)
		l = rand()%l;
	fo << tmparr[l].x << " " << tmparr[l].y << " " << rand()%4;
	delete[] tmparr;
	fi.close();
	fo.close();
	return 0;
}

