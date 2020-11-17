#ifndef FVECTOR4_H
#define FVECTOR4_H

class FVector3;

class FVector4 
{
public:
	float VectorArray[4];

	///Constructors

	//Constructor using initial values for each component.
	inline FVector4(float x, float y, float z, float w) { VectorArray[0] = x; VectorArray[1] = y; VectorArray[2] = z; VectorArray[3] = w; }

	//Constructor initializing all components to a single float value.
	inline FVector4(float value) { VectorArray[0] = VectorArray[1] = VectorArray[2] = VectorArray[3] = value; }

	//Constructor initializing to match a given FVector4, a copy constructor
	FVector4(const FVector4& initilizerVector);

	//Default Constructor, initializes all values to 0;
	inline FVector4() { VectorArray[0] = 0; VectorArray[1] = 0; VectorArray[2] = 0; VectorArray[3] = 1; }

	inline FVector4(float array[4]) { VectorArray[0] = array[0]; VectorArray[1] = array[1]; VectorArray[2] = array[2]; VectorArray[3] = array[3]; }

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

	///The next 4 operators make sure the FVector3 class can be used as an array by API such as Open-GL and Vulkan
		inline const float operator[] (unsigned int index) const { return *(&VectorArray[0] + index); }//For R-values
		inline float& operator[] (unsigned int index) { return *(&VectorArray[0] + index); }//For L-Values

		inline operator const float* () const { return static_cast<const float*>(&VectorArray[0]); }//For R-Values
		inline operator float* () { return static_cast<float*>(&VectorArray[0]); }//For L-Values
	///-----------------------------------------------------------------------------------------------------------

	///Functions

	//This function treats BOTH vectors as their 3D vector(FVector3) counterparts, as there is no cross product between two 4D vectors.
	FVector3 CrossProduct(const FVector4& Vector);

	inline float Dot(const FVector4& Vector) { return (VectorArray[0] * Vector.VectorArray[0]) + (VectorArray[1] * Vector.VectorArray[1]) + (VectorArray[2] * Vector.VectorArray[2]) + (VectorArray[3] * Vector.VectorArray[3]); }

	//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length();

	//Gets the normalized form of this Vector
	FVector4 GetNormal();

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Utility to populate vector
	inline void Load(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; VectorArray[3] = W; }
};

#endif // !FVECTOR4_H

