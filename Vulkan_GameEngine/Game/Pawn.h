#ifndef PAWN_H
#define PAWN_H
#include "../Engine/Objects/GameObjects/GameObject.h"
#include "../Engine/Math/FVector3.h"

class C_CameraComponent;
class C_PhysicsComponent;
class C_StaticMeshComponent;
class FVector2;

class GO_Pawn : public O_GameObject
{
public:
	GO_Pawn(std::string name = "");
	virtual ~GO_Pawn();
	C_CameraComponent* Camera;
	C_PhysicsComponent* Movement;
	C_StaticMeshComponent* Mesh;
	inline void SetSpeed(float speed) { Speed = speed; }

	void Update(float deltaTime) override;

	static void MoveForward(void* self, SDL_Event* event);
	static void MoveBackwards(void* self, SDL_Event* event);
	static void StopMoving(void* self, SDL_Event* event);
	static void TurnRight(void* self, SDL_Event* event);
	static void TurnLeft(void* self, SDL_Event* event);
	static void StopTurning(void* self, SDL_Event* event);
	static void TurnCamera(void* self, SDL_Event* event);
	static void ZoomCamera(void* self, SDL_Event* event);
	static void PrintFrustum(void* self, SDL_Event* event);
	static void Grab(void* self, SDL_Event* event);

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

