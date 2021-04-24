#include "Pawn.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Math/IVector2.h"
#include "Math/FVector2.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "MouseHandler.h"
#include "Event/EventHandler.h"
#include <SDL.h>

GO_Pawn::GO_Pawn(L_Level* level, std::string name) : O_GameObject(level, name)
{

	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Box001");
	Mesh->SetMaterialName("M_diceTexture");
	Mesh->SetComponentScale({ 0.3f, 0.3f, 0.3f });
	Mesh->SetComponentPosition({ 0.0f, 0.0, 0.0 });
	//Mesh->SetComponentRotation(FQuaternion({ 0.0f, 0.0f, 1.0f }, 90.0f));

	Camera = AddComponentOfClass<C_CameraComponent>();
	Camera->SetComponentPosition({ 0.0f, 0.0f, 5.0f });

	Movement = AddComponentOfClass<C_MovementComponent>();

	EventHandler::AddFunctionByInput(this, MoveForward, SDL_KEYDOWN, SDLK_w);
	EventHandler::AddFunctionByInput(this, MoveBackwards, SDL_KEYDOWN, SDLK_s);
	EventHandler::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_w);
	EventHandler::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_s);

	EventHandler::AddFunctionByInput(this, TurnRight, SDL_KEYDOWN, SDLK_d);
	EventHandler::AddFunctionByInput(this, TurnLeft, SDL_KEYDOWN, SDLK_a);
	EventHandler::AddFunctionByInput(this, StopTurning, SDL_KEYUP, SDLK_d);
	EventHandler::AddFunctionByInput(this, StopTurning, SDL_KEYUP, SDLK_a);

	EventHandler::AddFunctionByInput(this, TurnCamera, SDL_MOUSEMOTION, SDLK_UNKNOWN);
}

GO_Pawn::~GO_Pawn()
{
}

void GO_Pawn::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
	Camera->GetComponentPosition().Print();
	Camera->GetComponentAbsolutePosition().Print();
}

void GO_Pawn::MoveForward(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->MoveForward(false);
}

void GO_Pawn::MoveBackwards(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->MoveForward(true);
}

void GO_Pawn::StopMoving(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->StopMoving();
}

void GO_Pawn::TurnRight(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->Turn(false);
}

void GO_Pawn::TurnLeft(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->Turn(true);
}

void GO_Pawn::StopTurning(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->StopTurning();
}

void GO_Pawn::TurnCamera(O_Object* self, SDL_Event* event)
{
	IVector2 offset = MouseHandler::GetCursorOffset();
	dynamic_cast<GO_Pawn*>(self)->TurnCamera({(float)offset.X, (float)offset.Y});
}

void GO_Pawn::ZoomCamera(O_Object* self, SDL_Event* event)
{
}

void GO_Pawn::MoveForward(bool backwards)
{
	if (backwards) Movement->SetVelocity(GetRotation().GetForwardVector() * -Speed);
	else  Movement->SetVelocity(GetRotation().GetForwardVector() * Speed);
}

void GO_Pawn::StopMoving()
{
	Movement->SetVelocity(FVector3(0.0f));
}

void GO_Pawn::Turn(bool left)
{
	if (left) Movement->SetAngularVelocity(FVector3(0.0f, 1.0f, 0.0f));
	else  Movement->SetAngularVelocity(FVector3(0.0f, -1.0f, 0.0f));
}

void GO_Pawn::StopTurning()
{
	Movement->SetAngularVelocity(FVector3(0.0f, 0.0f, 0.0f));
}

void GO_Pawn::TurnCamera(FVector2 offset)
{	
	FVector3 rotation = FVector3(offset.Y * -CameraTurnSpeed, offset.X * -CameraTurnSpeed, 0.0f);
	if (rotation != FVector3())
	{
		while (rotation.X > 360.0f) rotation.X = rotation.X - 360.0f;
		while (rotation.X < -360.0f) rotation.X = rotation.X + 360.0f;
		while (rotation.Y > 360.0f) rotation.Y = rotation.Y - 360.0f;
		while (rotation.Y < -360.0f) rotation.Y = rotation.Y + 360.0f;

		CameraRotation = CameraRotation + rotation;

		if (rotation.X >= 60.0f) rotation.X = 60.0f;
		if (rotation.X <= -60.0f) rotation.X = -60.0f;
		if (rotation.Y >= 60.0f) rotation.Y = 60.0f;
		if (rotation.Y <= -60.0f) rotation.Y = -60.0f;

		//FQuaternion result = Camera->GetComponentRotation() * FQuaternion(rotation.GetNormal(), rotation.Length() / 2.0 * M_PI, true, true);
		/*result.Normalize();
		rotation = result.GetEulerAngle();

		if (rotation.X >= 90.0f) rotation.X = 90.0f;
		if (rotation.X <= -90.0f) rotation.X = -90.0f;
		if (rotation.Y >= 90.0f) rotation.Y = 90.0f;
		if (rotation.Y <= -90.0f) rotation.Y = -90.0f;

		result = FQuaternion(rotation, true);*/
		FQuaternion result = FQuaternion(CameraRotation, true);
		Camera->SetComponentRotation(result.GetNormal());
	}
}

void GO_Pawn::ZoomCamera(float zoom)
{

}
