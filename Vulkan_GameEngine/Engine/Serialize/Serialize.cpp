#include "Serialize.h"

Serialize::~Serialize()
{
}

istream& operator>>(istream& in, Serialize& obj)
{
	in >> obj.m_x;
	in >> obj.m_y;
	return in;
}

ostream& operator<<(ostream& out, const Serialize& obj)
{
	out << obj.m_x << ' ';
	out << obj.m_y << endl;
	return out;
}
