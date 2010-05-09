#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "../../inc/cell.h"
using namespace std;
Cell *tmparr;
bool *visited;
int w, h;
int rx = 0, ry = 0;
int maxscores = 0, angle = 0;
int tmax_angle = 0;
void Clear()
{
	for (int i = 0; i < w*h; i++)
		visited[i] = false;
}
int Fill(int x, int y, int dir)
{
	int i = x + w*y;
	if ((x < 0) || (y < 0) || (x >= w) || (y >= h) || visited[i])
		return 0;
	if (tmparr[i].Player > 1)
		return 0;
	if (!(tmparr[i].Value() & dir))
		return 0;
	visited[i] = true;
	int res = 1;//tmparr[i].Player > 1;
	if (tmparr[i].Up())
		res += Fill(x, y - 1, 2);
	if (tmparr[i].Left())
		res += Fill(x - 1, y, 4);
	if (tmparr[i].Right())
		res += Fill(x + 1, y, 1);
	if (tmparr[i].Down())
		res += Fill(x, y + 1, 8);
	return res;
}

int Step(int x, int y)
{
	if (tmparr[x + w*y].Fixed || !tmparr[x + w*y].Value())
		return -1;
	int tmax_scores = 0;
	tmax_angle = 0;
	int col = tmparr[x + w*y].Player;
	if (col > 1)
		tmparr[x + w*y].Player = 0;
	for (int i = 0; i < 4; i++)
	{
		Clear();
		int res = Fill(x, y, 15);
		if (res > tmax_scores)
		{
			tmax_angle = i;
			tmax_scores = res;
		}
		tmparr[x + w*y].Rotate(1);
	}
	tmparr[x + w*y].Player = col;
	return tmax_scores;
}

void Store(int x, int y, int scores)
{
	rx = x;
	ry = y;
	angle = tmax_angle;
	maxscores = scores;
}

int main()
{
	ifstream fi("input.txt", ios::in);
	ofstream fo("output.txt", ios::out);
	ofstream fto("temp.txt", ios::out);
	int l = 0;
	fi >> w >> h >> l >> l;
	tmparr = new Cell[w*h];
	for (int i = 0; i < w*h; i++)
	{
		fi >> tmparr[i];
	}
	visited = new bool[w*h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			int scores = Step(x, y);
			fto << (x+1) << " " << (y+1) << " " << scores << endl;
			if (scores >= maxscores)
				Store(x, y, scores);
		}
	fo<<(rx+1)<<" "<<(ry+1)<<" "<<angle;
	fi.close();
	fo.close();
	fto.close();
	return 0;
}
