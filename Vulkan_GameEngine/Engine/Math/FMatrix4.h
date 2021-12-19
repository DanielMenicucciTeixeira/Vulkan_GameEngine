#ifndef FMATRIX4_H
#define FMATRIX4_H

class FVector3; 
class FQuaternion;
class FMatrix;
#include "FVector4.h"

enum eRotationAxis;

class FMatrix4
{
public:
	//the Matrix itself
	FVector4 Colum[4];

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
	FMatrix4
		(
			float a0, float a1, float a2, float a3,
			float b0, float b1, float b2, float b3,
			float c0, float c1, float c2, float c3,
			float d0, float d1, float d2, float d3
		);

	//Initializes the Matrix to a rotation matrix given the angle and axis, optionaly, input true to give the angle in radians
	FMatrix4(eRotationAxis axis, float angle, bool isAngleRadian = false);

	///Destructors

	~FMatrix4();

	///Operator Overloads

	void operator= (FMatrix4 matrix);

	void operator= (FMatrix4* matrix);

	FMatrix4 operator* (float multiplier) const;

	FMatrix4 operator* (FMatrix4* matrix) const;

	FMatrix4 operator* (FMatrix4 matrix) const;

	FVector4 operator*(FVector4* vector) const;

	FVector4 operator*(FVector4 vector) const;

	FVector3 operator*(FVector3 vector) const;

	void operator*= (FMatrix4* matrix);

	void operator*= (FMatrix4 matrix);

	void operator*= (float multiplier);

	inline const FVector4 operator [] (int index) const { return *(Colum + index); }
	inline FVector4& operator [] (int index) { return *(Colum + index); }

	/// These allow me convert from type Matrix to const float * without issues
	inline operator float* () { return static_cast<float*>(&Colum[0][0]); }
	inline operator const float* () const { return static_cast<const float*>(Colum[0]); }

	//inline operator FVector4* () { return static_cast<FVector4*>(&Colum[0]); }
	//inline operator const FVector4* () const { return static_cast<const FVector4*>(&Colum[0]); }

	///Functions

	//Makes this Matrix an Identity matrix
	void SetToIdentity();

	void SetRow(const FVector4& vector, int row);

	//Makes this Matrix a Rotation Matrix on given axis with given degrees optionaly, input true to give the angle in radians
	static FMatrix4 GetRotationMatrix(float angle, float x, float y, float z, bool isAngleRadian = false);

	static FMatrix4 GetRotationMatrix(const FQuaternion& rotation);
	inline static FMatrix4 GetRotationMatrix(const FQuaternion* rotation) { return GetRotationMatrix(*rotation); }

	void SetToRotationMatrix(float angle, float x, float y, float z, bool isAngleRadian = false);

	void SetToLookAtMatrix(const FVector3& eye, const FVector3& target, const FVector3& up);

	static FMatrix4 GetLookAtMatrix(const FVector3& eye, const FVector3& target, const FVector3& up);

	void SetToPerspectiveMatrix(float fieldOfView, float aspect, float zNear, float zFar, bool isRadian = false);

	static FMatrix4 GetPerspectiveMatrix(float fieldOfView, float aspect, float zNear, float zFar, bool isRadian = false);

	//Makes this Matrix a Scaling Matrix with X, Y and Z respectively on the diagonal
	void SetToScalingMatrix(float scaleX, float scaleY, float scaleZ);

	static FMatrix4 GetScalingMatrix(const FVector3& scale);
	inline static FMatrix4 GetScalingMatrix(const FVector3*& scale) { return GetScalingMatrix(*scale); }

	//Makes this Matrix a Translation Matrix
	void SetToTranslationMatrix(float X, float Y, float Z);

	static FMatrix4 GetTranslationMatrix(const FVector3& position);
	inline static FMatrix4 GetTranslationMatrix(const FVector3*& position) { return GetTranslationMatrix(*position); }

	static FMatrix4 GetViewMatrix(const FQuaternion& rotation, const FVector3& position);

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

	void Print();
};
#endif

