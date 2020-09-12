#include "FMatrix.h"
#include "FVector3.h"

#include <math.h>
#include<iostream>

///Constructors

//Default contructor, initilizes to an identity matrix
FMatrix::FMatrix()
{
	Matrix[0][0] = 1.0f;
	Matrix[0][1] = 0.0f;
	Matrix[0][2] = 0.0f;

	Matrix[1][0] = 0.0f;
	Matrix[1][1] = 1.0f;
	Matrix[1][2] = 0.0f;

	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;
}

FMatrix::FMatrix(float Value)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = Value;
		}
	}
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix::FMatrix(FVector3 Row1, FVector3 Row2, FVector3 Row3)
{
	Matrix[0][0] = Row1.x;
	Matrix[0][1] = Row1.y;
	Matrix[0][2] = Row1.z;

	Matrix[1][0] = Row2.x;
	Matrix[1][1] = Row2.y;
	Matrix[1][2] = Row2.z;

	Matrix[2][0] = Row3.x;
	Matrix[2][1] = Row3.y;
	Matrix[2][2] = Row3.z;
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix::FMatrix(float X, float Y, float Z)
{
	this->SetToIdentity();

	Matrix[0][0] = X;
	Matrix[1][1] = Y;
	Matrix[2][2] = Z;
}

//Initializes the Matrix given a 3x3 Array
FMatrix::FMatrix(float _Matrix[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = _Matrix[i][j];
		}
	}
}

//Initializes the Matrix to a rotation matrix given the Angle and Axis, optionaly, input true to give the angle in radians
FMatrix::FMatrix(eRotationAxis Axis, float Angle, bool IsAngleRadian)
{
	if (!IsAngleRadian) Angle *= 0.174533f;//Converts input from degrees to radians
	this->SetToIdentity();

	switch (Axis)
	{
	case X:
		Matrix[1][1] = cos(Angle);
		Matrix[1][2] = -sin(Angle);
		Matrix[2][1] = sin(Angle);
		Matrix[2][2] = cos(Angle);
		break;

	case Y:
		Matrix[0][0] = cos(Angle);
		Matrix[0][1] = -sin(Angle);
		Matrix[1][0] = sin(Angle);
		Matrix[1][1] = cos(Angle);
		break;

	case Z:
		Matrix[0][0] = cos(Angle);
		Matrix[0][2] = -sin(Angle);
		Matrix[2][0] = sin(Angle);
		Matrix[2][2] = cos(Angle);
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
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = _Matrix.Matrix[i][j];
		}
	}
}

FMatrix FMatrix::operator*(float Multiplier)
{
	FMatrix returnMatrix = FMatrix(Matrix);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			returnMatrix.Matrix[i][j] *= Multiplier;
		}
	}

	return returnMatrix;
}

FMatrix FMatrix::operator*(FMatrix _Matrix)
{
	FMatrix ReturnMatrix = FMatrix(0.0f);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)//The first two for loops will be looping through each element of the Return Matrix
		{
			for (int k = 0; k < 3; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				ReturnMatrix.Matrix[i][j] += Matrix[i][k] * _Matrix.Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	return ReturnMatrix;
}

FMatrix * FMatrix::operator*(FMatrix * _Matrix)
{
	FMatrix * ReturnMatrix = new FMatrix(0.0f);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)//The first two for loops will be looping through each element of the Return Matrix
		{
			for (int k = 0; k < 3; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				ReturnMatrix->Matrix[i][j] += Matrix[i][k] * _Matrix->Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	return ReturnMatrix;
}

FVector3 FMatrix::operator*(FVector3 vector)
{
	FVector3 ReturnVector = FVector3(0);

	ReturnVector.x = Matrix[0][0] * vector.x + Matrix[0][1] * vector.y + Matrix[0][2] * vector.z;
	ReturnVector.y = Matrix[1][0] * vector.x + Matrix[1][1] * vector.y + Matrix[1][2] * vector.z;
	ReturnVector.z = Matrix[2][0] * vector.x + Matrix[2][1] * vector.y + Matrix[2][2] * vector.z;

	return ReturnVector;
}

void FMatrix::operator*=(FMatrix _Matrix)
{
	FMatrix tempMatrix = FMatrix(0.0f);

	for (int i = 0; i < 3; i++)//The first two for loops will be looping through each element of the  tempMatrix
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				tempMatrix.Matrix[i][j] += Matrix[i][k] * _Matrix.Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	SetToArray33(tempMatrix.Matrix);
}

void FMatrix::operator*=(float Multiplier)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] *= Multiplier;
		}
	}
}

