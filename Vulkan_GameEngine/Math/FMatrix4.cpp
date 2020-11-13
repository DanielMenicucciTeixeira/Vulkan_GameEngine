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
	this[0][0] = 1.0f;
	this[0][1] = 0.0f;
	this[0][2] = 0.0f;
	this[0][3] = 0.0f;

	this[1][0] = 0.0f;
	this[1][1] = 1.0f;
	this[1][2] = 0.0f;
	this[1][3] = 0.0f;

	this[2][0] = 0.0f;
	this[2][1] = 0.0f;
	this[2][2] = 1.0f;
	this[2][3] = 0.0f;

	this[3][0] = 0.0f;
	this[3][1] = 0.0f;
	this[3][2] = 0.0f;
	this[3][3] = 1.0f;
}

FMatrix4::FMatrix4(FMatrix matrix)
{
	SetToIdentity();

	for (int i = 0; i < 16; i++)
	{
		this[i] = matrix[i];
	}
}

FMatrix4::FMatrix4(float value)
{
	for (int i = 0; i < 16; i++)
	{
		this[i] = value;
	}
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix4::FMatrix4(const FVector4& row1, const FVector4& row2, const FVector4& row3, const FVector4& row4)
{
	this[0][0] = row1.VectorArray[0];
	this[0][1] = row1.VectorArray[1];
	this[0][2] = row1.VectorArray[2];
	this[0][3] = row1.VectorArray[3];

	this[1][0] = row2.VectorArray[0];
	this[1][1] = row2.VectorArray[1];
	this[1][2] = row2.VectorArray[2];
	this[1][3] = row2.VectorArray[3];

	this[2][0] = row3.VectorArray[0];
	this[2][1] = row3.VectorArray[1];
	this[2][2] = row3.VectorArray[2];
	this[2][3] = row3.VectorArray[3];

	this[3][0] = row4.VectorArray[0];
	this[3][1] = row4.VectorArray[1];
	this[3][2] = row4.VectorArray[2];
	this[3][3] = row4.VectorArray[3];
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix4::FMatrix4(float x, float y, float z)
{
	SetToIdentity();

	this[0][0] = x;
	this[1][1] = y;
	this[2][2] = z;
}

//Initializes the Matrix given a 3x3 Array
FMatrix4::FMatrix4(float matrix[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this[i][j] = matrix[i][j];
		}
	}
}

//Initializes the Matrix to a rotation matrix given the angle and axis, optionaly, input true to give the angle in radians
FMatrix4::FMatrix4(eRotationAxis axis, float angle, bool isAngleRadian)
{
	if (!isAngleRadian) angle *= 0.174533f;//Converts input from degrees to radians
	SetToIdentity();

	switch (axis)
	{
	case X:
		this[1][1] = cos(angle);
		this[1][2] = -sin(angle);
		this[2][1] = sin(angle);
		this[2][2] = cos(angle);
		break;

	case Y:
		this[0][0] = cos(angle);
		this[0][1] = -sin(angle);
		this[1][0] = sin(angle);
		this[1][1] = cos(angle);
		break;

	case Z:
		this[0][0] = cos(angle);
		this[0][2] = -sin(angle);
		this[2][0] = sin(angle);
		this[2][2] = cos(angle);
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
		for (int j = 0; j < 4; j++)
		{
			this[i][j] = this[i][j];
		}
	}
}

void FMatrix4::operator=(FMatrix4* matrix)
{
	*this = *matrix;
}

FMatrix4 FMatrix4::operator*(float Multiplier)
{
	FMatrix4* tempMatrix = new FMatrix4(*this);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tempMatrix[i][j] *= Multiplier;
		}
	}
	FMatrix4 returnMatrix = *tempMatrix;
	delete(tempMatrix);

	return returnMatrix;
}

FMatrix4 FMatrix4::operator*(FMatrix4* matrix)
{
	FMatrix4* tempMatrix = new FMatrix4(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)//The first two for loops will be looping through each element of the Return Matrix
		{
			for (int k = 0; k < 4; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				tempMatrix[i][j] += this[i][k] * matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}
	FMatrix4 returnMatrix = *tempMatrix;
	delete(tempMatrix);

	return returnMatrix;
}

FMatrix4 FMatrix4::operator*(FMatrix4 matrix)
{
	return (*this * &matrix);
}

FVector3 FMatrix4::operator*(FVector3 vector)
{
	FVector3 ReturnVector = FVector3();

	ReturnVector.VectorArray[0] = this[0][0] * vector.VectorArray[0] + this[0][1] * vector.VectorArray[1] + this[0][2] * vector.VectorArray[2];
	ReturnVector.VectorArray[1] = this[1][0] * vector.VectorArray[0] + this[1][1] * vector.VectorArray[1] + this[1][2] * vector.VectorArray[2];
	ReturnVector.VectorArray[2] = this[2][0] * vector.VectorArray[0] + this[2][1] * vector.VectorArray[1] + this[2][2] * vector.VectorArray[2];

	return ReturnVector;
}

FVector4 FMatrix4::operator*(FVector4* vector)
{
	FVector4 ReturnVector = FVector4(0);

	ReturnVector.VectorArray[0] = this[0][0] * vector->VectorArray[0] + this[0][1] * vector->VectorArray[1] + this[0][2] * vector->VectorArray[2] + this[0][3] * vector->VectorArray[3];
	ReturnVector.VectorArray[1] = this[1][0] * vector->VectorArray[0] + this[1][1] * vector->VectorArray[1] + this[1][2] * vector->VectorArray[2] + this[1][3] * vector->VectorArray[3];
	ReturnVector.VectorArray[2] = this[2][0] * vector->VectorArray[0] + this[2][1] * vector->VectorArray[1] + this[2][2] * vector->VectorArray[2] + this[2][3] * vector->VectorArray[3];
	ReturnVector.VectorArray[3] = this[3][0] * vector->VectorArray[0] + this[3][1] * vector->VectorArray[1] + this[3][2] * vector->VectorArray[2] + this[3][3] * vector->VectorArray[3];

	return ReturnVector;
}

void FMatrix4::operator*=(FMatrix4* matrix)
{
	FMatrix4* tempMatrix = new FMatrix4(0.0f);

	for (int i = 0; i < 4; i++)//The first two for loops will be looping through each element of the  tempMatrix
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				tempMatrix[i][j] += this[i][k] * matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	*this = *tempMatrix;
	delete(tempMatrix);
}

void FMatrix4::operator*=(FMatrix4 matrix)
{
	*this *= &matrix;
}

void FMatrix4::operator*=(float Multiplier)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this[i][j] *= Multiplier;
		}
	}
}

