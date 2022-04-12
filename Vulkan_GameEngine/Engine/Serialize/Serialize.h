#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <fstream>
#include <iostream>

#include "Tetrahedron.h"

using namespace std;

class FVector3;

class Serialize
{
public:
	//Serialize(int x) : m_x(x), m_y(x + 1) {};
	Serialize() {};
	~Serialize();
	
	//friend istream& operator>> (istream& in, Serialize& obj);
	//friend ostream& operator<< (ostream& out, const Serialize& obj);

	//friend ostream& operator<< (ostream& out, const GO_Tetrahedron& obj);
	//friend istream& operator>> (istream& in, GO_Tetrahedron& obj);

	void SaveGame(ostream& out, const GO_Tetrahedron& obj);

	void LoadGame();
	
private:
	//int m_x;
	//int m_y;
};

#endif // !SERIALIZE_H



