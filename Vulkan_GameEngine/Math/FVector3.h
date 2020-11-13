#ifndef FVector3_
#define FVector3_


class FVector3
{
public:
	//Array containing Vector elements
	float VectorArray[3];

	///Constructors

	//Constructor using initial values for each component.
	FVector3(float x, float y, float z);

	//Constructor initializing all components to a single float value.
	FVector3(float Float);

	//Constructor initializing to match a given FVector3, a copy constructor
	FVector3(const FVector3& InitilizerVector);

	//Contructor initalizaing to match given array
	FVector3(float array[3]);

	//Default Constructor, initializes all values to 0;
	FVector3();

	///Destructor

	~FVector3();

	///Operators

	virtual FVector3 operator+ (FVector3 Vector);//Overload of the "+" operator, makes a vector plus vector addition.

	virtual FVector3 operator- (FVector3 Vector);//Overload of the "-" operator, makes a vector minus vector subtraction.

	FVector3 operator* (float Multiplier);//Overload of the "*" operator, makes a vector times scalar multiplication.

	FVector3 operator/ (float Divisor);//Overload of the "/" operator, makes a vector divided by scalar division.

	virtual float operator* (FVector3 Vector);//Overload of the "*" operator, makes a vector times vector dot product.

	virtual void operator= (FVector3 Vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator= (FVector3 * Vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator+= (FVector3 Vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.

	void operator-= (FVector3 Vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.

	bool operator== (FVector3 Vector);//Overload of the "==" operator, returns the boolean value of equal to between two vectors.

	///The next 4 operators make sure the FVector3 class can be used as an array by API such as Open-GL and Vulkan
		inline const float operator[] (unsigned int index) const { return *(&VectorArray[0] + index); }//For R-values
		inline float& operator[] (unsigned int index) { return*(&VectorArray[0] + index); }//For L-Values

		inline operator const float* () const { return static_cast<const float*>(&VectorArray[0]); }//For R-Values
		inline operator float* () { return static_cast<float*>(&VectorArray[0]); }//For L-Values
	///-----------------------------------------------------------------------------------------------------------


	///Functions

	//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length();

	//Returns the angle between this vector and another given vector in degrees.
	float GetAngle(FVector3 Vector);

	//Returns the angle between this vector and another given vector in radians.
	float GetRadAngle(FVector3 Vector);

	//Returns the cross product between this vector and another given vector.
	FVector3 CrossProduct(FVector3 Vector);

	//Gets the normalized form of this Vector
	FVector3 GetNormal();

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Rotates the vector by disered amount in degrees in the Z axis
	void RotateZ(float Degrees);

	//Rotates the vector by disered amount in radians in the Z axis
	void RotateZRad(float Degrees);

	//Utility to populate vector
	inline void Load(float x, float y, float z);
};
#endif