#include "Serialize.h"

Serialize::~Serialize()
{
}

void Serialize::SaveGame(ostream& out, const GO_Tetrahedron& obj)
{
	out << obj.GetName() << ' ';
	//out << obj.GetPosition() << ' ';
	
}

/*istream& operator>>(istream& in, Serialize& obj)
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

	

ostream& operator<<(ostream& out, const GO_Tetrahedron& obj)
{
	out << obj.GetName() << ' ' << obj.GetPosition();
	return out;
}

istream& operator>>(istream& in, GO_Tetrahedron& obj)
{
	return in;
}
*/

