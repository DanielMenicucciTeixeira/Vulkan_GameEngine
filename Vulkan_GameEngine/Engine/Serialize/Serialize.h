#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <fstream>
#include <iostream>

using namespace std;

class Serialize
{
public:
	Serialize(int x) : m_x(x), m_y(x + 1) {};
	~Serialize();

	friend istream& operator>> (istream& in, Serialize& obj);
	friend ostream& operator<< (ostream& out, const Serialize& obj);
	
private:
	int m_x;
	int m_y;
};

#endif // !SERIALIZE_H



