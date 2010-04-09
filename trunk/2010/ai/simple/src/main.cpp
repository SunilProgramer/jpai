#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "../../inc/cell.h"
using namespace std;

int main()
{
	ifstream fi("input.txt", ios::in);
	ofstream fo("output.txt", ios::out);
	int w = 0, h = 0, i = 0;	
	fi >> w >> h >> h >> h;	
	while (!fi.eof())
	{
		Cell t;
		fi>>t;
		if (!t.isEmpty() && !t.Fixed)
			break;
		i++;
	}	
	srand(time(NULL));
	fo<<(i%w + 1)<<" "<<(i/w + 1)<<" "<<rand()%4;	
	fi.close();
	fo.close();	
	return 0;
}

