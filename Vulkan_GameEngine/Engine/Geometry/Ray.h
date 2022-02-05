#ifndef RAY_H
#define RAY_H

class FVector3;
class C_BoundingBox;

class Ray
{
public:
	//Contructors
	Ray();
	Ray(const FVector3 start, const FVector3 direction, float length = 0, bool infinit = true);

	~Ray();

private:
	FVector3* Origin;
	FVector3* Direction;
	float Lenght;
	bool Infinit;

public:
	FVector3 GetPositionAtLenght(float displacement) const;

	FVector3 GetOrigin() const;
	FVector3 GetDirection() const;
	inline float GetLenght() const { return Lenght; }
	inline bool IsInfinit() const { return Infinit; }

	void SetOrigin(const FVector3& position);
	void SetDirection(const FVector3& direction);
	inline void SetLenght(float lenght) { Lenght = lenght; }
	inline void SetInfinit(bool infinit) { Infinit = infinit; }

	inline Ray& operator=(const Ray& ray);

	bool IsColliding(C_BoundingBox* box);
};

#endif
