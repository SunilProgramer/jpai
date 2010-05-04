#ifndef CELL_HEADER
#define CELL_HEADER
#include <istream>
#include <cstring>
using namespace std;
struct Cell
{
	int Player;
	bool Fixed;
	bool data[4];
	bool Up()
	{
		return data[0];
	}
	bool Right()
	{
		return data[1];
	}
	bool Down()
	{
		return data[2];
	}
	bool Left()
	{
		return data[3];
	}
	bool isEmpty()
	{
		return !(Up()||Right()||Down()||Left());
	}
	int Signature()
	{
		return (Player<<6) + (Fixed<<4) + Value();
	}
	int Value()
	{
		return (Up()<<3) + (Right()<<2) + (Down()<<1) + Left();
	}
	void GetFromInt(int val)
	{
		data[0] = val & 8;
		data[1] = val & 4;
		data[2] = val & 2;
		data[3] = val & 1;
	}
	void Rotate(int angle)
	{
		for (int i = 0; i < angle; i++)
		{
			bool l = Left();
			data[3] = data[2];
			data[2] = data[1];
			data[1] = data[0];
			data[0] = l;			
		}
	}
};

istream& operator >> (istream& is, Cell& cell)
{
	char s[20]; 
	int len = 0;	
	is>>s;
	len = strlen(s);	
	cell.Fixed	= s[len-5]=='1';
	cell.data[0]	= s[len-4]=='1';
	cell.data[1]	= s[len-3]=='1';
	cell.data[2]	= s[len-2]=='1';
	cell.data[3]	= s[len-1]=='1';
	s[len-5] = 0;
	cell.Player = atoi(s);
	return is;
}
#endif
