#ifndef FMATRIX4_H
#define FMATRIX4_H
#include "FMatrix.h"

class FVector3;
class FVector4;

class FMatrix4
{


public:

	//the Matrix itself
	float Matrix[4][4];

	///Constructors

	//Default contructor, initilizes to an identity matrix
	FMatrix4();

	FMatrix4(FMatrix _Matrix);

	//Constructor that initilizes all elements of the matrix to a given float
	FMatrix4(float Value);

	//Initializes the Matrix Given 3 FVector3s for rows
	FMatrix4(const FVector4& Row1, const FVector4& Row2, const FVector4& Row3, const FVector4& Row4);

	//Initialiazes the Matrix given 3 floats for the diagonal
	FMatrix4(float X, float Y, float Z);

	//Initializes the Matrix given a 4x4 Array
	FMatrix4(float _Matrix[4][4]);

	//Initializes the Matrix to a rotation matrix given the Angle and Axis, optionaly, input true to give the angle in radians
	FMatrix4(eRotationAxis Axis, float Angle, bool IsAngleRadian = false);

	///Destructors

	~FMatrix4();

	///Operator Overloads

	void operator= (FMatrix4 _Matrix);

	FMatrix4 operator* (float Multiplier);

	FMatrix4 operator* (FMatrix4 _Matrix);

	FMatrix4* operator* (FMatrix4* _Matrix);

	FVector3 operator*(FVector3 Vector);

	FVector4* operator*(FVector4* Vector);

	void operator*= (FMatrix4 _Matrix);

	void operator*= (float Multiplier);

	///Functions

	//Makes this Matrix an Identity matrix
	void SetToIdentity();

	//Makes this Matrix a Rotation Matrix on given axis with given degrees optionaly, input true to give the angle in radians
	void SetToRotationMatrix(eRotationAxis Asix, float Angle, bool IsAngleRadian = false);

	//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
	void SetToScalingMatrix(float ScaleX, float ScaleY, float ScaleZ);

	//Makes this Matrix a Translation Matrix
	void SetToTranslationMatrix(float X, float Y, float Z);

	//Swaps Rows and Columns
	void Transpose();

	//Sets the matrix to be equal another matrix given in the for of a 3x3 array.
	void SetToArray44(float Array[4][4]);

	//Sets all elements of the matrix to a given value
	void SetAllElements(float Value);

	//Returns the Determinant of the Matrix
	float Det();

	//Returns the Iverse of the Matrix
	FMatrix4 GetInverse();

	//Inverts the Matrix
	void Invert();

	//Prints the matrix to console
	void PrintMatrix();
};
#endif