///Functions

//Returns the Determinant of this Matrix
float FMatrix::Det()
{
	float Det = 0;

	for (int i = 0; i < 3; i++)
	{
		Det += Matrix[0][i] * (Matrix[1][(i + 1) % 3] * Matrix[2][(i + 2) % 3] - Matrix[2][(i + 1) % 3] * Matrix[1][(i + 2) % 3]);
	}

	return Det;
}

//Returns the iverse matrix to this matrix
FMatrix FMatrix::GetInverse()
{
	if (Det() == 0) return FMatrix();

	FMatrix ReturnMatrix = FMatrix();

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			//Row and collumns are swapt so there is no need to transpose the matrix later
			ReturnMatrix.Matrix[col][row] = ((Matrix[(row + 1) % 3][(col + 1) % 3] * Matrix[(row + 2) % 3][(col + 2) % 3]) - (Matrix[(row + 1) % 3][(col + 2) % 3] * Matrix[(row + 2) % 3][(col + 1) % 3]))/Det();
		}
	}
	return ReturnMatrix;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix::Invert()
{
	SetToArray33(GetInverse().Matrix);
}

//Prints Matrix to console
void FMatrix::PrintMatrix()
{
	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			std::cout << Matrix[row][col] << "\t";
		}
		std::cout << std::endl;
	}
}

//Makes this Matrix an Identity matrix
void FMatrix::SetToIdentity()
{
	Matrix[0][0] = 1.0f;
	Matrix[0][1] = 0.0f;
	Matrix[0][2] = 0.0f;

	Matrix[1][0] = 0.0f;
	Matrix[1][1] = 1.0f;
	Matrix[1][2] = 0.0f;

	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;
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
			Matrix[1][1] = cosf(Angle);
			Matrix[1][2] = 0.0f;
			Matrix[2][1] = 0.0f;
			Matrix[2][2] = cosf(Angle);
			break;

		case Y:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][2] = 0.0f;
			Matrix[2][0] = 0.0f;
			Matrix[2][2] = cosf(Angle);
			break;

		case Z:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][1] = 0.0f;
			Matrix[1][0] = 0.0f;
			Matrix[1][1] = cosf(Angle);
			break;
		}
	}
	else
	{
		if (!IsAngleRadian) Angle *= 0.174533f;//Converts input from degrees to radians

		switch (Axis)
		{
		case X:
			Matrix[1][1] = cosf(Angle);
			Matrix[1][2] = -sinf(Angle);
			Matrix[2][1] = sinf(Angle);
			Matrix[2][2] = cosf(Angle);
			break;

		case Y:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][2] = -sinf(Angle);
			Matrix[2][0] = sinf(Angle);
			Matrix[2][2] = cosf(Angle);
			break;

		case Z:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][1] = -sinf(Angle);
			Matrix[1][0] = sinf(Angle);
			Matrix[1][1] = cosf(Angle);
			break;
		}
	}

}

//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
void FMatrix::SetToScalingMatrix(float ScaleX, float ScaleY, float ScaleZ)
{
	SetToIdentity();

	Matrix[0][0] = ScaleX;
	Matrix[1][1] = ScaleY;
	Matrix[2][2] = ScaleZ;
}

//Swaps Rows and Columns
void FMatrix::Transpose()
{
	FMatrix tempMatrix = FMatrix(Matrix);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = tempMatrix.Matrix[j][i];
		}
	}
}

void FMatrix::SetToArray33(float Array[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = Array[i][j];
		}
	}
}

//Sets all elements of the matrix to a given value
void FMatrix::SetAllElements(float Value)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = Value;
		}
	}
}
