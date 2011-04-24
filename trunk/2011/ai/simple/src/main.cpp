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
	fi >> w >> h >> mv >> n;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int t = 0;
			fi >> t;
			if (!t)
			{
				srand(tmp);
				fo << (x+1) << " " << (y+1) << " " << (rand()%mv + 1);
				fi.close();
				fo.close();
			}
			tmp = (tmp + t)%0xfffffff; 
			fi >> t;
			tmp = (tmp + t)%0xfffffff; 
		}
	}
	fo << "0 0 1";
	fi.close();
	fo.close();
	return 0;
}

