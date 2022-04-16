#ifndef IVECTOR2_H
#define IVECTOR2_H

class FVector2;

class IVector2
{
public:
	//Array containing Vector elements
	int X, Y;

	///Constructors

	//Constructor using initial values for each component.
	IVector2(int x, int y);

	//Constructor initializing all components to a single int value.
	IVector2(int value);

	//Constructor initializing to match a given IVector2, a copy constructor
	IVector2(const IVector2& vector);

	//Constructor initializing to match a given FVector2, a copy constructor
	IVector2(const FVector2& vector);

	//Contructor initalizaing to match given values
	IVector2(int values[2]);

	//Default Constructor, initializes all values to 0;
	IVector2();

	///Destructor

	~IVector2();

	///Operators

	IVector2 operator+ (const IVector2& vector) const;//Overload of the "+" operator, makes a vector plus vector addition.
	inline IVector2 operator+ (const IVector2*& vector) const { return *this + *vector; }

	IVector2 operator- (const IVector2& vector) const;//Overload of the "-" operator, makes a vector minus vector subtraction.
	inline IVector2 operator- (const IVector2*& vector) const { return *this - *vector; }

	IVector2 operator* (int multiplier) const;//Overload of the "*" operator, makes a vector times scalar multiplication.

	IVector2 operator/ (int divisor) const;//Overload of the "/" operator, makes a vector divided by scalar division.

	int operator* (const IVector2& vector) const;//Overload of the "*" operator, makes a vector times vector dot product.
	inline int operator* (const IVector2*& vector) const { return *this * *vector; }

	inline IVector2 operator| (const IVector2& vector) const { return IVector2(X * vector.X, Y * vector.Y); }//Overload of the "|" operator, returns vector { X*x, Y*y}
	inline IVector2 operator| (const IVector2*& vector) const { return *this | vector; }

	void operator= (const IVector2& vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
	inline void operator= (IVector2* vector) { *this = *vector; }

	void operator= (int values[3]);

	void operator+= (const IVector2& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
	inline void operator+= (const IVector2*& vector) { *this += *vector; }

	void operator-= (const IVector2& vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
	inline void operator-= (const IVector2*& vector) { *this -= *vector; }

	bool operator== (const IVector2& vector) const;//Overload of the "==" operator, returns the boolean value of equal to between two vectors.
	inline bool operator== (const IVector2*& vector) const { return *this == *vector; }

	inline bool operator!= (const IVector2& vector) const { return !(*this == vector); }//Overload of the "!=" operator, returns the boolean value of not equal to between two vectors.
	inline bool operator!= (const IVector2*& vector) const { return !(*this == *vector); }

	///The next 4 operators make sure the IVector2 class can be used as an values by APIs such as Open-GL and Vulkan
	inline const int operator[] (unsigned int index) const { return *(&X + index); }//For R-values
	inline int& operator[] (unsigned int index) { return*(&X + index); }//For L-Values

	inline operator const int* () const { return static_cast<const int*>(&X); }//For R-Values
	inline operator int* () { return static_cast<int*>(&X); }//For L-Values
///-----------------------------------------------------------------------------------------------------------


///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
	int Length() const;

	//Returns the angle between this vector and another given vector in degrees.
	int GetAngle(const IVector2& vector, bool inRadians = false);
	inline int GetAngle(const IVector2*& vector, bool inRadians = false) { return GetAngle(*vector, inRadians); }

	//Returns the cross product between this vector and another given vector.
	IVector2 CrossProduct(const IVector2& vector);
	inline IVector2 CrossProduct(const IVector2*& vector) { return CrossProduct(*vector); }

	//Gets the normalized form of this Vector
	//Does not alter teh vector itself
	IVector2 GetNormal() const;

	//Divides the vector by its Magnitude to set it to the normalized unit vector.
	void Normalize();

	//Rotates the vector by a given angle
	//set second parameter to true to pass angle value in radians
	void Rotate(int angle, bool isRadian = false);

	//Returns the 2D vector that points at a given angle
	//set second parameter to true to pass angle value in radians
	static IVector2 GetVectorByAngle(int angle, bool isRadian = false);

	int GetDistance(IVector2 vector) const;
	static int GetDistance(IVector2 vector1, IVector2 vector2);

	void Print() const;
};

struct HASH_IVector2
{
	size_t operator() (const IVector2& vector) const;
};
#endif
