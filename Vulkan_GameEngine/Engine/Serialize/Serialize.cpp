#include "Serialize.h"
#include "../Objects/GameObjects/GameObject.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Math/FVector3.h"


Serialize::Serialize()
{
}


Serialize::~Serialize()
{
}

void Serialize::SaveGame(ostream& out, const O_GameObject& obj)
{
	out << "Object name:" << obj.GetName() << endl;
	out << "Object position: " << obj.GetPosition() << endl;
}

/*
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
*/