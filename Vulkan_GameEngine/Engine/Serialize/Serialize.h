#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <fstream>
#include <iostream>

#include <string>

#include "../Objects/Components/Component.h"

using namespace std;

class Serialize
{
public:
	Serialize();
	~Serialize();

	//friend istream& operator>> (istream& in, Serialize& obj);
	//friend ostream& operator<< (ostream& out, const Serialize& obj);

	//friend ostream& operator<< (ostream& out, const GO_Tetrahedron& obj);
	//friend istream& operator>> (istream& in, GO_Tetrahedron& obj);

	void SaveGame(ostream& out, const O_GameObject& obj);
	
private:
	//int m_x;
	//int m_y;
};

#endif // !SERIALIZE_H



