#ifndef CELL_HEADER
#define CELL_HEADER
#include<istream>
using namespace std;
struct Cell
{
	int Player;
	bool Fixed;
	bool Up;
	bool Right;
	bool Down;
	bool Left;
	bool isEmpty()
	{
		return !(Up||Right||Down||Left);
	}
	int Signature()
	{
		return (((((((Up << 1) | Right) << 1) | Down) << 1) | Left) << 1) | 
		  (Player << 6)|(Fixed << 5);
	}
};

istream& operator >> (istream& is, Cell& cell)
{
	char s[20]; 
	int len = 0;	
	is>>s;
	len = strlen(s);	
	cell.Fixed	= s[len-5]=='1';
	cell.Up		= s[len-4]=='1';
	cell.Right	= s[len-3]=='1';
	cell.Down	= s[len-2]=='1';
	cell.Left	= s[len-1]=='1';	
	s[len-5] = 0;
	cell.Player = atoi(s);	
	return is;
}
#endif
