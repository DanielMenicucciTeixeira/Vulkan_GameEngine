#ifndef FMATRIX3_
#define FMATRIX3_

class FVector3;

enum eRotationAxis { X, Y, Z };

class FMatrix
{


public:

	//the Matrix itself
	float Matrix[3][3];

	///Constructors

	//Default contructor, initilizes to an identity matrix
	FMatrix();

	//Constructor that initilizes all elements of the matrix to a given float
	FMatrix(float Value);

	//Initializes the Matrix Given 3 FVector3s for rows
	FMatrix(FVector3 Row1, FVector3 Row2, FVector3 Row3);

	//Initialiazes the Matrix given 3 floats for the diagonal
	FMatrix(float X, float Y, float Z);

	//Initializes the Matrix given a 3x3 Array
	FMatrix(float _Matrix[3][3]);

	//Initializes the Matrix to a rotation matrix given the Angle and Axis, optionaly, input true to give the angle in radians
	FMatrix(eRotationAxis Axis, float Angle, bool IsAngleRadian = false);

	///Destructors

	~FMatrix();

	///Operator Overloads

	void operator= (FMatrix _Matrix);

	FMatrix operator* (float Multiplier);

	FMatrix operator* (FMatrix _Matrix);

	FMatrix* operator* (FMatrix * _Matrix);

	FVector3 operator*(FVector3 _Vecator);

	void operator*= (FMatrix _Matrix);

	void operator*= (float Multiplier);

	///Functions

	//Makes this Matrix an Identity matrix
	void SetToIdentity();

	//Makes this Matrix a Rotation Matrix on given axis with given degrees optionaly, input true to give the angle in radians
	void SetToRotationMatrix(eRotationAxis Asix, float Angle, bool IsAngleRadian = false);

	//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
	void SetToScalingMatrix(float ScaleX, float ScaleY, float ScaleZ);

	//Swaps Rows and Columns
	void Transpose();

	//Sets the matrix to be equal another matrix given in the for of a 3x3 array.
	void SetToArray33(float Array[3][3]);

	//Sets all elements of the matrix to a given value
	void SetAllElements(float Value);

	//Returns the Determinant of the Matrix
	float Det();

	//Returns the Iverse of the Matrix
	FMatrix GetInverse();

	//Inverts the Matrix
	void Invert();

	//Prints the matrix to console
	void PrintMatrix();

};
#endif