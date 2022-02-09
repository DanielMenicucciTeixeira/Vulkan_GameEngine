#ifndef FVECTOR4_H
#define FVECTOR4_H

class FVector3;

class FVector4
{
public:
	float X, Y, Z, W;

	///Constructors

	//Constructor using initial values for each component.
	inline FVector4(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
	
	//FVector3 to Vector4 constructor
	inline FVector4(FVector3 vec, float w_) { X = vec.X; Y = vec.Y; Z = vec.Z; W = w_; }

	FVector4(const FVector3& vector);

	//Constructor initializing all components to a single float value.
	inline FVector4(float value) { X = Y = Z = W = value; }

	//Constructor initializing to match a given FVector4, a copy constructor
	FVector4(const FVector4& initilizerVector);

	//Default Constructor, initializes all values to 0;
	inline FVector4() { X = 0; Y = 0; Z = 0; W = 1; }

	inline FVector4(float values[4]) { X = values[0]; Y = values[1]; Z = values[2]; W = values[3]; }

	///Destructor

	~FVector4();

	///Operators

	FVector4 operator+ (const FVector4& vector) const;//Overload of the "+" operator, makes a vector plus vector addition.

	FVector4 operator- (const FVector4& vector) const;//Overload of the "-" operator, makes a vector minus vector subtraction.

	FVector4 operator* (float multiplier) const;//Overload of the "*" operator, makes a vector times scalar multiplication.

	FVector4 operator/ (float divisor) const;//Overload of the "/" operator, makes a vector divided by scalar division.

	float operator* (const FVector4& vector) const;//Overload of the "*" operator, makes a vector times vector dot product.

	void operator= (const FVector4& vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator= (FVector4* vector);//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.

	void operator= (float values[4]);//

	void operator+= (const FVector4& vector);//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.

	void operator-= (const FVector4& vector);//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.

	void operator*= (const FVector4& vector);

	///The next 4 operators make sure the FVector3 class can be used as an values by API such as Open-GL and Vulkan
		inline const float operator[] (unsigned int index) const { return *(&X + index); }//For R-values
		inline float& operator[] (unsigned int index) { return *(&X + index); }//For L-Values

		inline operator const float* () const { return static_cast<const float*>(&X); }//For R-Values
		inline operator float* () { return static_cast<float*>(&X); }//For L-Values
	///-----------------------------------------------------------------------------------------------------------

	///Functions

	//This function treats BOTH vectors as their 3D vector(FVector3) counterparts, as there is no cross product between two 4D vectors.
	FVector3 CrossProduct(const FVector4& Vector) const;

	inline float Dot(const FVector4& Vector) const { return (X * Vector.X) + (Y * Vector.Y) + (Z * Vector.Z) + (W * Vector.W); }

	//Returns the magnitude of the Vector (or Vector's scalar lenght)
	float Length() const;

	//Gets the normalized form of this Vector
	FVector4 GetNormal() const;

	//Divides the vector by its Magnitude to get the normalized unit vector.
	void Normalize();

	//Utility to populate vector
	inline void Load(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; W = W; }

	void Print() const;
};

#endif // !FVECTOR4_H

