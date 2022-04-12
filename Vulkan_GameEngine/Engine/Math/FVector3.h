#ifndef FVector3_H
#define FVector3_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <type_traits>

class FQuaternion;
class FVector4;

class FVector3
{
public:
	//Array containing Vector elements
	float X, Y, Z;

	///Constructors

	//Constructor using initial values for each component.
	FVector3(float x, float y, float z);

	//Constructor initializing all components to a single float value.
	FVector3(float value);

	//Constructor initializing to match a given FVector3, a copy constructor
	FVector3(const FVector3& vector);

	//Constructor initializing to match a given FVector3, a copy constructor
	FVector3(const FVector4& vector);

	//Contructor initalizaing to match given values
	FVector3(float values[3]);

	//Default Constructor, initializes all values to 0;
	FVector3();

	///Destructor

	~FVector3();

	///Operators

	FVector3 operator+ (const FVector3& vector) const;//Overload of the "+" operator, makes a vector plus vector addition.
	inline FVector3 operator+ (const FVector3*& vector) const { return *this + *vector; }

	FVector3 operator- (const FVector3& vector) const;//Overload of the "-" operator, makes a vector minus vector subtraction.
	inline FVector3 operator- (const FVector3*& vector) const { return *this - *vector; }

	FVector3 operator* (float multiplier) const;//Overload of the "*" operator, makes a vector times scalar multiplication.

	FVector3 operator/ (float divisor) const;//Overload of the "/" operator, makes a vector divided by scalar division.

	float operator* (const FVector3& vector) const;//Overload of the "*" operator, makes a vector times vector dot product.
	inline float operator* (const FVector3*& vector) const { return *this * *vector; }

	void operator= (const FVector3& vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
	inline void operator= (FVector3* vector) { *this = *vector; }

	void operator= (const FVector4& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
	inline void operator= (const FVector4*& vector) { *this = *vector; }

	void operator= (float values[3]);

	void operator+= (const FVector3& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
	inline void operator+= (const FVector3*& vector) { *this += *vector; }

	void operator-= (const FVector3& vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
	inline void operator-= (const FVector3*& vector) { *this -= *vector; }

	bool operator== (const FVector3& vector) const;//Overload of the "==" operator, returns the boolean value of equal to between two vectors.
	inline bool operator== (const FVector3*& vector) const { return *this == *vector; }
	inline FVector3 operator-() const { return FVector3(-X, -Y, -Z); }
	inline bool operator!= (const FVector3& vector) const { return !(*this == vector); }//Overload of the "!=" operator, returns the boolean value of not equal to between two vectors.
	inline bool operator!= (const FVector3*& vector) const { return !(*this == *vector); }

	///The next 4 operators make sure the FVector3 class can be used as an values by APIs such as Open-GL and Vulkan
		inline const float operator[] (unsigned int index) const { return *(&X + index); }//For R-values
		inline float& operator[] (unsigned int index) { return*(&X + index); }//For L-Values

		inline operator const float* () const { return static_cast<const float*>(&X); }//For R-Values
		inline operator float* () { return static_cast<float*>(&X); }//For L-Values
	///-----------------------------------------------------------------------------------------------------------


	///Functions

	//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length() const;

	//Returns the angle between this vector and another given vector in degrees.
	float GetAngle(const FVector3& vector, bool inRadians = false);
	inline float GetAngle(const FVector3*& vector, bool inRadians = false) { return GetAngle(*vector, inRadians); }

	//Returns the cross product between this vector and another given vector.
	FVector3 CrossProduct(const FVector3& vector);
	inline FVector3 CrossProduct(const FVector3*& vector) { return CrossProduct(*vector); }

	//Gets the normalized form of this Vector
	FVector3 GetNormal() const;

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Rotates the vector by a quarternion
	void Rotate(const FQuaternion& rotation);
	inline void Rotate(const FQuaternion*& rotation) { Rotate(*rotation); }

	//Returns the rotated form of the vector given a quaternion
	FVector3 GetRotatedVector(const FQuaternion& rotation) const;
	inline FVector3 GetRotatedVector(const FQuaternion*& rotation) const { return GetRotatedVector(*rotation); }

	//Returns the dot product of vector
	inline float GetDotProduct(FVector3 b) { return X * b.X + Y + b.Y + Z * b.Z; }

	//Utility to populate vector
	inline void Load(float x, float y, float z);

	void Print();

	friend std::ostream& operator<<(std::ostream& os, const FVector3& vec);
};

struct HASH_FVector3
{
	size_t operator() (const FVector3& vector) const;
};

#endif