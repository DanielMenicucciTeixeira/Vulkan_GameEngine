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
	Mesh->SetComponentPosition({ 0.0f, -3.5f, 0.0f });

	Camera = AddComponentOfClass<C_CameraComponent>();
	Camera->SetComponentPosition({ 0.0f, 0.0f, 2.0f });

	Movement = AddComponentOfClass<C_MovementComponent>();

	EventHandler::AddFunctionByInput(this, MoveForward, SDL_KEYDOWN, SDLK_w);
	EventHandler::AddFunctionByInput(this, MoveBackwards, SDL_KEYDOWN, SDLK_s);
	EventHandler::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_w);
	EventHandler::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_s);
	EventHandler::AddFunctionByInput(this, TurnCamera, SDL_MOUSEMOTION, SDLK_UNKNOWN);
}

GO_Pawn::~GO_Pawn()
{
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

void GO_Pawn::TurnCamera(O_Object* self, SDL_Event* event)
{
	IVector2 offset = MouseHandler::GetCursorOffset();
	dynamic_cast<GO_Pawn*>(self)->TurnCamera({(float)offset.X, (float)offset.Y});
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

void GO_Pawn::TurnCamera(FVector2 offset)
{	
	FVector3 rotation = FVector3(offset.Y * TurnSpeed, offset.X * TurnSpeed, 0.0f);
	if (rotation != FVector3())
	{
		while (rotation.Length() > 360.0f) rotation = rotation - (rotation.GetNormal() * 360.0f);
		while (rotation.Length() < -360.0f) rotation = rotation + (rotation.GetNormal() * 360.0f);
		FQuaternion result = FQuaternion(rotation.GetNormal(), rotation.Length() / 2.0 * M_PI, true, true);
		result.Normalize();
		rotation = result.GetEulerAngle();

		if (rotation.X >= 0.78f) rotation.X = 0.78f;
		if (rotation.X <= -0.78f) rotation.X = -0.78f;
		if (rotation.Y >= 0.78f) rotation.Y = 0.78f;
		if (rotation.Y <= -0.78f) rotation.Y = -0.78f;

		result = FQuaternion(rotation, true);

		Camera->SetComponentRotation((result * Camera->GetComponentRotation()).GetNormal());
	}
}
