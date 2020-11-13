#ifndef RAY_H
#define RAY_H

class FVector3;

class Ray
{
public:
	//Contructors
	Ray();
	Ray(FVector3* Start, FVector3* Direction, int Length = 0, bool Infinit = true);

	~Ray();

private:
	FVector3* StartPosition;
	FVector3* Direction;
	float Lenght;
	bool Infinit;

public:
	FVector3 GetPosition(float displacement);

	FVector3* GetStartPosition() { return StartPosition; }
	FVector3* GetDirection() { return Direction; }
	inline float GetLenght() { return Lenght; }
	inline bool IsInfinit() { return Infinit; }
};

#endif
