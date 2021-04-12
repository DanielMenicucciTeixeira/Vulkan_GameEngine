#ifndef PAWN_H
#define PAWN_H
#include "Objects/GameObjects/GameObject.h"

class C_CameraComponent;
class C_MovementComponent;
class C_StaticMeshComponent;
class FVector2;

class GO_Pawn : public O_GameObject
{
public:
	GO_Pawn(L_Level* level = nullptr, std::string name = "");
	virtual ~GO_Pawn();
	C_CameraComponent* Camera;
	C_MovementComponent* Movement;
	C_StaticMeshComponent* Mesh;
	inline void SetSpeed(float speed) { Speed = speed; }

	static void MoveForward(O_Object* self, SDL_Event* event);
	static void MoveBackwards(O_Object* self, SDL_Event* event);
	static void StopMoving(O_Object* self, SDL_Event* event);
	static void TurnCamera(O_Object* self, SDL_Event* event);
	void MoveForward(bool backwards);
	void StopMoving();
	void TurnCamera(FVector2 offset);

protected:
	float Speed = 1.0f;
	float TurnSpeed = 0.005f;
};
#endif

