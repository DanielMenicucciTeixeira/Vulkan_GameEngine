#ifndef FTransform_
#define FTransform_

class FVector3;
class FQuaternion;
class FMatrix4;

struct FTransform
{
public:
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
	FTransform operator+ (const FTransform& transform) const;
	inline FTransform operator+ (const FTransform* transform) const { return *this + *transform; }

	void SetPosition(const FVector3& position);
	void SetRotation(const FQuaternion& rotation);
	void SetScale(const FVector3& scale);
};
#endif