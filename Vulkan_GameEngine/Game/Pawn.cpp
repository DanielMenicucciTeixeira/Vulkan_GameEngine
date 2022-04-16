#include "Pawn.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Math/IVector2.h"
#include "Math/FVector2.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Event/MouseEventHandler.h"
#include "Event/EventListener.h"
#include "Geometry/Ray.h"
#include "CollisionHandler.h"
#include "Math/IVector2.h"

#include "Objects/Components/Colliders/BoundingBox.h"

#include <iostream>
#include <SDL.h>

GO_Pawn::GO_Pawn(std::string name) : O_GameObject(name)
{
	Movement = AddComponentOfClass<C_PhysicsComponent>();
	Movement->SetApplyGravity(true);
	Movement->SetRubberness(0.0f);
	//Movement->SetVelocity(FVector3(0.0f, -9.8f, 0.0f));


	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Box001");
	Mesh->SetMaterialName("M_diceTexture");
	Mesh->SetComponentScale( FVector3(0.25f) );
	Mesh->SetComponentPosition({ 0.0f, 0.0, 0.0 });

	//SetPosition(FVector3(0.0f));

	C_BoundingBox* spherePtr = AddComponentOfClass<C_BoundingBox>();
	spherePtr->SetCollisionType(ECollisionType::COLLISION);
	spherePtr->SetComponentScale(FVector3(0.25f));


	Camera = AddComponentOfClass<C_CameraComponent>();
	Camera->SetComponentPosition({ 0.0f, 1.0f, 8.0f });



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
	//dynamic_cast<GO_Pawn*>(self)->GetComponentOfClass<C_PhysicsComponent>()->SetApplyGravity(false);
	//dynamic_cast<GO_Pawn*>(self)->GetComponentOfClass<C_PhysicsComponent>()->Translate(FVector3(0.0f, 0.5f, 0.0f));
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
	IVector2 offset = MouseEventHandler::GetCursorOffset();
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
		Movement->AddVelocity(GetRotation().GetForwardVector() * -Speed);
	}
	else {
		Movement->AddVelocity(GetRotation().GetForwardVector() * Speed);
	}
}

void GO_Pawn::StopMoving()
{
	//Movement->AddVelocity(FVector3(0.0f));
}

void GO_Pawn::Turn(bool left)
{
	if (left) { 
		Movement->SetAngularVelocity(FVector3(0.0f, 10.0f, 0.0f));
	}
	else {
		Movement->SetAngularVelocity(FVector3(0.0f, -10.0f, 0.0f));
	}
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

void GO_Pawn::Grab()
{

	//TODO: aim ray right in front of object.
	
	Ray ray = MouseEventHandler::MousePositionToWorldRay();

	FVector3 scale = GetScale() / 2;
	FVector3 fwd = GetRotation().GetForwardVector();

	FVector3 rot = FVector3(scale.X * fwd.X, scale.Y * fwd.Y, scale.Z * fwd.Z);

	FVector3 centre = GetPosition() + (scale * 2);

	ray.SetOrigin(centre + rot);
	ray.SetDirection(GetRotation().GetForwardVector());
	ray.obj = this;

	auto data = CollisionHandler::GetInstance()->GetCollisionSingleRay(ray);
	
		if (data.OtherCollisonComponent != nullptr)
		{
			std::cout << data.OtherCollisonComponent->GetOwner()->GetName() << " was hit!" << std::endl;

			data.OtherCollisonComponent->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->AddVelocity(GetRotation().GetForwardVector() * 100.0f);
		}
		else std::cout << "No hit!" << std::endl;
	
}
