#ifndef FTransform_
#define FTransform_

class FVector3;
class FQuaternion;
class FMatrix4;

struct FTransform
{
	FTransform();
	FTransform(const FTransform& transform);
	FTransform(const FVector3& position, const FQuaternion& rotation, const FVector3& scale);

	~FTransform();

	FVector3* Position;
	FVector3*  Scale;
	FQuaternion* Rotation;

	FMatrix4 GetModelMatrix() const;
	void operator= (const FTransform& transform);
	inline void operator= (const FTransform*& transform) { *this = *transform; }
	bool operator== (const FTransform& transform) const;
	inline bool operator== (const FTransform*& transform) const { return *this == *transform; }
};
#endif