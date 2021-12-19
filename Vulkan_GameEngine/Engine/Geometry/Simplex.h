#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "Math/FVector3.h"
#include "Math/FVector4.h"
#include "Math/FMatrix.h"
#include <array>

//#define Simplex2D Simplex<3>
//#define Simplex3D Simplex<4>

struct Simplex 
{
private:
	std::array<FVector3, 4> Vertices;
	unsigned Size;

public:
	Simplex() : Vertices() , Size(0) {}

	Simplex& operator=(std::initializer_list<FVector3> list) 
	{
		for (auto v = list.begin(); v != list.end(); v++) 
		{
			Vertices[std::distance(list.begin(), v)] = *v;
		}
		Size = list.size();

		return *this;
	}

	void PushFront(FVector3 point) 
	{
		Vertices = { point, Vertices[0], Vertices[1], Vertices[2] };
		Size = std::min(Size + 1, 4u);
	}

	inline FVector3& operator[](unsigned index) { return Vertices[index]; }
	unsigned size() const { return Size; }

	auto begin() const { return Vertices.begin(); }
	auto end()   const { return Vertices.end() - (4 - Size); }
};

//template<int numberOfVertices>
//class Simplex
//{
//protected:
//	FVector3 Vertices[numberOfVertices];
//
//public:
//	inline void SetVertexAtIndex(int index, FVector3 newVertex) { Vertices[index] = newVertex; }
//	
//	void PushBack(FVector3 newVertex)
//	{
//		for (int i = 0; i < numberOfVertices - 2; i++)
//		{
//			Vertices[i] = Vertices[i + 1];
//		}
//
//		Vertices[numberOfVertices - 1] = newVertex;
//	}
//	
//	void PushFront(FVector3 newVertex)
//	{
//		for (int i = numberOfVertices - 1; i > 0; i--)
//		{
//			Vertices[i] = Vertices[i - 1];
//		}
//
//		Vertices[0] = newVertex;
//	}
//
//	inline FVector3 GetVertexAtIndex(unsigned int index)
//	{ 
//		if (index < numberOfVertices)
//		{
//			auto test = Vertices[index];
//			return Vertices[index];
//		}
//		else return FVector3();
//	}
//};

/*struct Simplex3D
{
	std::vector<FVector4> PointList;

	void AddPoint(FVector3 pos_)
	{
		PointList.push_back(FVector4(pos_.X, pos_.Y, pos_.Z, 1.0f));
	}

	FVector4 GetPointAtIndex(int index)
	{
		return PointList[index];
	}

	FVector3 GetCenter()
	{
		return (PointList[0] + PointList[1] + PointList[2]) / 3.0f;
	}

	void EraseP(int index)
	{
		PointList.erase(PointList.begin() + index);
	}
};*/

#endif
