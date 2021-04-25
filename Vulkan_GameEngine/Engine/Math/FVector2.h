#ifndef FVector2_H
#define FVector2_H

class IVector2;

class FVector2
{
public:
	//Array containing Vector elements
	float X, Y;

	///Constructors

	//Constructor using initial values for each component.
	FVector2(float x, float y);

	//Constructor initializing all components to a single float value.
	FVector2(float value);

	//Constructor initializing to match a given FVector2, a copy constructor
	FVector2(const FVector2& vector);
	
	//Constructor initializing to match a given IVector2, a copy constructor
	FVector2(const IVector2& vector);

	//Contructor initalizaing to match given values
	FVector2(float values[2]);

	//Default Constructor, initializes all values to 0;
	FVector2();

	///Destructor

	~FVector2();

	///Operators

	FVector2 operator+ (const FVector2& vector) const;//Overload of the "+" operator, makes a vector plus vector addition.
	inline FVector2 operator+ (const FVector2*& vector) const { return *this + *vector; }

	FVector2 operator- (const FVector2& vector) const;//Overload of the "-" operator, makes a vector minus vector subtraction.
	inline FVector2 operator- (const FVector2*& vector) const { return *this - *vector; }

	FVector2 operator* (float multiplier) const;//Overload of the "*" operator, makes a vector times scalar multiplication.

	FVector2 operator/ (float divisor) const;//Overload of the "/" operator, makes a vector divided by scalar division.

	float operator* (const FVector2& vector) const;//Overload of the "*" operator, makes a vector times vector dot product.
	inline float operator* (const FVector2*& vector) const { return *this * *vector; }

	void operator= (const FVector2& vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
	inline void operator= (FVector2* vector) { *this = *vector; }

	void operator= (float values[3]);

	void operator+= (const FVector2& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
	inline void operator+= (const FVector2*& vector) { *this += *vector; }

	void operator-= (const FVector2& vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
	inline void operator-= (const FVector2*& vector) { *this -= *vector; }

	bool operator== (const FVector2& vector) const;//Overload of the "==" operator, returns the boolean value of equal to between two vectors.
	inline bool operator== (const FVector2*& vector) const { return *this == *vector; }

	inline bool operator!= (const FVector2& vector) const { return !(*this == vector); }//Overload of the "!=" operator, returns the boolean value of not equal to between two vectors.
	inline bool operator!= (const FVector2*& vector) const { return !(*this == *vector); }

	///The next 4 operators make sure the FVector2 class can be used as an values by APIs such as Open-GL and Vulkan
	inline const float operator[] (unsigned int index) const { return *(&X + index); }//For R-values
	inline float& operator[] (unsigned int index) { return*(&X + index); }//For L-Values

	inline operator const float* () const { return static_cast<const float*>(&X); }//For R-Values
	inline operator float* () { return static_cast<float*>(&X); }//For L-Values
///-----------------------------------------------------------------------------------------------------------


///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length() const;

	//Returns the angle between this vector and another given vector in degrees.
	float GetAngle(const FVector2& vector, bool inRadians = false);
	inline float GetAngle(const FVector2*& vector, bool inRadians = false) { return GetAngle(*vector, inRadians); }

	//Returns the cross product between this vector and another given vector.
	FVector2 CrossProduct(const FVector2& vector);
	inline FVector2 CrossProduct(const FVector2*& vector) { return CrossProduct(*vector); }

	//Gets the normalized form of this Vector
	//Does not alter teh vector itself
	FVector2 GetNormal() const;

	//Divides the vector by its Magnitude to set it to the normalized unit vector.
	void Normalize();

	//Rotates the vector by a given angle
	//set second parameter to true to pass angle value in radians
	void Rotate(float angle, bool isRadian = false);

	//Returns the 2D vector that points at a given angle
	//set second parameter to true to pass angle value in radians
	static FVector2 GetVectorByAngle(float angle, bool isRadian = false);

	float GetDistance(FVector2 vector) const;
	static float GetDistance(FVector2 vector1, FVector2 vector2);

	void Print();
};

struct HASH_FVector2
{
	size_t operator() (const FVector2& vector) const;
};

#endif