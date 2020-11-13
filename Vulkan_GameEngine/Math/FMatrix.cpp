#include "FMatrix.h"
#include "FVector3.h"
#include <math.h>
#include<iostream>

///Constructors

//Default contructor, initilizes to an identity matrix
FMatrix::FMatrix()
{
	Matrix[0] = 1;	Matrix[3] = 0;	Matrix[6] = 0;
	Matrix[1] = 0;	Matrix[4] = 1;	Matrix[7] = 0;
	Matrix[2] = 0;	Matrix[5] = 0;	Matrix[8] = 1;

}

FMatrix::FMatrix(float value)
{
	for (int i = 0; i < 16; i++) Matrix[i] = value;
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix::FMatrix(FVector3 Row1, FVector3 Row2, FVector3 Row3)
{
	Matrix[0] = Row1.VectorArray[0];
	Matrix[3] = Row1.VectorArray[1];
	Matrix[6] = Row1.VectorArray[2];

	Matrix[1] = Row2.VectorArray[0];
	Matrix[4] = Row2.VectorArray[1];
	Matrix[7] = Row2.VectorArray[2];

	Matrix[2] = Row3.VectorArray[0];
	Matrix[5] = Row3.VectorArray[1];
	Matrix[8] = Row3.VectorArray[2];
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix::FMatrix(float x, float y, float z)
{
	this->SetToIdentity();

	Matrix[0] = x;
	Matrix[4] = y;
	Matrix[8] = z;
}

FMatrix::FMatrix(float matrix[9])
{
	for (int i = 0; i < 9; i++) Matrix[i] = matrix[i];
}

//Initializes the Matrix to a rotation matrix given the Angle and Axis, optionaly, input true to give the angle in radians
FMatrix::FMatrix(eRotationAxis Axis, float Angle, bool IsAngleRadian)
{
	if (!IsAngleRadian) Angle *= 0.174533f;//Converts input from degrees to radians
	this->SetToIdentity();

	switch (Axis)
	{
	case X:
		Matrix[4] = cos(Angle);
		Matrix[7] = -sin(Angle);
		Matrix[5] = sin(Angle);
		Matrix[8] = cos(Angle);
		break;

	case Y:
		Matrix[0] = cos(Angle);
		Matrix[3] = -sin(Angle);
		Matrix[1] = sin(Angle);
		Matrix[4] = cos(Angle);
		break;

	case Z:
		Matrix[0] = cos(Angle);
		Matrix[7] = -sin(Angle);
		Matrix[2] = sin(Angle);
		Matrix[8] = cos(Angle);
		break;
	}
}

///Destructors

FMatrix::~FMatrix()
{
}

///Operator Overloads

void FMatrix::operator=(FMatrix _Matrix)
{
	for (int i = 0; i < 9; i++)
	{
		Matrix[i] = _Matrix.Matrix[i];
	}
}

FMatrix FMatrix::operator*(float Multiplier)
{
	FMatrix returnMatrix = FMatrix(Matrix);

	for (int i = 0; i < 9; i++)
	{
		returnMatrix.Matrix[i] *= Multiplier;
	}

	return returnMatrix;
}

FMatrix FMatrix::operator*(FMatrix matrix)
{
	FMatrix result;
	for (int i = 0; i < 3; ++i) 
	{
		for (int j = 0; j < 3; ++j) 
		{
			result[i * 3 + j] = (Matrix[0 * 3 + j] * matrix[i * 3 + 0]) + (Matrix[1 * 3 + j] * matrix[i * 3 + 1]) + (Matrix[2 * 3 + j] * matrix[i * 3 + 2]);
		}
	}
	return result;
}

FMatrix FMatrix::operator*(FMatrix* matrix)
{

	FMatrix result;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result[i * 3 + j] = (Matrix[0 * 3 + j] * *matrix[i * 3 + 0]) + (Matrix[1 * 3 + j] * *matrix[i * 3 + 1]) + (Matrix[2 * 3 + j] * *matrix[i * 3 + 2]);
		}
	}
	return result;
}

FVector3 FMatrix::operator*(FVector3 vector)
{
	FVector3 ReturnVector = FVector3();

	ReturnVector.VectorArray[0] = Matrix[0] * vector.VectorArray[0] + Matrix[3] * vector.VectorArray[1] + Matrix[6] * vector.VectorArray[2];
	ReturnVector.VectorArray[1] = Matrix[1] * vector.VectorArray[0] + Matrix[4] * vector.VectorArray[1] + Matrix[7] * vector.VectorArray[2];
	ReturnVector.VectorArray[2] = Matrix[2] * vector.VectorArray[0] + Matrix[5] * vector.VectorArray[1] + Matrix[8] * vector.VectorArray[2];

	return ReturnVector;
}

