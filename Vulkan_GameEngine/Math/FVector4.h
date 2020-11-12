#ifndef FVECTOR4_H
#define FVECTOR4_H

#include "FVector3.h"

class FVector4 : public FVector3
{
public:
	float w;

	///Constructors

	//Constructor using initial values for each component.
	inline FVector4(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; w = W; }

	//Constructor initializing all components to a single float value.
	inline FVector4(float Float) { X = Y = Z = w = Float; }

	//Constructor initializing to match a given FVector4, a copy constructor
	inline FVector4(const FVector4& InitilizerVector);

	//Default Constructor, initializes all values to 0;
	inline FVector4() { X = Y = Z = 0; w = 1; }

	///Destructor

	~FVector4();

	///Operators

	FVector4 operator + (const FVector4& Vector);//Overload of the "+" operator, makes a vector plus vector addition.

	FVector4 operator - (const FVector4& Vector);//Overload of the "-" operator, makes a vector minus vector subtraction.

	FVector4 operator * (float Multiplier);//Overload of the "*" operator, makes a vector times scalar multiplication.

	FVector4 operator / (float Divisor);//Overload of the "/" operator, makes a vector divided by scalar division.

	float operator * (const FVector4& Vector);//Overload of the "*" operator, makes a vector times vector dot product.

	void operator = (const FVector4& Vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator = (FVector4* Vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator += (const FVector4& Vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.

	void operator -= (const FVector4& Vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.

	///Functions

	//This function treats BOTH vectors as their 3D vector(FVector3) counterparts, as there is no cross product between two 4D vectors.
	inline FVector3 CrossProduct(const FVector4& Vector) { return FVector3(Y * Vector.Z - Z * Vector.Y, Z * Vector.X - X * Vector.Z, X * Vector.Y - Y * Vector.X); }

	inline float Dot(const FVector4& Vector) { return (X * Vector.X) + (Y * Vector.Y) + (Z * Vector.Z) + (w * Vector.w); }

	//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length();

	//Gets the normalized form of this Vector
	FVector4 GetNormal();

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Utility to populate vector
	inline void Load(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; w = W; }

	void test();
};

#endif // !FVECTOR4_H

