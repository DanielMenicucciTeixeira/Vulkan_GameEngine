#ifndef PAWN_H
#define PAWN_H
#include "Objects/GameObjects/GameObject.h"
#include "Math/FVector3.h"

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

	void Update(float deltaTime) override;

	static void MoveForward(O_Object* self, SDL_Event* event);
	static void MoveBackwards(O_Object* self, SDL_Event* event);
	static void StopMoving(O_Object* self, SDL_Event* event);
	static void TurnRight(O_Object* self, SDL_Event* event);
	static void TurnLeft(O_Object* self, SDL_Event* event);
	static void StopTurning(O_Object* self, SDL_Event* event);
	static void TurnCamera(O_Object* self, SDL_Event* event);
	static void ZoomCamera(O_Object* self, SDL_Event* event);
	static void PrintFrustum(O_Object* self, SDL_Event* event);
	static void Grab(O_Object* self, SDL_Event* event);

	void MoveForward(bool backwards);
	void StopMoving();
	void Turn(bool left);
	void StopTurning();
	void TurnCamera(FVector2 offset);
	void ZoomCamera(float zoom);
	void Grab();

protected:
	float Speed = 10.0f;
	float CameraTurnSpeed = 0.005f;
	float CameraZoomSpeed = 1.0f;
	FVector3 CameraRotation;
};
#endif