///Functions

//Returns the Determinant of this Matrix
float FMatrix4::Det()//TODO fix Det()
{
	float Det = 0;

	for (int i = 0; i < 3; i++)
	{
		Det += this[0][i] * (this[1][(i + 1) % 3] * this[2][(i + 2) % 3] - this[2][(i + 1) % 3] * this[1][(i + 2) % 3]);
	}

	return Det;
}

//Returns the iverse matrix to this matrix
FMatrix4 FMatrix4::GetInverse()
{
	if (Det() == 0) return FMatrix4();

	FMatrix4* tempMatrix = new FMatrix4();

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			//row and collumns are swapt so there is no need to transpose the matrix later
			tempMatrix[col][row] = ((this[(row + 1) % 3][(col + 1) % 3] * this[(row + 2) % 3][(col + 2) % 3]) - (this[(row + 1) % 3][(col + 2) % 3] * this[(row + 2) % 3][(col + 1) % 3])) / Det();
		}
	}
	
	FMatrix4 returnMatrix = *tempMatrix;
	delete(tempMatrix);

	return returnMatrix;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix4::Invert()
{
	*this = GetInverse();
}

//Prints Matrix to console
void FMatrix4::Print()
{
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			std::cout << this[row][col] << "\t";
		}
		std::cout << std::endl;
	}
}

//Makes this Matrix an Identity matrix
void FMatrix4::SetToIdentity()
{
	this[0][0] = 1.0f;
	this[0][1] = 0.0f;
	this[0][2] = 0.0f;
	this[0][3] = 0.0f;

	this[1][0] = 0.0f;
	this[1][1] = 1.0f;
	this[1][2] = 0.0f;
	this[1][3] = 0.0f;

	this[2][0] = 0.0f;
	this[2][1] = 0.0f;
	this[2][2] = 1.0f;
	this[2][3] = 0.0f;

	this[3][0] = 0.0f;
	this[3][1] = 0.0f;
	this[3][2] = 0.0f;
	this[3][3] = 1.0f;
}

//Makes this Matrix a Rotation Matrix on Z axis with given degrees
void FMatrix4::SetToRotationMatrix(eRotationAxis axis, float angle, bool isAngleRadian)
{
	this->SetToIdentity();

	if (!isAngleRadian && angle == 90)//Corrects a calculation mistake that does not see sin(90) as 0 due to conversion loss, but rather a extremely small number.
	{
		angle *= 0.174533f;//Converts input from degrees to radians

		switch (axis)
		{
		case X:
			this[1][1] = cosf(angle);
			this[1][2] = 0;
			this[2][1] = 0;
			this[2][2] = cosf(angle);
			break;

		case Y:
			this[0][0] = cosf(angle);
			this[0][2] = 0;
			this[2][0] = 0;
			this[2][2] = cosf(angle);
			break;

		case Z:
			this[0][0] = cosf(angle);
			this[0][1] = 0;
			this[1][0] = 0;
			this[1][1] = cosf(angle);
			break;
		}
	}
	else
	{
		if (!isAngleRadian) angle *= 0.174533f;//Converts input from degrees to radians

		switch (axis)
		{
		case X:
			this[1][1] = cosf(angle);
			this[1][2] = -sinf(angle);
			this[2][1] = sinf(angle);
			this[2][2] = cosf(angle);
			break;

		case Y:
			this[0][0] = cosf(angle);
			this[0][2] = -sinf(angle);
			this[2][0] = sinf(angle);
			this[2][2] = cosf(angle);
			break;

		case Z:
			this[0][0] = cosf(angle);
			this[0][1] = -sinf(angle);
			this[1][0] = sinf(angle);
			this[1][1] = cosf(angle);
			break;
		}
	}

}

//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
void FMatrix4::SetToScalingMatrix(float scaleX, float scaleY, float scaleZ)
{
	SetToIdentity();

	this[0][0] = scaleX;
	this[1][1] = scaleY;
	this[2][2] = scaleZ;
}

void FMatrix4::SetToTranslationMatrix(float X, float Y, float Z)
{
	SetToIdentity();

	this[0][3] = X;
	this[1][3] = Y;
	this[2][3] = Z;
}

//Swaps rows and Columns
void FMatrix4::Transpose()
{
	FMatrix4* tempMatrix = new FMatrix4(*this);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this[i][j] = tempMatrix[j][i];
		}
	}

	delete(tempMatrix);
}

//Sets all elements of the matrix to a given value
void FMatrix4::SetAllElements(float value)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this[i][j] = value;
		}
	}
}