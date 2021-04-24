#include "FMatrix4.h"

#include "FVector4.h"
#include "FVector3.h"
#include "FMatrix.h"
#include "FQuaternion.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include<iostream>

///Constructors

//Default contructor, initilizes to an identity matrix
FMatrix4::FMatrix4()
{
	Colum[0][0] = 1.0f;
	Colum[1][0] = 0.0f;
	Colum[2][0] = 0.0f;
	Colum[3][0] = 0.0f;

	Colum[0][1] = 0.0f;
	Colum[1][1] = 1.0f;
	Colum[2][1] = 0.0f;
	Colum[3][1] = 0.0f;

	Colum[0][2] = 0.0f;
	Colum[1][2] = 0.0f;
	Colum[2][2] = 1.0f;
	Colum[3][2] = 0.0f;

	Colum[0][3] = 0.0f;
	Colum[1][3] = 0.0f;
	Colum[2][3] = 0.0f;
	Colum[3][3] = 1.0f;
}

FMatrix4::FMatrix4(FMatrix matrix)
{
	for (int i = 0; i < 4; i++)
	{
		Colum[i] = matrix[i];
	}
}

FMatrix4::FMatrix4(float value)
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		Colum[i][j] = value;
	}
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix4::FMatrix4(const FVector4& row1, const FVector4& row2, const FVector4& row3, const FVector4& row4)
{
	Colum[0][0] = row1.X;
	Colum[1][0] = row1.Y;
	Colum[2][0] = row1.Z;
	Colum[3][0] = row1.W;

	Colum[0][1] = row2.X;
	Colum[1][1] = row2.Y;
	Colum[2][1] = row2.Z;
	Colum[3][1] = row2.W;

	Colum[0][2] = row3.X;
	Colum[1][2] = row3.Y;
	Colum[2][2] = row3.Z;
	Colum[3][2] = row3.W;

	Colum[0][3] = row4.X;
	Colum[1][3] = row4.Y;
	Colum[2][3] = row4.Z;
	Colum[3][3] = row4.W;
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix4::FMatrix4(float x, float y, float z)
{
	SetToIdentity();

	Colum[0][0] = x;
	Colum[1][1] = y;
	Colum[2][2] = z;
}

FMatrix4::FMatrix4(float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3)
{
	Colum[0][0] = a0;	Colum[1][0] = b0;	Colum[2][0] = c0;	Colum[3][0] = d0;
	Colum[0][1] = a1;	Colum[1][1] = b1;	Colum[2][1] = c1;	Colum[3][1] = d1;
	Colum[0][2] = a2;	Colum[1][2] = b2;	Colum[2][2] = c2;	Colum[3][2] = d2;
	Colum[0][3] = a3;	Colum[1][3] = b3;	Colum[2][3] = c3;	Colum[3][3] = d3;
}

//Initializes the Matrix to a rotation matrix given the angle and axis, optionaly, input true to give the angle in radians
FMatrix4::FMatrix4(eRotationAxis axis, float angle, bool isAngleRadian)
{
	if (!isAngleRadian) angle *= M_PI/180.0f;//Converts input from degrees to radians
	SetToIdentity();

	switch (axis)
	{
	case X:
		Colum[1][1] = cos(angle);
		Colum[2][1] = -sin(angle);
		Colum[1][2] = sin(angle);
		Colum[2][2] = cos(angle);
		break;

	case Y:
		Colum[0][0] = cos(angle);
		Colum[1][0] = -sin(angle);
		Colum[0][1] = sin(angle);
		Colum[1][1] = cos(angle);
		break;

	case Z:
		Colum[0][0] = cos(angle);
		Colum[2][0] = -sin(angle);
		Colum[0][2] = sin(angle);
		Colum[2][2] = cos(angle);
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
	for (int i = 0; i < 4; i++)
	{
			Colum[i] = matrix[i];
	}
}

void FMatrix4::operator=(FMatrix4* matrix)
{   
	for (int i = 0; i < 4; i++)
	{
		Colum[i] = *matrix[i];
	}
}

FMatrix4 FMatrix4::operator*(float multiplier) const
{
	FMatrix4 returnMatrix = *this;

	for (int i = 0; i < 4; i++)
	{
		returnMatrix[i] *= multiplier;
	}

	return returnMatrix;
}

FMatrix4 FMatrix4::operator*(FMatrix4 matrix) const
{
	return FMatrix4
	(
		(Colum[0][0] * matrix[0][0]) + (Colum[1][0] * matrix[0][1]) + (Colum[2][0] * matrix[0][2]) + (Colum[3][0] * matrix[0][3]),
		(Colum[0][1] * matrix[0][0]) + (Colum[1][1] * matrix[0][1]) + (Colum[2][1] * matrix[0][2]) + (Colum[3][1] * matrix[0][3]),
		(Colum[0][2] * matrix[0][0]) + (Colum[1][2] * matrix[0][1]) + (Colum[2][2] * matrix[0][2]) + (Colum[3][2] * matrix[0][3]),
		(Colum[0][3] * matrix[0][0]) + (Colum[1][3] * matrix[0][1]) + (Colum[2][3] * matrix[0][2]) + (Colum[3][3] * matrix[0][3]),
		(Colum[0][0] * matrix[1][0]) + (Colum[1][0] * matrix[1][1]) + (Colum[2][0] * matrix[1][2]) + (Colum[3][0] * matrix[1][3]),
		(Colum[0][1] * matrix[1][0]) + (Colum[1][1] * matrix[1][1]) + (Colum[2][1] * matrix[1][2]) + (Colum[3][1] * matrix[1][3]),
		(Colum[0][2] * matrix[1][0]) + (Colum[1][2] * matrix[1][1]) + (Colum[2][2] * matrix[1][2]) + (Colum[3][2] * matrix[1][3]),
		(Colum[0][3] * matrix[1][0]) + (Colum[1][3] * matrix[1][1]) + (Colum[2][3] * matrix[1][2]) + (Colum[3][3] * matrix[1][3]),
		(Colum[0][0] * matrix[2][0]) + (Colum[1][0] * matrix[2][1]) + (Colum[2][0] * matrix[2][2]) + (Colum[3][0] * matrix[2][3]),
		(Colum[0][1] * matrix[2][0]) + (Colum[1][1] * matrix[2][1]) + (Colum[2][1] * matrix[2][2]) + (Colum[3][1] * matrix[2][3]),
		(Colum[0][2] * matrix[2][0]) + (Colum[1][2] * matrix[2][1]) + (Colum[2][2] * matrix[2][2]) + (Colum[3][2] * matrix[2][3]),
		(Colum[0][3] * matrix[2][0]) + (Colum[1][3] * matrix[2][1]) + (Colum[2][3] * matrix[2][2]) + (Colum[3][3] * matrix[2][3]),
		(Colum[0][0] * matrix[3][0]) + (Colum[1][0] * matrix[3][1]) + (Colum[2][0] * matrix[3][2]) + (Colum[3][0] * matrix[3][3]),
		(Colum[0][1] * matrix[3][0]) + (Colum[1][1] * matrix[3][1]) + (Colum[2][1] * matrix[3][2]) + (Colum[3][1] * matrix[3][3]),
		(Colum[0][2] * matrix[3][0]) + (Colum[1][2] * matrix[3][1]) + (Colum[2][2] * matrix[3][2]) + (Colum[3][2] * matrix[3][3]),
		(Colum[0][3] * matrix[3][0]) + (Colum[1][3] * matrix[3][1]) + (Colum[2][3] * matrix[3][2]) + (Colum[3][3] * matrix[3][3])
	);
}

FMatrix4 FMatrix4::operator*(FMatrix4* matrix) const
{
	return (*this * *matrix);
}

FVector3 FMatrix4::operator*(FVector3 vector) const
{
	FVector3 ReturnVector = FVector3();

	ReturnVector.X = Colum[0][0] * vector.X + Colum[1][0] * vector.Y + Colum[2][0] * vector.Z;
	ReturnVector.Y = Colum[0][1] * vector.X + Colum[1][1] * vector.Y + Colum[2][1] * vector.Z;
	ReturnVector.Z = Colum[0][2] * vector.X + Colum[1][2] * vector.Y + Colum[2][2] * vector.Z;

	return ReturnVector;
}

FVector4 FMatrix4::operator*( FVector4* vector) const
{

	return FVector4
	(
		(Colum[0][0] * *vector[0] + Colum[1][0] * *vector[1] + Colum[2][0] * *vector[2] + Colum[3][0] * *vector[3]),
		(Colum[0][1] * *vector[0] + Colum[1][1] * *vector[1] + Colum[2][1] * *vector[2] + Colum[3][1] * *vector[3]),
		(Colum[0][2] * *vector[0] + Colum[1][2] * *vector[1] + Colum[2][2] * *vector[2] + Colum[3][2] * *vector[3]),
		(Colum[0][3] * *vector[0] + Colum[1][3] * *vector[1] + Colum[2][3] * *vector[2] + Colum[3][3] * *vector[3])
	);
}

FVector4 FMatrix4::operator*(FVector4 vector) const
{
	return FVector4
	(
		(Colum[0][0] * vector[0] + Colum[1][0] * vector[1] + Colum[2][0] * vector[2] + Colum[3][0] * vector[3]),
		(Colum[0][1] * vector[0] + Colum[1][1] * vector[1] + Colum[2][1] * vector[2] + Colum[3][1] * vector[3]),
		(Colum[0][2] * vector[0] + Colum[1][2] * vector[1] + Colum[2][2] * vector[2] + Colum[3][2] * vector[3]),
		(Colum[0][3] * vector[0] + Colum[1][3] * vector[1] + Colum[2][3] * vector[2] + Colum[3][3] * vector[3])
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

void FMatrix4::operator*=(float multiplier)
{
	for (int i = 0; i < 4; i++)
	{
		Colum[i] *= multiplier;
	}
}

///Functions

//Returns the Determinant of Matrix Matrix
float FMatrix4::Det()
{
	float calculation, result = 1;
	FMatrix4 tempMatrix = *this;

	for (int i = 0; i < 4; i++) 
	{
		for (int k = i + 1; k < 4; k++) 
		{
			calculation = tempMatrix[k][i] / tempMatrix[i][i];
			
			for (int j = i; j < 4; j++)
			{
				tempMatrix[k][j] = tempMatrix[k][j] - calculation * tempMatrix[i][j];
			}
		}
	}

	for (int i = 0; i < 4; i++) result *= tempMatrix[i][i];
	return result;
}

//Returns the iverse matrix to Matrix matrix
FMatrix4 FMatrix4::GetInverse()//TODO Fix GetInverse
{
	if (Det() == 0) return FMatrix();

	FMatrix inverse;

	inverse[0] =	Colum[1][1] * Colum[2][2] * Colum[3][3] -
					Colum[1][1] * Colum[2][3] * Colum[3][2] -
					Colum[2][1] * Colum[1][2] * Colum[3][3] +
					Colum[2][1] * Colum[1][3] * Colum[3][2] +
					Colum[3][1] * Colum[1][2] * Colum[2][3] -
					Colum[3][1] * Colum[1][3] * Colum[2][2];

	inverse[4] =	-Colum[1][0] * Colum[2][2] * Colum[3][3] +
					Colum[1][0] * Colum[2][3] * Colum[3][2] +
					Colum[2][0] * Colum[1][2] * Colum[3][3] -
					Colum[2][0] * Colum[1][3] * Colum[3][2] -
					Colum[3][0] * Colum[1][2] * Colum[2][3] +
					Colum[3][0] * Colum[1][3] * Colum[2][2];

	inverse[8] =	Colum[1][0] * Colum[2][1] * Colum[3][3] -
					Colum[1][0] * Colum[2][3] * Colum[3][1] -
					Colum[2][0] * Colum[1][1] * Colum[3][3] +
					Colum[2][0] * Colum[1][3] * Colum[3][1] +
					Colum[3][0] * Colum[1][1] * Colum[2][3] -
					Colum[3][0] * Colum[1][3] * Colum[2][1];

	inverse[12] =	-Colum[1][0] * Colum[2][1] * Colum[3][2] +
					Colum[1][0] * Colum[2][2] * Colum[3][1] +
					Colum[2][0] * Colum[1][1] * Colum[3][2] -
					Colum[2][0] * Colum[1][2] * Colum[3][1] -
					Colum[3][0] * Colum[1][1] * Colum[2][2] +
					Colum[3][0] * Colum[1][2] * Colum[2][1];

	inverse[1] =	-Colum[0][1] * Colum[2][2] * Colum[3][3] +
					Colum[0][1]  * Colum[2][3] * Colum[3][2] +
					Colum[2][1]  * Colum[0][2] * Colum[3][3] -
					Colum[2][1]  * Colum[0][3] * Colum[3][2] -
					Colum[3][1] * Colum[0][2] * Colum[2][3] +
					Colum[3][1] * Colum[0][3] * Colum[2][2];

	inverse[5] =	Colum[0][0]  * Colum[2][2] * Colum[3][3] -
					Colum[0][0]  * Colum[2][3] * Colum[3][2] -
					Colum[2][0]  * Colum[0][2] * Colum[3][3] +
					Colum[2][0]  * Colum[0][3] * Colum[3][2] +
					Colum[3][0] * Colum[0][2] * Colum[2][3] -
					Colum[3][0] * Colum[0][3] * Colum[2][2];

	inverse[9] =	-Colum[0][0] * Colum[2][1] * Colum[3][3] +
					Colum[0][0]  * Colum[2][3] * Colum[3][1] +
					Colum[2][0]  * Colum[0][1] * Colum[3][3] -
					Colum[2][0]  * Colum[0][3] * Colum[3][1] -
					Colum[3][0] * Colum[0][1] * Colum[2][3] +
					Colum[3][0] * Colum[0][3] * Colum[2][1];

	inverse[13] =	Colum[0][0]  * Colum[2][1] * Colum[3][2] -
					Colum[0][0]  * Colum[2][2] * Colum[3][1] -
					Colum[2][0]  * Colum[0][1] * Colum[3][2] +
					Colum[2][0]  * Colum[0][2] * Colum[3][1] +
					Colum[3][0] * Colum[0][1] * Colum[2][2] -
					Colum[3][0] * Colum[0][2] * Colum[2][1];

	inverse[2] =	Colum[0][1]  * Colum[1][2] * Colum[3][3] -
					Colum[0][1]  * Colum[1][3] * Colum[3][2] -
					Colum[1][1]  * Colum[0][2] * Colum[3][3] +
					Colum[1][1]  * Colum[0][3] * Colum[3][2] +
					Colum[3][1] * Colum[0][2] * Colum[1][3] -
					Colum[3][1] * Colum[0][3] * Colum[1][2];

	inverse[6] =	-Colum[0][0] * Colum[1][2] * Colum[3][3] +
					Colum[0][0]  * Colum[1][3] * Colum[3][2] +
					Colum[1][0]  * Colum[0][2] * Colum[3][3] -
					Colum[1][0]  * Colum[0][3] * Colum[3][2] -
					Colum[3][0] * Colum[0][2] * Colum[1][3] +
					Colum[3][0] * Colum[0][3] * Colum[1][2];

	inverse[10] =	Colum[0][0]  * Colum[1][1] * Colum[3][3] -
					Colum[0][0]  * Colum[1][3] * Colum[3][1] -
					Colum[1][0]  * Colum[0][1] * Colum[3][3] +
					Colum[1][0]  * Colum[0][3] * Colum[3][1] +
					Colum[3][0] * Colum[0][1] * Colum[1][3] -
					Colum[3][0] * Colum[0][3] * Colum[1][1];

	inverse[14] =	-Colum[0][0] * Colum[1][1] * Colum[3][2] +
					Colum[0][0]  * Colum[1][2] * Colum[3][1] +
					Colum[1][0]  * Colum[0][1] * Colum[3][2] -
					Colum[1][0]  * Colum[0][2] * Colum[3][1] -
					Colum[3][0] * Colum[0][1] * Colum[1][2] +
					Colum[3][0] * Colum[0][2] * Colum[1][1];

	inverse[3] =	-Colum[0][1] * Colum[1][2] * Colum[2][3] +
					Colum[0][1]  * Colum[1][3] * Colum[2][2] +
					Colum[1][1]  * Colum[0][2] * Colum[2][3] -
					Colum[1][1]  * Colum[0][3] * Colum[2][2] -
					Colum[2][1]  * Colum[0][2] * Colum[1][3] +
					Colum[2][1]  * Colum[0][3] * Colum[1][2];

	inverse[7] =	Colum[0][0] * Colum[1][2] * Colum[2][3] -
					Colum[0][0] * Colum[1][3] * Colum[2][2] -
					Colum[1][0] * Colum[0][2] * Colum[2][3] +
					Colum[1][0] * Colum[0][3] * Colum[2][2] +
					Colum[2][0] * Colum[0][2] * Colum[1][3] -
					Colum[2][0] * Colum[0][3] * Colum[1][2];

	inverse[11] =	-Colum[0][0] * Colum[1][1] * Colum[2][3] +
					Colum[0][0]  * Colum[1][3] * Colum[2][1] +
					Colum[1][0]  * Colum[0][1] * Colum[2][3] -
					Colum[1][0]  * Colum[0][3] * Colum[2][1] -
					Colum[2][0]  * Colum[0][1] * Colum[1][3] +
					Colum[2][0]  * Colum[0][3] * Colum[1][1];

	inverse[15] =	Colum[0][0] * Colum[1][1] * Colum[2][2] -
					Colum[0][0] * Colum[1][2] * Colum[2][1] -
					Colum[1][0] * Colum[0][1] * Colum[2][2] +
					Colum[1][0] * Colum[0][2] * Colum[2][1] +
					Colum[2][0] * Colum[0][1] * Colum[1][2] -
					Colum[2][0] * Colum[0][2] * Colum[1][1];

	return inverse;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix4::Invert()
{
	if(Det() != 0) *this = GetInverse();
}

void FMatrix4::Print()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f\t", Colum[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//Makes Matrix Matrix an Identity matrix
void FMatrix4::SetToIdentity()
{
	Colum[0][0] = 1.0f; Colum[1][0] = 0.0f; Colum[2][0] = 0.0f; Colum[3][0] = 0.0f;
	Colum[0][1] = 0.0f; Colum[1][1] = 1.0f; Colum[2][1] = 0.0f; Colum[3][1] = 0.0f;
	Colum[0][2] = 0.0f; Colum[1][2] = 0.0f; Colum[2][2] = 1.0f; Colum[3][2] = 0.0f;
	Colum[0][3] = 0.0f; Colum[1][3] = 0.0f; Colum[2][3] = 0.0f; Colum[3][3] = 1.0f;
}

FMatrix4 FMatrix4::GetRotationMatrix(float angle, float x, float y, float z, bool isAngleRadian)
{
	if (!isAngleRadian) angle *= M_PI/180.0f;//Converts input from degrees to radians

	float cosang, sinang, cosm;
	FVector3 rotAxis(x, y, z);
	rotAxis.Normalize();
	cosang = cos(angle);
	sinang = sin(angle);
	cosm = (1.0f - cosang);

	FMatrix4 returnMatrix;

	returnMatrix[0][0] = (rotAxis.X * rotAxis.X * cosm) + cosang;
	returnMatrix[0][1] = (rotAxis.X * rotAxis.Y * cosm) + (rotAxis.Z * sinang);
	returnMatrix[0][2] = (rotAxis.X * rotAxis.Z * cosm) - (rotAxis.Y * sinang);
	returnMatrix[0][3] = 0.0;
	
	returnMatrix[1][0] = (rotAxis.X * rotAxis.Y * cosm) - (rotAxis.Z * sinang);
	returnMatrix[1][1] = (rotAxis.Y * rotAxis.Y * cosm) + cosang;
	returnMatrix[1][2] = (rotAxis.Y * rotAxis.Z * cosm) + (rotAxis.X * sinang);
	returnMatrix[1][3] = 0.0;
	
	returnMatrix[2][0] = (rotAxis.X * rotAxis.Z * cosm) + (rotAxis.Y * sinang);
	returnMatrix[2][1] = (rotAxis.Y * rotAxis.Z * cosm) - (rotAxis.X * sinang);
	returnMatrix[2][2] = (rotAxis.Z * rotAxis.Z * cosm) + cosang;
	returnMatrix[2][3] = 0.0;
	
	returnMatrix[3][0] = 0.0;
	returnMatrix[3][1] = 0.0;
	returnMatrix[3][2] = 0.0;
	returnMatrix[3][3] = 1.0;

	return returnMatrix;
}

FMatrix4 FMatrix4::GetRotationMatrix(const FQuaternion& rotation)
{
	FMatrix4 matrixA
	(
		rotation.W, rotation.Z, -rotation.Y, rotation.X,
		-rotation.Z, rotation.W, rotation.X, rotation.Y,
		rotation.Y, -rotation.X, rotation.W, rotation.Z,
		-rotation.X, -rotation.Y, -rotation.Z, rotation.W
	);

	FMatrix4 matrixB
	(
		rotation.W, rotation.Z, -rotation.Y, -rotation.X,
		-rotation.Z, rotation.W, rotation.X, -rotation.Y,
		rotation.Y, -rotation.X, rotation.W, -rotation.Z,
		rotation.X, rotation.Y, rotation.Z, rotation.W
	);

	auto check = matrixA * matrixB;
	return check;
}

void FMatrix4::SetToRotationMatrix(float angle, float x, float y, float z, bool isAngleRadian)
{
	*this = GetRotationMatrix(angle, x, y, z, isAngleRadian);
}

void FMatrix4::SetToLookAtMatrix(const FVector3& eye, const FVector3& target, const FVector3& up)
{
	*this = GetLookAtMatrix(eye, target, up);
}

FMatrix4 FMatrix4::GetLookAtMatrix(const FVector3& eye, const FVector3& target, const FVector3& up)
{
	FMatrix4 returnMatrix;
	FVector3 forward = (target - eye);
	forward.Normalize();
	FVector3 right = forward.CrossProduct(up).GetNormal();
	FVector3 upVec = right.CrossProduct(forward).GetNormal();

	returnMatrix[0][0] = right.X;
	returnMatrix[1][0] = right.Y;
	returnMatrix[2][0] = right.Z;
	
	returnMatrix[0][1] = upVec.X;
	returnMatrix[1][1] = upVec.Y;
	returnMatrix[2][1] = upVec.Z;
	
	returnMatrix[0][2] = -forward.X;
	returnMatrix[1][2] = -forward.Y;
	returnMatrix[2][2] = -forward.Z;
	
	returnMatrix[3][0] = -(right * eye);
	returnMatrix[3][1] = -(upVec * eye);
	returnMatrix[3][2] = (forward * eye);

	return returnMatrix;
}

void FMatrix4::SetToPerspectiveMatrix(float fieldOfView, float aspect, float zNear, float zFar, bool isRadian)
{
	*this = GetPerspectiveMatrix(fieldOfView, aspect, zNear, zFar, isRadian);
}

FMatrix4 FMatrix4::GetPerspectiveMatrix(float fieldOfView, float aspect, float zNear, float zFar, bool isRadian)
{
	if (!isRadian) fieldOfView *= M_PI / 180.0f;

	float const tanHalfFovy = tan(fieldOfView / 2);
	FMatrix4 returnMatrix = FMatrix4(0);
	
	returnMatrix[0][0] = 1.0f / (aspect * tanHalfFovy);
	returnMatrix[1][1] = 1.0f / (tanHalfFovy);
	returnMatrix[2][2] = - (zFar + zNear) / (zFar - zNear);
	returnMatrix[2][3] = - 1.0f;
	returnMatrix[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
	
	return returnMatrix;
}

//Makes Matrix Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
void FMatrix4::SetToScalingMatrix(float scaleX, float scaleY, float scaleZ)
{
	SetToIdentity();

	Colum[0][0] = scaleX;
	Colum[1][1] = scaleY;
	Colum[2][2] = scaleZ;
}

FMatrix4 FMatrix4::GetScalingMatrix(const FVector3& scale)
{
	FMatrix4 returnMatrix;
	returnMatrix.Colum[0][0] = scale.X;
	returnMatrix.Colum[1][1] = scale.Y;
	returnMatrix.Colum[2][2] = scale.Z;
	return returnMatrix;
}

void FMatrix4::SetToTranslationMatrix(float x, float y, float z)
{
	SetToIdentity();

	Colum[3][0] = x;
	Colum[3][1] = y;
	Colum[3][2] = z;
}

FMatrix4 FMatrix4::GetTranslationMatrix(const FVector3& position)
{
	FMatrix4 returnMatrix;
	returnMatrix.Colum[3][0] = position.X;
	returnMatrix.Colum[3][1] = position.Y;
	returnMatrix.Colum[3][2] = position.Z;
	return returnMatrix;
}

FMatrix4 FMatrix4::GetViewMatrix(const FQuaternion& rotation, const FVector3& position)
{
	FVector3 forward = rotation.GetForwardVector();
	FVector3 right = rotation.GetRightVector();
	FVector3 up = rotation.GetUpVector();

	FMatrix4 result =
		FMatrix4
		(
			forward.X, forward.Y, forward.Z, 0,
			up.X, up.Y, up.Z, 0,
			right.X, right.Y, right.Z, 0,
			position.X, position.Y, position.Z, -1
		);

	return result;
}

//Swaps rows and Columns
void FMatrix4::Transpose()
{
	FMatrix4 tempMatrix = *this;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Colum[i][j] = tempMatrix[j][i];
		}
	}
}

//Sets all elements of the matrix to a given value
void FMatrix4::SetAllElements(float value)
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Colum[i][j] = value;
		}
	}
}