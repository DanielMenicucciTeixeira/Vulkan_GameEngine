#include "FMatrix4.h"

#include "FVector4.h"
#include <math.h>
#include<iostream>

///Constructors

//Default contructor, initilizes to an identity matrix
FMatrix4::FMatrix4()
{
	Matrix[0][0] = 1.0f;
	Matrix[0][1] = 0.0f;
	Matrix[0][2] = 0.0f;
	Matrix[0][3] = 0.0f;

	Matrix[1][0] = 0.0f;
	Matrix[1][1] = 1.0f;
	Matrix[1][2] = 0.0f;
	Matrix[1][3] = 0.0f;

	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;
	Matrix[2][3] = 0.0f;

	Matrix[3][0] = 0.0f;
	Matrix[3][1] = 0.0f;
	Matrix[3][2] = 0.0f;
	Matrix[3][3] = 1.0f;
}

FMatrix4::FMatrix4(FMatrix _Matrix)
{
	SetToIdentity();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = _Matrix.Matrix[i][j];
		}
	}
}

FMatrix4::FMatrix4(float Value)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] = Value;
		}
	}
}

//Initilizes the Matrix Given 3 FVector3s for rows
FMatrix4::FMatrix4(const FVector4& Row1, const FVector4& Row2, const FVector4& Row3, const FVector4& Row4)
{
	Matrix[0][0] = Row1.X;
	Matrix[0][1] = Row1.Y;
	Matrix[0][2] = Row1.Z;
	Matrix[0][3] = Row1.w;

	Matrix[1][0] = Row2.X;
	Matrix[1][1] = Row2.Y;
	Matrix[1][2] = Row2.Z;
	Matrix[1][3] = Row2.w;

	Matrix[2][0] = Row3.X;
	Matrix[2][1] = Row3.Y;
	Matrix[2][2] = Row3.Z;
	Matrix[2][3] = Row3.w;

	Matrix[3][0] = Row4.X;
	Matrix[3][1] = Row4.Y;
	Matrix[3][2] = Row4.Z;
	Matrix[3][3] = Row4.w;
}

//Initialiazes the Matrix given 3 floats for the diagonal
FMatrix4::FMatrix4(float X, float Y, float Z)
{
	SetToIdentity();

	Matrix[0][0] = X;
	Matrix[1][1] = Y;
	Matrix[2][2] = Z;
}

//Initializes the Matrix given a 3x3 Array
FMatrix4::FMatrix4(float _Matrix[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] = _Matrix[i][j];
		}
	}
}

//Initializes the Matrix to a rotation matrix given the Angle and Axis, optionaly, input true to give the angle in radians
FMatrix4::FMatrix4(eRotationAxis Axis, float Angle, bool IsAngleRadian)
{
	if (!IsAngleRadian) Angle *= 0.174533f;//Converts input from degrees to radians
	SetToIdentity();

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

FMatrix4::~FMatrix4()
{
}

///Operator Overloads

void FMatrix4::operator=(FMatrix4 _Matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] = _Matrix.Matrix[i][j];
		}
	}
}

FMatrix4 FMatrix4::operator*(float Multiplier)
{
	FMatrix4 returnMatrix = FMatrix4(Matrix);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			returnMatrix.Matrix[i][j] *= Multiplier;
		}
	}

	return returnMatrix;
}

FMatrix4 FMatrix4::operator*(FMatrix4 _Matrix)
{
	FMatrix4 ReturnMatrix = FMatrix4(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)//The first two for loops will be looping through each element of the Return Matrix
		{
			for (int k = 0; k < 4; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				ReturnMatrix.Matrix[i][j] += Matrix[i][k] * _Matrix.Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	return ReturnMatrix;
}

FMatrix4* FMatrix4::operator*(FMatrix4* _Matrix)
{
	FMatrix4* ReturnMatrix4 = new FMatrix4(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)//The first two for loops will be looping through each element of the Return Matrix
		{
			for (int k = 0; k < 4; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				ReturnMatrix4->Matrix[i][j] += Matrix[i][k] * _Matrix->Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	return ReturnMatrix4;
}

FVector3 FMatrix4::operator*(FVector3 vector)
{
	FVector3 ReturnVector = FVector3(0);

	ReturnVector.X = Matrix[0][0] * vector.X + Matrix[0][1] * vector.Y + Matrix[0][2] * vector.Z;
	ReturnVector.Y = Matrix[1][0] * vector.X + Matrix[1][1] * vector.Y + Matrix[1][2] * vector.Z;
	ReturnVector.Z = Matrix[2][0] * vector.X + Matrix[2][1] * vector.Y + Matrix[2][2] * vector.Z;

	return ReturnVector;
}

FVector4* FMatrix4::operator*(FVector4* vector)
{
	FVector4* ReturnVector = new FVector4(0);

	ReturnVector->X = Matrix[0][0] * vector->X + Matrix[0][1] * vector->Y + Matrix[0][2] * vector->Z + Matrix[0][3] * vector->w;
	ReturnVector->Y = Matrix[1][0] * vector->X + Matrix[1][1] * vector->Y + Matrix[1][2] * vector->Z + Matrix[1][3] * vector->w;
	ReturnVector->Z = Matrix[2][0] * vector->X + Matrix[2][1] * vector->Y + Matrix[2][2] * vector->Z + Matrix[2][3] * vector->w;
	ReturnVector->w = Matrix[3][0] * vector->X + Matrix[3][1] * vector->Y + Matrix[3][2] * vector->Z + Matrix[3][3] * vector->w;

	return ReturnVector;
}

void FMatrix4::operator*=(FMatrix4 _Matrix)
{
	FMatrix4 tempMatrix = FMatrix4(0.0f);

	for (int i = 0; i < 4; i++)//The first two for loops will be looping through each element of the  tempMatrix
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)//Loops through each element of current row for this matrix and of current coloum for the multipling matrix
			{
				tempMatrix.Matrix[i][j] += Matrix[i][k] * _Matrix.Matrix[k][j];//Multiplies the rows from this Matrix by the coloumns of the multipling matrix
			}
		}
	}

	SetToArray44(tempMatrix.Matrix);
}

void FMatrix4::operator*=(float Multiplier)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] *= Multiplier;
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
		Det += Matrix[0][i] * (Matrix[1][(i + 1) % 3] * Matrix[2][(i + 2) % 3] - Matrix[2][(i + 1) % 3] * Matrix[1][(i + 2) % 3]);
	}

	return Det;
}

