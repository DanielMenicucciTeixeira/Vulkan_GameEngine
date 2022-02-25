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
	float Length;
	bool Infinit;

	float intersectDistance;

public:
	FVector3 GetPositionAtLength(float displacement) const;

	FVector3 GetOrigin() const;
	FVector3 GetDirection() const;
	inline float GetLength() const { return Length; }
	inline bool IsInfinit() const { return Infinit; }

	void SetOrigin(const FVector3& position);
	void SetDirection(const FVector3& direction);
	inline void SetLength(float length) { Length = length; }
	inline void SetInfinit(bool infinit) { Infinit = infinit; }

	inline Ray& operator=(const Ray& ray);

	inline float GetIntersectDistance() {
		return intersectDistance;
	}
	void SetIntersectDistance(float dis);
};

#endif
