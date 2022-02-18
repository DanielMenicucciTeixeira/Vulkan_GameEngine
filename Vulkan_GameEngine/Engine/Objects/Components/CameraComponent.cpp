#include "CameraComponent.h"
#include "CoreEngine.h"
#include "Math/FVector2.h"
#include "Renderers/RenderObject.h"
#include "Core/Window.h"
#include "Math/FQuaternion.h"
#include "Colliders/BoundingBox.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"
#include <SDL.h>

void C_CameraComponent::CalculateFrustum()
{
	FMatrix4 world = UCO->Projection * UCO->View;

	// Left clipping plane
	Frustum[0].X = world[3][0] + world[0][0];
	Frustum[0].Y = world[3][1] + world[0][1];
	Frustum[0].Z = world[3][2] + world[0][2];
	Frustum[0].W = world[3][3] + world[0][3];
	Frustum[0].NormalizePlane();
	// Right clipping plane
	Frustum[1].X = (world[3][0] - world[0][0]);
	Frustum[1].Y = (world[3][1] - world[0][1]);
	Frustum[1].Z = (world[3][2] - world[0][2]);
	Frustum[1].W = world[3][3] - world[0][3];
	Frustum[1].NormalizePlane();
	// Top clipping plane
	Frustum[2].X = (world[3][0] - world[1][0]);
	Frustum[2].Y = (world[3][1] - world[1][1]);
	Frustum[2].Z = (world[3][2] - world[1][2]);
	Frustum[2].W = world[3][3] - world[1][3];
	Frustum[2].NormalizePlane();
	// Bottom clipping plane
	Frustum[3].X = world[3][0] + world[1][0];
	Frustum[3].Y = world[3][1] + world[1][1];
	Frustum[3].Z = world[3][2] + world[1][2];
	Frustum[3].W = world[3][3] + world[1][3];
	Frustum[3].NormalizePlane();
	// Near clipping plane
	Frustum[4].X = world[3][0] + world[2][0];
	Frustum[4].Y = world[3][1] + world[2][1];
	Frustum[4].Z = world[3][2] + world[2][2];
	Frustum[4].W = world[3][3] + world[2][3];
	Frustum[4].NormalizePlane();
	// Far clipping plane
	Frustum[5].X = -(world[3][0] - world[2][0]);
	Frustum[5].Y = -(world[3][1] - world[2][1]);
	Frustum[5].Z = -(world[3][2] - world[2][2]);
	Frustum[5].W = (world[3][3] - world[2][3]);
	Frustum[5].NormalizePlane();
}

void C_CameraComponent::Update(const float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	UCO->View.SetToLookAtMatrix(GetComponentAbsolutePosition(), GetComponentAbsolutePosition() + GetComponentAbsoluteRotation().GetForwardVector(), GetComponentAbsoluteRotation().GetUpVector());
	//UCO->View = FMatrix4::GetViewMatrix(GetComponentAbsoluteRotation(), GetComponentAbsolutePosition());
	CalculateFrustum();
}

void C_CameraComponent::Start()
{
	C_TransformComponent::Start();
	UCO->View.SetToLookAtMatrix(GetComponentAbsolutePosition(), GetComponentAbsolutePosition() + GetComponentAbsoluteRotation().GetForwardVector(), GetComponentAbsoluteRotation().GetUpVector());
	//UCO->View = FMatrix4::GetViewMatrix(GetComponentAbsoluteRotation(), GetComponentAbsolutePosition());
	
	FVector2 size = CoreEngine::GetInstance()->GetWindowSize();

	UCO->Projection.SetToPerspectiveMatrix(FildOfView.Angle, size.Y/ size.X, FildOfView.NearPlane, FildOfView.FarPlane);
	CalculateFrustum();
}

void C_CameraComponent::UpdateProjection()
{
	int w, h;
	SDL_GetWindowSize(CoreEngine::GetInstance()->GetWindowSDL(), &w, &h);
	UCO->Projection.SetToPerspectiveMatrix(FildOfView.Angle, float(w) / float(h), FildOfView.NearPlane, FildOfView.FarPlane);
}

C_CameraComponent::C_CameraComponent(O_GameObject* owner) : C_TransformComponent(owner), UCO(new UniformCameraObject)
{
	
}

C_CameraComponent::~C_CameraComponent()
{
	if (UCO) delete(UCO);
}

FMatrix4 C_CameraComponent::GetViewMatrix()
{
	return UCO->View;
}

FMatrix4 C_CameraComponent::GetProjectionMatrix()
{
	return UCO->Projection;
}

bool C_CameraComponent::FrustumCheck(C_BoundingBox* meshBox)
{
	bool wrongSide, rightSide;
	int i, j, k;
	FVector3 point;
	bool getOut;

	for (int p = 0; p < 6; p++) //for each plane
	{
		wrongSide = false; rightSide = false;
		getOut = false;

		for (i = 0; i < 2 && !getOut; i++)
		{
			if (i == 0) point.X = meshBox->GetMin().X;
			else point.X = meshBox->GetMax().X;

			for (j = 0; j < 2 && !getOut; j++)
			{
				if (j == 0) point.Y = meshBox->GetMin().Y;
				else point.Y = meshBox->GetMax().Y;

				for (k = 0; k < 2 && !getOut; k++)
				{
					if (k == 0) point.Z = meshBox->GetMin().Z;
					else point.Z = meshBox->GetMax().Z;

					// is the corner outside or inside
					if (Frustum[p] * FVector4(point.X, point.Y, point.Z, 1) < 0) wrongSide = true;
					else rightSide = true;

					//get out of the cycle as soon as a box has corners both inside and out of the plane
					getOut = (rightSide == true && wrongSide == true);
				}
			}
		}

		if (!rightSide) return (false);//if all corners are out
	}
	return(true);
}

void C_CameraComponent::PrintFrustum()
{
	for (const auto& plane : Frustum) plane.Print();
	printf("\n");
}