//Returns the iverse matrix to this matrix
FMatrix4 FMatrix4::GetInverse()
{
	if (Det() == 0) return FMatrix4();

	FMatrix4 ReturnMatrix = FMatrix4();

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			//Row and collumns are swapt so there is no need to transpose the matrix later
			ReturnMatrix.Matrix[col][row] = ((Matrix[(row + 1) % 3][(col + 1) % 3] * Matrix[(row + 2) % 3][(col + 2) % 3]) - (Matrix[(row + 1) % 3][(col + 2) % 3] * Matrix[(row + 2) % 3][(col + 1) % 3])) / Det();
		}
	}
	return ReturnMatrix;
}

//Transform the Matrix into its Inverse Matrix
void FMatrix4::Invert()
{
	SetToArray44(GetInverse().Matrix);
}

//Prints Matrix to console
void FMatrix4::PrintMatrix()
{
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			std::cout << Matrix[row][col] << "\t";
		}
		std::cout << std::endl;
	}
}

//Makes this Matrix an Identity matrix
void FMatrix4::SetToIdentity()
{
	Matrix[0][0] = 1.0f;
	Matrix[0][1] = 0.0f;
	Matrix[0][2] = 0.0f;
	Matrix[0][3] = 0.0f;

	Matrix[1][0] = 0.0f;
	Matrix[1][1] = 1.0f;
	Matrix[1][2] = 0.0f;
	Matrix[1][3] = 0.0f;

	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;
	Matrix[2][3] = 0.0f;

	Matrix[3][0] = 0.0f;
	Matrix[3][1] = 0.0f;
	Matrix[3][2] = 0.0f;
	Matrix[3][3] = 1.0f;
}

//Makes this Matrix a Rotation Matrix on Z axis with given degrees
void FMatrix4::SetToRotationMatrix(eRotationAxis Axis, float Angle, bool IsAngleRadian)
{
	this->SetToIdentity();

	if (!IsAngleRadian && Angle == 90)//Corrects a calculation mistake that does not see sin(90) as 0 due to conversion loss, but rather a extremely small number.
	{
		Angle *= 0.174533f;//Converts input from degrees to radians

		switch (Axis)
		{
		case X:
			Matrix[1][1] = cosf(Angle);
			Matrix[1][2] = 0;
			Matrix[2][1] = 0;
			Matrix[2][2] = cosf(Angle);
			break;

		case Y:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][2] = 0;
			Matrix[2][0] = 0;
			Matrix[2][2] = cosf(Angle);
			break;

		case Z:
			Matrix[0][0] = cosf(Angle);
			Matrix[0][1] = 0;
			Matrix[1][0] = 0;
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
void FMatrix4::SetToScalingMatrix(float ScaleX, float ScaleY, float ScaleZ)
{
	SetToIdentity();

	Matrix[0][0] = ScaleX;
	Matrix[1][1] = ScaleY;
	Matrix[2][2] = ScaleZ;
}

void FMatrix4::SetToTranslationMatrix(float X, float Y, float Z)
{
	SetToIdentity();

	Matrix[0][3] = X;
	Matrix[1][3] = Y;
	Matrix[2][3] = Z;
}

//Swaps Rows and Columns
void FMatrix4::Transpose()
{
	FMatrix4 tempMatrix = FMatrix4(Matrix);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Matrix[i][j] = tempMatrix.Matrix[j][i];
		}
	}
}

void FMatrix4::SetToArray44(float Array[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] = Array[i][j];
		}
	}
}

//Sets all elements of the matrix to a given value
void FMatrix4::SetAllElements(float Value)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Matrix[i][j] = Value;
		}
	}
}