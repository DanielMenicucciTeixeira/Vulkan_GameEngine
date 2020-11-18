#include "FMatrix4.h"

#include "FVector4.h"
#include "FVector3.h"
#include "FMatrix.h"

#include <math.h>
#include<iostream>

///Constructors

//Default contructor, initilizes to an identity matrix
FMatrix4::FMatrix4()
{
	Matrix[0] = 1.0f;
	Matrix[4] = 0.0f;
	Matrix[8] = 0.0f;
	Matrix[12] = 0.0f;

	Matrix[1] = 0.0f;
	Matrix[5] = 1.0f;
	Matrix[9] = 0.0f;
	Matrix[13] = 0.0f;

	Matrix[2] = 0.0f;
	Matrix[6] = 0.0f;
	Matrix[10] = 1.0f;
	Matrix[14] = 0.0f;

	Matrix[3] = 0.0f;
	Matrix[7] = 0.0f;
	Matrix[11] = 0.0f;
	Matrix[15] = 1.0f;
}

FMatrix4::FMatrix4(FMatrix matrix)
{
	SetToIdentity();

	for (int i = 0; i < 16; i++)
	{
		Matrix[i] = matrix[i];
	}
}

FMatrix4::FMatrix4(float value)
{
	for (int i = 0; i < 16; i++)
	{
		Matrix[i] = value;
	}
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix4::FMatrix4(const FVector4& row1, const FVector4& row2, const FVector4& row3, const FVector4& row4)
{
	Matrix[0] = row1.X;
	Matrix[4] = row1.Y;
	Matrix[8] = row1.Z;
	Matrix[12] = row1.W;

	Matrix[1] = row2.X;
	Matrix[5] = row2.Y;
	Matrix[9] = row2.Z;
	Matrix[13] = row2.W;

	Matrix[2] = row3.X;
	Matrix[6] = row3.Y;
	Matrix[10] = row3.Z;
	Matrix[14] = row3.W;

	Matrix[3] = row4.X;
	Matrix[7] = row4.Y;
	Matrix[11] = row4.Z;
	Matrix[15] = row4.W;
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix4::FMatrix4(float x, float y, float z)
{
	SetToIdentity();

	Matrix[0] = x;
	Matrix[5] = y;
	Matrix[10] = z;
}

FMatrix4::FMatrix4(float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3)
{
	Matrix[0] = a0;	Matrix[4] = b0;	Matrix[8] = c0;	Matrix[12] = d0;
	Matrix[1] = a1;	Matrix[5] = b1;	Matrix[9] = c1;	Matrix[13] = d1;
	Matrix[2] = a2;	Matrix[6] = b2;	Matrix[10] = c2;Matrix[14] = d2;
	Matrix[3] = a3;	Matrix[7] = b3;	Matrix[11] = c3;Matrix[15] = d3;
}

//Initializes the Matrix to a rotation matrix given the angle and axis, optionaly, input true to give the angle in radians
FMatrix4::FMatrix4(eRotationAxis axis, float angle, bool isAngleRadian)
{
	if (!isAngleRadian) angle *= 0.174533f;//Converts input from degrees to radians
	SetToIdentity();

	switch (axis)
	{
	case X:
		Matrix[5] = cos(angle);
		Matrix[9] = -sin(angle);
		Matrix[6] = sin(angle);
		Matrix[10] = cos(angle);
		break;

	case Y:
		Matrix[0] = cos(angle);
		Matrix[4] = -sin(angle);
		Matrix[1] = sin(angle);
		Matrix[5] = cos(angle);
		break;

	case Z:
		Matrix[0] = cos(angle);
		Matrix[8] = -sin(angle);
		Matrix[2] = sin(angle);
		Matrix[10] = cos(angle);
		break;
	}
}

///Destructors

FMatrix4::~FMatrix4()
{
}

///Operator Overloads

void FMatrix4::operator=(FMatrix4 matrix)
{
	for (int i = 0; i < 16; i++)
	{
			Matrix[i] = matrix[i];
	}
}

void FMatrix4::operator=(FMatrix4* matrix)
{
	for (int i = 0; i < 16; i++)
	{
		Matrix[i] = *matrix[i];
	}
}

FMatrix4 FMatrix4::operator*(float multiplier)
{
	FMatrix4 returnMatrix = FMatrix4(*Matrix);

	for (int i = 0; i < 16; i++)
	{
		returnMatrix[i] *= multiplier;
	}

	return returnMatrix;
}

FMatrix4 FMatrix4::operator*(FMatrix4 matrix)
{
	return FMatrix4
	(
		(Matrix[0 * 4 + 0] * matrix[0 * 4 + 0]) + (Matrix[1 * 4 + 0] * matrix[0 * 4 + 1]) + (Matrix[2 * 4 + 0] * matrix[0 * 4 + 2]) + (Matrix[3 * 4 + 0] * matrix[0 * 4 + 3]),
		(Matrix[0 * 4 + 1] * matrix[0 * 4 + 0]) + (Matrix[1 * 4 + 1] * matrix[0 * 4 + 1]) + (Matrix[2 * 4 + 1] * matrix[0 * 4 + 2]) + (Matrix[3 * 4 + 1] * matrix[0 * 4 + 3]),
		(Matrix[0 * 4 + 2] * matrix[0 * 4 + 0]) + (Matrix[1 * 4 + 2] * matrix[0 * 4 + 1]) + (Matrix[2 * 4 + 2] * matrix[0 * 4 + 2]) + (Matrix[3 * 4 + 2] * matrix[0 * 4 + 3]),
		(Matrix[0 * 4 + 3] * matrix[0 * 4 + 0]) + (Matrix[1 * 4 + 3] * matrix[0 * 4 + 1]) + (Matrix[2 * 4 + 3] * matrix[0 * 4 + 2]) + (Matrix[3 * 4 + 3] * matrix[0 * 4 + 3]),
		(Matrix[0 * 4 + 0] * matrix[1 * 4 + 0]) + (Matrix[1 * 4 + 0] * matrix[1 * 4 + 1]) + (Matrix[2 * 4 + 0] * matrix[1 * 4 + 2]) + (Matrix[3 * 4 + 0] * matrix[1 * 4 + 3]),
		(Matrix[0 * 4 + 1] * matrix[1 * 4 + 0]) + (Matrix[1 * 4 + 1] * matrix[1 * 4 + 1]) + (Matrix[2 * 4 + 1] * matrix[1 * 4 + 2]) + (Matrix[3 * 4 + 1] * matrix[1 * 4 + 3]),
		(Matrix[0 * 4 + 2] * matrix[1 * 4 + 0]) + (Matrix[1 * 4 + 2] * matrix[1 * 4 + 1]) + (Matrix[2 * 4 + 2] * matrix[1 * 4 + 2]) + (Matrix[3 * 4 + 2] * matrix[1 * 4 + 3]),
		(Matrix[0 * 4 + 3] * matrix[1 * 4 + 0]) + (Matrix[1 * 4 + 3] * matrix[1 * 4 + 1]) + (Matrix[2 * 4 + 3] * matrix[1 * 4 + 2]) + (Matrix[3 * 4 + 3] * matrix[1 * 4 + 3]),
		(Matrix[0 * 4 + 0] * matrix[2 * 4 + 0]) + (Matrix[1 * 4 + 0] * matrix[2 * 4 + 1]) + (Matrix[2 * 4 + 0] * matrix[2 * 4 + 2]) + (Matrix[3 * 4 + 0] * matrix[2 * 4 + 3]),
		(Matrix[0 * 4 + 1] * matrix[2 * 4 + 0]) + (Matrix[1 * 4 + 1] * matrix[2 * 4 + 1]) + (Matrix[2 * 4 + 1] * matrix[2 * 4 + 2]) + (Matrix[3 * 4 + 1] * matrix[2 * 4 + 3]),
		(Matrix[0 * 4 + 2] * matrix[2 * 4 + 0]) + (Matrix[1 * 4 + 2] * matrix[2 * 4 + 1]) + (Matrix[2 * 4 + 2] * matrix[2 * 4 + 2]) + (Matrix[3 * 4 + 2] * matrix[2 * 4 + 3]),
		(Matrix[0 * 4 + 3] * matrix[2 * 4 + 0]) + (Matrix[1 * 4 + 3] * matrix[2 * 4 + 1]) + (Matrix[2 * 4 + 3] * matrix[2 * 4 + 2]) + (Matrix[3 * 4 + 3] * matrix[2 * 4 + 3]),
		(Matrix[0 * 4 + 0] * matrix[3 * 4 + 0]) + (Matrix[1 * 4 + 0] * matrix[3 * 4 + 1]) + (Matrix[2 * 4 + 0] * matrix[3 * 4 + 2]) + (Matrix[3 * 4 + 0] * matrix[3 * 4 + 3]),
		(Matrix[0 * 4 + 1] * matrix[3 * 4 + 0]) + (Matrix[1 * 4 + 1] * matrix[3 * 4 + 1]) + (Matrix[2 * 4 + 1] * matrix[3 * 4 + 2]) + (Matrix[3 * 4 + 1] * matrix[3 * 4 + 3]),
		(Matrix[0 * 4 + 2] * matrix[3 * 4 + 0]) + (Matrix[1 * 4 + 2] * matrix[3 * 4 + 1]) + (Matrix[2 * 4 + 2] * matrix[3 * 4 + 2]) + (Matrix[3 * 4 + 2] * matrix[3 * 4 + 3]),
		(Matrix[0 * 4 + 3] * matrix[3 * 4 + 0]) + (Matrix[1 * 4 + 3] * matrix[3 * 4 + 1]) + (Matrix[2 * 4 + 3] * matrix[3 * 4 + 2]) + (Matrix[3 * 4 + 3] * matrix[3 * 4 + 3])
	);
}

FMatrix4 FMatrix4::operator*(FMatrix4* matrix)
{
	return (*this * *matrix);
}

FVector3 FMatrix4::operator*(FVector3 vector)
{
	FVector3 ReturnVector = FVector3();

	ReturnVector.X = Matrix[0] * vector.X + Matrix[4] * vector.Y + Matrix[8] * vector.Z;
	ReturnVector.Y = Matrix[1] * vector.X + Matrix[5] * vector.Y + Matrix[9] * vector.Z;
	ReturnVector.Z = Matrix[2] * vector.X + Matrix[6] * vector.Y + Matrix[10] * vector.Z;

	return ReturnVector;
}

FVector4 FMatrix4::operator*( FVector4* vector)
{

	return FVector4
	(
		(Matrix[0] * *vector[0] + Matrix[4] * *vector[1] + Matrix[8] * *vector[2] + Matrix[12] * *vector[3]),
		(Matrix[1] * *vector[0] + Matrix[5] * *vector[1] + Matrix[9] * *vector[2] + Matrix[13] * *vector[3]),
		(Matrix[2] * *vector[0] + Matrix[6] * *vector[1] + Matrix[10] * *vector[2] + Matrix[14] * *vector[3]),
		(Matrix[3] * *vector[0] + Matrix[7] * *vector[1] + Matrix[11] * *vector[2] + Matrix[15] * *vector[3])
	);
}

FVector4 FMatrix4::operator*(FVector4 vector)
{
	return FVector4
	(
		(Matrix[0] * vector[0] + Matrix[4] * vector[1] + Matrix[8] * vector[2] + Matrix[12] * vector[3]),
		(Matrix[1] * vector[0] + Matrix[5] * vector[1] + Matrix[9] * vector[2] + Matrix[13] * vector[3]),
		(Matrix[2] * vector[0] + Matrix[6] * vector[1] + Matrix[10] * vector[2] + Matrix[14] * vector[3]),
		(Matrix[3] * vector[0] + Matrix[7] * vector[1] + Matrix[11] * vector[2] + Matrix[15] * vector[3])
	);
}

void FMatrix4::operator*=(FMatrix4* matrix)
{
	*this = *this * *matrix;
}

void FMatrix4::operator*=(FMatrix4 matrix)
{
	*this *= matrix;
}

void FMatrix4::operator*=(float Multiplier)
{
	for (int i = 0; i < 16; i++)
	{
		Matrix[i] *= Multiplier;
	}
}

///Functions

//Returns the Determinant of Matrix Matrix
float FMatrix4::Det()
{
	float calculation, result = 1;
	FMatrix tempMatrix = FMatrix(*this);

	for (int i = 0; i < 4; i++) 
	{
		for (int k = i + 1; k < 4; k++) 
		{
			calculation = tempMatrix[k+ 4*i] / tempMatrix[i+ 4*i];
			
			for (int j = i; j < 4; j++)
			{
				tempMatrix[k+ 4*j] = tempMatrix[k + 4*j] - calculation * tempMatrix[i + 4*j];
			}
		}
	}

	for (int i = 0; i < 4; i++) result *= tempMatrix[i + 4*i];
	return result;
}

//Returns the iverse matrix to Matrix matrix
FMatrix4 FMatrix4::GetInverse()//TODO Fix GetInverse
{
	if (Det() == 0) return FMatrix();

	FMatrix inverse;

	inverse[0] =	Matrix[5] * Matrix[10] * Matrix[15] -
					Matrix[5] * Matrix[11] * Matrix[14] -
					Matrix[9] * Matrix[6] * Matrix[15] +
					Matrix[9] * Matrix[7] * Matrix[14] +
					Matrix[13] * Matrix[6] * Matrix[11] -
					Matrix[13] * Matrix[7] * Matrix[10];

	inverse[4] =	-Matrix[4] * Matrix[10] * Matrix[15] +
					Matrix[4] * Matrix[11] * Matrix[14] +
					Matrix[8] * Matrix[6] * Matrix[15] -
					Matrix[8] * Matrix[7] * Matrix[14] -
					Matrix[12] * Matrix[6] * Matrix[11] +
					Matrix[12] * Matrix[7] * Matrix[10];

	inverse[8] =	Matrix[4] * Matrix[9] * Matrix[15] -
					Matrix[4] * Matrix[11] * Matrix[13] -
					Matrix[8] * Matrix[5] * Matrix[15] +
					Matrix[8] * Matrix[7] * Matrix[13] +
					Matrix[12] * Matrix[5] * Matrix[11] -
					Matrix[12] * Matrix[7] * Matrix[9];

	inverse[12] =	-Matrix[4] * Matrix[9] * Matrix[14] +
					Matrix[4] * Matrix[10] * Matrix[13] +
					Matrix[8] * Matrix[5] * Matrix[14] -
					Matrix[8] * Matrix[6] * Matrix[13] -
					Matrix[12] * Matrix[5] * Matrix[10] +
					Matrix[12] * Matrix[6] * Matrix[9];

	inverse[1] =	-Matrix[1] * Matrix[10] * Matrix[15] +
					Matrix[1]  * Matrix[11] * Matrix[14] +
					Matrix[9]  * Matrix[2] * Matrix[15] -
					Matrix[9]  * Matrix[3] * Matrix[14] -
					Matrix[13] * Matrix[2] * Matrix[11] +
					Matrix[13] * Matrix[3] * Matrix[10];

	inverse[5] =	Matrix[0]  * Matrix[10] * Matrix[15] -
					Matrix[0]  * Matrix[11] * Matrix[14] -
					Matrix[8]  * Matrix[2] * Matrix[15] +
					Matrix[8]  * Matrix[3] * Matrix[14] +
					Matrix[12] * Matrix[2] * Matrix[11] -
					Matrix[12] * Matrix[3] * Matrix[10];

	inverse[9] =	-Matrix[0] * Matrix[9] * Matrix[15] +
					Matrix[0]  * Matrix[11] * Matrix[13] +
					Matrix[8]  * Matrix[1] * Matrix[15] -
					Matrix[8]  * Matrix[3] * Matrix[13] -
					Matrix[12] * Matrix[1] * Matrix[11] +
					Matrix[12] * Matrix[3] * Matrix[9];

	inverse[13] =	Matrix[0]  * Matrix[9] * Matrix[14] -
					Matrix[0]  * Matrix[10] * Matrix[13] -
					Matrix[8]  * Matrix[1] * Matrix[14] +
					Matrix[8]  * Matrix[2] * Matrix[13] +
					Matrix[12] * Matrix[1] * Matrix[10] -
					Matrix[12] * Matrix[2] * Matrix[9];

	inverse[2] =	Matrix[1]  * Matrix[6] * Matrix[15] -
					Matrix[1]  * Matrix[7] * Matrix[14] -
					Matrix[5]  * Matrix[2] * Matrix[15] +
					Matrix[5]  * Matrix[3] * Matrix[14] +
					Matrix[13] * Matrix[2] * Matrix[7] -
					Matrix[13] * Matrix[3] * Matrix[6];

	inverse[6] =	-Matrix[0] * Matrix[6] * Matrix[15] +
					Matrix[0]  * Matrix[7] * Matrix[14] +
					Matrix[4]  * Matrix[2] * Matrix[15] -
					Matrix[4]  * Matrix[3] * Matrix[14] -
					Matrix[12] * Matrix[2] * Matrix[7] +
					Matrix[12] * Matrix[3] * Matrix[6];

	inverse[10] =	Matrix[0]  * Matrix[5] * Matrix[15] -
					Matrix[0]  * Matrix[7] * Matrix[13] -
					Matrix[4]  * Matrix[1] * Matrix[15] +
					Matrix[4]  * Matrix[3] * Matrix[13] +
					Matrix[12] * Matrix[1] * Matrix[7] -
					Matrix[12] * Matrix[3] * Matrix[5];

	inverse[14] =	-Matrix[0] * Matrix[5] * Matrix[14] +
					Matrix[0]  * Matrix[6] * Matrix[13] +
					Matrix[4]  * Matrix[1] * Matrix[14] -
					Matrix[4]  * Matrix[2] * Matrix[13] -
					Matrix[12] * Matrix[1] * Matrix[6] +
					Matrix[12] * Matrix[2] * Matrix[5];

	inverse[3] =	-Matrix[1] * Matrix[6] * Matrix[11] +
					Matrix[1]  * Matrix[7] * Matrix[10] +
					Matrix[5]  * Matrix[2] * Matrix[11] -
					Matrix[5]  * Matrix[3] * Matrix[10] -
					Matrix[9]  * Matrix[2] * Matrix[7] +
					Matrix[9]  * Matrix[3] * Matrix[6];

	inverse[7] =	Matrix[0] * Matrix[6] * Matrix[11] -
					Matrix[0] * Matrix[7] * Matrix[10] -
					Matrix[4] * Matrix[2] * Matrix[11] +
					Matrix[4] * Matrix[3] * Matrix[10] +
					Matrix[8] * Matrix[2] * Matrix[7] -
					Matrix[8] * Matrix[3] * Matrix[6];

	inverse[11] =	-Matrix[0] * Matrix[5] * Matrix[11] +
					Matrix[0]  * Matrix[7] * Matrix[9] +
					Matrix[4]  * Matrix[1] * Matrix[11] -
					Matrix[4]  * Matrix[3] * Matrix[9] -
					Matrix[8]  * Matrix[1] * Matrix[7] +
					Matrix[8]  * Matrix[3] * Matrix[5];

	inverse[15] =	Matrix[0] * Matrix[5] * Matrix[10] -
					Matrix[0] * Matrix[6] * Matrix[9] -
					Matrix[4] * Matrix[1] * Matrix[10] +
					Matrix[4] * Matrix[2] * Matrix[9] +
					Matrix[8] * Matrix[1] * Matrix[6] -
					Matrix[8] * Matrix[2] * Matrix[5];

	return inverse;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix4::Invert()
{
	if(Det() != 0) *this = GetInverse();
}

//Makes Matrix Matrix an Identity matrix
void FMatrix4::SetToIdentity()
{
	Matrix[0] = 1.0f; Matrix[4] = 0.0f; Matrix[8] = 0.0f; Matrix[12] = 0.0f;
	Matrix[1] = 0.0f; Matrix[5] = 1.0f; Matrix[9] = 0.0f; Matrix[13] = 0.0f;
	Matrix[2] = 0.0f; Matrix[6] = 0.0f; Matrix[10] = 1.0f; Matrix[14] = 0.0f;
	Matrix[3] = 0.0f; Matrix[7] = 0.0f; Matrix[11] = 0.0f; Matrix[15] = 1.0f;
}

//Makes Matrix Matrix a Rotation Matrix on Z axis with given degrees
void FMatrix4::SetToRotationMatrix(eRotationAxis axis, float angle, bool isAngleRadian)
{
	this->SetToIdentity();

	if (!isAngleRadian && angle == 90)//Corrects a calculation mistake that does not see sin(90) as 0 due to conversion loss, but rather a extremely small number.
	{
		angle *= 0.174533f;//Converts input from degrees to radians

		switch (axis)
		{
		case X:
			Matrix[5] = cosf(angle);
			Matrix[9] = 0;
			Matrix[6] = 0;
			Matrix[10] = cosf(angle);
			break;

		case Y:
			Matrix[0] = cosf(angle);
			Matrix[8] = 0;
			Matrix[2] = 0;
			Matrix[10] = cosf(angle);
			break;

		case Z:
			Matrix[0] = cosf(angle);
			Matrix[4] = 0;
			Matrix[1] = 0;
			Matrix[5] = cosf(angle);
			break;
		}
	}
	else
	{
		if (!isAngleRadian) angle *= 0.174533f;//Converts input from degrees to radians

		switch (axis)
		{
		case X:
			Matrix[5] = cosf(angle);
			Matrix[9] = -sinf(angle);
			Matrix[6] = sinf(angle);
			Matrix[10] = cosf(angle);
			break;

		case Y:
			Matrix[0] = cosf(angle);
			Matrix[8] = -sinf(angle);
			Matrix[2] = sinf(angle);
			Matrix[10] = cosf(angle);
			break;

		case Z:
			Matrix[0] = cosf(angle);
			Matrix[4] = -sinf(angle);
			Matrix[1] = sinf(angle);
			Matrix[5] = cosf(angle);
			break;
		}
	}

}

//Makes Matrix Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
void FMatrix4::SetToScalingMatrix(float scaleX, float scaleY, float scaleZ)
{
	SetToIdentity();

	Matrix[0] = scaleX;
	Matrix[5] = scaleY;
	Matrix[10] = scaleZ;
}

void FMatrix4::SetToTranslationMatrix(float X, float Y, float Z)
{
	SetToIdentity();

	Matrix[12] = X;
	Matrix[13] = Y;
	Matrix[14] = Z;
}

//Swaps rows and Columns
void FMatrix4::Transpose()
{
	FMatrix4 tempMatrix = FMatrix4(*Matrix);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i + 4*j] = tempMatrix[j + 4*i];
		}
	}
}

//Sets all elements of the matrix to a given value
void FMatrix4::SetAllElements(float value)
{
	for (int i = 0; i < 16; i++)
	{
		Matrix[i] = value;
	}
}