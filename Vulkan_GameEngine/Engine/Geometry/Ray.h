#ifndef RAY_H
#define RAY_H

class FVector3;

class Ray
{
public:
	//Contructors
	Ray();
	Ray(const FVector3& start, const FVector3& direction, float length = 0, bool infinit = true);

	~Ray();

private:
	FVector3* StartPosition;
	FVector3* Direction;
	float Lenght;
	bool Infinit;

public:
	FVector3 GetPosition(float displacement) const;

	FVector3 GetStartPosition() const;
	FVector3 GetDirection() const;

	void SetStartPosition(const FVector3& position);
	void SetDirection(const FVector3& direction);

	inline float GetLenght() const { return Lenght; }
	inline bool IsInfinit() const { return Infinit; }
};

#endif
