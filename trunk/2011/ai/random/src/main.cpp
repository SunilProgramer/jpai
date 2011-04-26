#include <fstream>
#include <time.h>
#include <stdlib.h>
using namespace std;

struct TPoint
{
	int x, y;
};

int main(int argc, char *argv[])
{
	int tmp = time(NULL);
	ifstream fi("input.txt", ios::in);
	ofstream fo("output.txt", ios::out);
	int w, h, mv, n;
	fi >> w >> h >> n;
	TPoint *tmparr = new TPoint[w*h];
	for (int i = 0; i < n; i++)
		fi >> mv;
	n = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int t = 0;
			fi >> t;
			if (!t)
			{
				tmparr[n].x = x + 1;
				tmparr[n++].y = y + 1;
			}
			fi >> t;
			tmp = (tmp + t)%0xfffffff; 
		}
	}
	srand(tmp);
	if (n != 0)
		n = rand()%n;
	fo << tmparr[n].x << " " << tmparr[n].y;
	delete[] tmparr;
	fi.close();
	fo.close();
	return 0;
}

