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
#include "Geometry/Ray.h"
#include "Objects/Components/Colliders/CollisionComponent.h"
#include "Math/IVector2.h"

#include <iostream>
#include <SDL.h>

GO_Pawn::GO_Pawn(L_Level* level, std::string name) : O_GameObject(level, name)
{

	/*Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Box001");
	Mesh->SetMaterialName("M_diceTexture");
	Mesh->SetComponentScale({ 0.3f, 0.3f, 0.3f });
	Mesh->SetComponentPosition({ 0.0f, 0.0, 0.0 });*/

	Camera = AddComponentOfClass<C_CameraComponent>();
	Camera->SetComponentPosition({ 0.0f, 0.0f, 5.0f });

	Movement = AddComponentOfClass<C_MovementComponent>();

	EventListener::AddFunctionByInput(this, MoveForward, SDL_KEYDOWN, SDLK_w);
	EventListener::AddFunctionByInput(this, MoveBackwards, SDL_KEYDOWN, SDLK_s);
	EventListener::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_w);
	EventListener::AddFunctionByInput(this, StopMoving, SDL_KEYUP, SDLK_s);

	EventListener::AddFunctionByInput(this, TurnRight, SDL_KEYDOWN, SDLK_d);
	EventListener::AddFunctionByInput(this, TurnLeft, SDL_KEYDOWN, SDLK_a);
	EventListener::AddFunctionByInput(this, StopTurning, SDL_KEYUP, SDLK_d);
	EventListener::AddFunctionByInput(this, StopTurning, SDL_KEYUP, SDLK_a);

	//EventListener::AddFunctionByInput(this, TurnCamera, SDL_MOUSEMOTION, SDLK_UNKNOWN);
	EventListener::AddFunctionByInput(this, Grab, SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT);
	EventListener::AddFunctionByInput(this, PrintFrustum, SDL_KEYUP, SDLK_c);
}

GO_Pawn::~GO_Pawn()
{
}

void GO_Pawn::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
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

void GO_Pawn::PrintFrustum(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->Camera->PrintFrustum();
}

void GO_Pawn::Grab(O_Object* self, SDL_Event* event)
{
	dynamic_cast<GO_Pawn*>(self)->Grab();
}

void GO_Pawn::MoveForward(bool backwards)
{
	if (backwards) {
		//Movement->SetVelocity(GetRotation().GetForwardVector() * -Speed);
			//TODO: Velocity function change
	}
	else {
		//Movement->SetVelocity(GetRotation().GetForwardVector() * Speed);
			//TODO: Velocity function change
	}
}

void GO_Pawn::StopMoving()
{
	//Movement->SetVelocity(FVector3(0.0f));
		//TODO: Velocity function change
}

void GO_Pawn::Turn(bool left)
{
	if (left) { //Movement->SetAngularVelocity(FVector3(0.0f, 1.0f, 0.0f));
			//TODO: Velocity function change
	}
	else {//Movement->SetAngularVelocity(FVector3(0.0f, -1.0f, 0.0f));
			//TODO: Velocity function change

	}
}

void GO_Pawn::StopTurning()
{
	//Movement->SetAngularVelocity(FVector3(0.0f, 0.0f, 0.0f));
		//TODO: Velocity function change
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

void GO_Pawn::Grab()
{
	S_CollisionData data;
	Ray ray = MouseHandler::MousePositionToWorldRay();
	if (C_CollisionComponent::RayCastSingleTarget(ray, data))
	{
		std::cout << data.OtherCollisonComponent->GetOwner()->GetName() << " was hit!" << std::endl;
	}
	else std::cout << "No hit!" << std::endl;
}
