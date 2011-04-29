#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
	ifstream fi("input.txt", ios::in);
	ofstream fo("output.txt", ios::out);
	int w, h, mv, n, t;
	fi >> w >> h >> n >> t;
	for (int i = 0; i < n; i++)
		fi >> mv;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int t = 0;
			fi >> t;
			if (!t)
			{
				fo << (x+1) << " " << (y+1);
				fi.close();
				fo.close();
				return 0;
			}
			fi >> t;
		}
	}
	fo << "0 0";
	fi.close();
	fo.close();
	return 0;
}