void FMatrix::operator*=(FMatrix matrix)
{
	FMatrix result;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result[i * 3 + j] = (Matrix[0 * 3 + j] * matrix[i * 3 + 0]) + (Matrix[1 * 3 + j] * matrix[i * 3 + 1]) + (Matrix[2 * 3 + j] * matrix[i * 3 + 2]);
		}
	}

	*this = result;
}

void FMatrix::operator*=(float multiplier)
{
	for (int i = 0; i < 9; i++)
	{
		
		Matrix[i] *= multiplier;
	}
}

///Functions

//Returns the Determinant of this Matrix
float FMatrix::Det()
{
	float Det = 0;
	float matrix[3][3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			matrix[i][j] = Matrix[i + 3 * j];
		}
	}

	for (int i = 0; i < 3; i++)
	{
		Det += matrix[0][i] * (matrix[1][(i + 1) % 3] * matrix[2][(i + 2) % 3] - matrix[2][(i + 1) % 3] * matrix[1][(i + 2) % 3]);
	}

	return Det;
}

//Returns the iverse matrix to this matrix
FMatrix FMatrix::GetInverse()
{
	if (Det() == 0) return FMatrix();

	FMatrix returnMatrix = FMatrix();

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			//Row and collumns are swapt so there is no need to transpose the matrix later
			returnMatrix[col + 3*row] = ((this[((row + 1) % 3) + 3*((col + 1) % 3)] * this[((row + 2) % 3) + 3*((col + 2) % 3)]) - (this[((row + 1) % 3) + 3*((col + 2) % 3)] * this[((row + 2) % 3) + 3*((col + 1) % 3)]))/Det();
		}
	}

	return returnMatrix;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix::Invert()
{
	*this = GetInverse();
}

//Prints Matrix to console
void FMatrix::PrintMatrix()
{
	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			std::cout << this[row + 3*col] << "\t";
		}
		std::cout << std::endl;
	}
}

//Makes this Matrix an Identity matrix
void FMatrix::SetToIdentity()
{
	Matrix[0] = 1.0f;
	Matrix[3] = 0.0f;
	Matrix[6] = 0.0f;

	Matrix[1] = 0.0f;
	Matrix[4] = 1.0f;
	Matrix[7] = 0.0f;

	Matrix[2] = 0.0f;
	Matrix[5] = 0.0f;
	Matrix[8] = 1.0f;
}

//Makes this Matrix a Rotation Matrix on Z axis with given degrees
void FMatrix::SetToRotationMatrix(eRotationAxis Axis, float Angle, bool IsAngleRadian)
{
	this->SetToIdentity();

	if (!IsAngleRadian && Angle == 90)//Corrects a calculation mistake that does not see sin(90) as 0 due to conversion loss, but rather a extremely small number.
	{
		Angle *= 0.174533f;//Converts input from degrees to radians

		switch (Axis)
		{
		case X:
			Matrix[4] = cosf(Angle);
			Matrix[7] = 0;
			Matrix[5] = 0;
			Matrix[8] = cosf(Angle);
			break;

		case Y:
			Matrix[0] = cosf(Angle);
			Matrix[6] = 0;
			Matrix[2] = 0;
			Matrix[8] = cosf(Angle);
			break;

		case Z:
			Matrix[0] = cosf(Angle);
			Matrix[3] = 0;
			Matrix[1] = 0;
			Matrix[4] = cosf(Angle);
			break;
		}
	}
	else
	{
		if (!IsAngleRadian) Angle *= 0.174533f;//Converts input from degrees to radians

		switch (Axis)
		{
		case X:
			Matrix[4] = cosf(Angle);
			Matrix[7] = -sinf(Angle);
			Matrix[5] = sinf(Angle);
			Matrix[8] = cosf(Angle);
			break;

		case Y:
			Matrix[0] = cosf(Angle);
			Matrix[6] = -sinf(Angle);
			Matrix[2] = sinf(Angle);
			Matrix[8] = cosf(Angle);
			break;

		case Z:
			Matrix[0] = cosf(Angle);
			Matrix[3] = -sinf(Angle);
			Matrix[1] = sinf(Angle);
			Matrix[4] = cosf(Angle);
			break;
		}
	}

}

//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
void FMatrix::SetToScalingMatrix(float ScaleX, float ScaleY, float ScaleZ)
{
	SetToIdentity();

	Matrix[0] = ScaleX;
	Matrix[4] = ScaleY;
	Matrix[8] = ScaleZ;
}

void FMatrix::SetToTranslationMatrix(float X, float Y, float Z)
{
	SetToIdentity();
}

//Swaps Rows and Columns
void FMatrix::Transpose()
{
	FMatrix tempMatrix = FMatrix(Matrix);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i + 3*j] = tempMatrix[j + 3*i];
		}
	}
}

//Sets all elements of the matrix to a given value
void FMatrix::SetAllElements(float value)
{
	for (int i = 0; i < 9; i++)
	{
		Matrix[i] = value;
	}
}
