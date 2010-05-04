#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "../../../../ai/inc/cell.h" //!
using namespace std;

void PrintHelp()
{
	printf("Usage: blender <input_file_name> [output_file_name] [-s]\n You may not specify output file name. If so, temporary file \"<input_file_name>~\" will be used.\n If you do not want to replace original file with temp use -s flag.\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		PrintHelp();
		return 0;
	}
	ifstream fi(argv[1], ios::in);
	char *out = argv[2];
	bool changed = false;
	if (argc == 2 || strlen(out) == 0 || !strcmp(argv[argc - 1], "-s"))
	{
		int len = strlen(argv[1]);
		out = new char[len+2];
		strcpy(out, argv[1]);
		out[len++] = '~';
		out[len] = 0;
		changed = true;
	}
	ofstream fo(out, ios::out);
	srand(time(NULL));
	int w = 0, h = 0;
	fi >> w >> h;
	fo << w << " " << h << endl;
	for (int i = 0; i < w*h; i++)
	{
		Cell cell;
		int a;
		fi>>a;
		cell.GetFromInt(a);
		cell.Rotate(rand()%4);
		fo << cell.Value() << " ";
		if (i % w == 0 && i)
			fo << endl;
	}
	fi.close();
	fo.close();
	if (!argv[argc - 1] || strcmp(argv[argc - 1], "-s") && changed)
	{
		remove(argv[1]);
		rename(out, argv[1]);
	}
}
