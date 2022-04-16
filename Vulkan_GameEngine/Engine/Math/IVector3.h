#ifndef IVector3_H
#define IVector3_H


class FQuaternion;

class IVector3
{
public:
	//Array containing Vector elements
	int X, Y, Z;

	///Constructors

	//Constructor using initial values for each component.
	IVector3(int x, int y, int z);

	//Constructor initializing all components to a single int value.
	IVector3(int value);

	//Constructor initializing to match a given IVector3, a copy constructor
	IVector3(const IVector3& vector);

	//Contructor initalizaing to match given values
	IVector3(int values[3]);

	//Default Constructor, initializes all values to 0;
	IVector3();

	///Destructor

	~IVector3();

	///Operators

	IVector3 operator+ (const IVector3& vector) const;//Overload of the "+" operator, makes a vector plus vector addition.
	inline IVector3 operator+ (const IVector3*& vector) const { return *this + *vector; }

	IVector3 operator- (const IVector3& vector) const;//Overload of the "-" operator, makes a vector minus vector subtraction.
	inline IVector3 operator- (const IVector3*& vector) const { return *this - *vector; }

	IVector3 operator* (int multiplier) const;//Overload of the "*" operator, makes a vector times scalar multiplication.

	IVector3 operator/ (int divisor) const;//Overload of the "/" operator, makes a vector divided by scalar division.

	int operator* (const IVector3& vector) const;//Overload of the "*" operator, makes a vector times vector dot product.
	inline int operator* (const IVector3*& vector) const { return *this * *vector; }

	void operator= (const IVector3& vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
	inline void operator= (IVector3* vector) { *this = *vector; }

	void operator= (int values[3]);

	void operator+= (const IVector3& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
	inline void operator+= (const IVector3*& vector) { *this += *vector; }

	void operator-= (const IVector3& vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
	inline void operator-= (const IVector3*& vector) { *this -= *vector; }

	bool operator== (const IVector3& vector) const;//Overload of the "==" operator, returns the boolean value of equal to between two vectors.
	inline bool operator== (const IVector3*& vector) const { return *this == *vector; }

	inline bool operator!= (const IVector3& vector) const { return !(*this == vector); }//Overload of the "!=" operator, returns the boolean value of not equal to between two vectors.
	inline bool operator!= (const IVector3*& vector) const { return !(*this == *vector); }

	///The next 4 operators make sure the IVector3 class can be used as an values by APIs such as Open-GL and Vulkan
	inline const int operator[] (unsigned int index) const { return *(&X + index); }//For R-values
	inline int& operator[] (unsigned int index) { return*(&X + index); }//For L-Values

	inline operator const int* () const { return static_cast<const int*>(&X); }//For R-Values
	inline operator int* () { return static_cast<int*>(&X); }//For L-Values
///-----------------------------------------------------------------------------------------------------------


///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
	int Length() const;

	//Returns the angle between this vector and another given vector in degrees.
	int GetAngle(const IVector3& vector, bool inRadians = false);
	inline int GetAngle(const IVector3*& vector, bool inRadians = false) { return GetAngle(*vector, inRadians); }

	//Returns the cross product between this vector and another given vector.
	IVector3 CrossProduct(const IVector3& vector);
	inline IVector3 CrossProduct(const IVector3*& vector) { return CrossProduct(*vector); }

	//Gets the normalized form of this Vector
	IVector3 GetNormal() const;

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Rotates the vector by a quarternion
	void Rotate(const FQuaternion& rotation);
	inline void Rotate(const FQuaternion*& rotation) { Rotate(*rotation); }

	//Returns the rotated form of the vector given a quaternion
	IVector3 GetRotatedVector(const FQuaternion& rotation) const;
	inline IVector3 GetRotatedVector(const FQuaternion*& rotation) const { return GetRotatedVector(*rotation); }

	//Utility to populate vector
	inline void Load(int x, int y, int z);

	void Print();
};

//struct HASH_FVector3
//{
//	size_t operator() (const IVector3& vector) const;
//};

#endif