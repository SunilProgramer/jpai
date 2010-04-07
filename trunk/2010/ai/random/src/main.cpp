#include<fstream>
#include<iostream>
#include<time.h>
#include<stdlib.h>

using namespace std;

struct TPoint
{
	int x, y;
};

int main()
{
	int tmp = time(NULL);
	ifstream fi; fi.open("input.txt", ios::in);
	ofstream fo; fo.open("output.txt", ios::out);
	int w, h, t;
	int l = 0;
	TPoint *tmparr = new TPoint[w*(h+1)];
	fi>>w>>h>>t>>l;
	l = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			fi>>t;
			if ((t/10000)%2 == 0)
			{
				tmparr[l].x = x + 1; 
				tmparr[l].y = y + 1;
				l++;
			}
			tmp = (tmp + t)%1000000; 
		}
	}
	srand(tmp);
	if (l != 0)
		l = rand()%l;
	fo<<tmparr[l].x<<" "<<tmparr[l].y<<" "<<rand()%4;
	delete[] tmparr;
	fi.close();
	fo.close();
	return 0;
}

