#ifndef FMATRIX4_H
#define FMATRIX4_H

class FVector3;
class FVector4;
class FMatrix;

enum eRotationAxis;

class FMatrix4
{
public:

	//the Matrix itself
	float Matrix[16];//this

	///Constructors

	//Default contructor, initilizes to an identity matrix
	FMatrix4();

	FMatrix4(FMatrix matrix);

	//Constructor that initilizes all elements of the matrix to a given float
	FMatrix4(float value);

	//Initializes the Matrix Given 3 FVector3s for rows
	FMatrix4(const FVector4& row1, const FVector4& row2, const FVector4& row3, const FVector4& row4);

	//Initialiazes the Matrix given 3 floats for the diagonal
	FMatrix4(float x, float y, float z);

	//Initializes the Matrix given a 4x4 Array
	FMatrix4(float matrix[4][4]);

	//Initializes the Matrix to a rotation matrix given the angle and axis, optionaly, input true to give the angle in radians
	FMatrix4(eRotationAxis axis, float angle, bool isAngleRadian = false);

	///Destructors

	~FMatrix4();

	///Operator Overloads

	void operator= (FMatrix4 matrix);

	void operator= (FMatrix4* matrix);

	FMatrix4 operator* (float multiplier);

	FMatrix4 operator* (FMatrix4* matrix);

	FMatrix4 operator* (FMatrix4 matrix);

	FVector4 operator*(FVector4* vector);

	FVector3 operator*(FVector3 vector);

	void operator*= (FMatrix4* matrix);

	void operator*= (FMatrix4 matrix);

	void operator*= (float multiplier);


	inline const float operator[] (int index) const { return Matrix[index]; }
	inline float& operator[] (int index) { return Matrix[index]; }

	inline const float operator[] (int index[2]) const { return Matrix[index[0] + index[1] * 4]; }
	inline float& operator[] (int index[2]) { return Matrix[index[0] + index[1] * 4]; }

	inline operator const float* () const { return static_cast<const float*>(&Matrix[0]); }
	inline operator float* () { return static_cast<float*>(&Matrix[0]); }

	///Functions

	//Makes this Matrix an Identity matrix
	void SetToIdentity();

	//Makes this Matrix a Rotation Matrix on given axis with given degrees optionaly, input true to give the angle in radians
	void SetToRotationMatrix(eRotationAxis axis, float angle, bool isAngleRadian = false);

	//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
	void SetToScalingMatrix(float scaleX, float scaleY, float scaleZ);

	//Makes this Matrix a Translation Matrix
	void SetToTranslationMatrix(float X, float Y, float Z);

	//Swaps rows and Columns
	void Transpose();

	//Sets all elements of the matrix to a given value
	void SetAllElements(float value);

	//Returns the Determinant of the Matrix
	float Det();

	//Returns the Iverse of the Matrix
	FMatrix4 GetInverse();

	//Inverts the Matrix
	void Invert();

	//Prints the matrix to console
	void Print();
};
#endif

