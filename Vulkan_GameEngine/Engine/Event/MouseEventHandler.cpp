#include "MouseEventHandler.h"
#include "Engine/Core/Game.h"
#include "Engine/Core/CoreEngine.h"
#include "Engine/Math/IVector2.h"
#include "Engine/Math/FVector2.h"
#include "Engine/Math/FMatrix4.h"
#include "Engine/Math/FVector3.h"
#include "Engine/Geometry/Ray.h"
#include "Engine/Renderers/UniformBufferObject.h"
#include "Engine/Core/LevelGraph.h"
#include "Engine/Objects/Components/CameraComponent.h"

#include <SDL.h>
#include "MouseEventHandler.h"

CoreEngine* MouseEventHandler::Engine;
IVector2 MouseEventHandler::CursorPosition, MouseEventHandler::PreviousCursorPosition;

MouseEventHandler::~MouseEventHandler()
{
}

void MouseEventHandler::SetEngineReference(CoreEngine* engine)
{
	Engine = engine;
}

void MouseEventHandler::Initialize()
{
	//TODO: Fix mouse get here
	//SDL_Window* window = SDLManager::GetInstance()->GetSDLWindowByName();
	FVector2 size = Engine->GetWindowSize();

	//TODO: fix window
	//SDL_WarpMouseInWindow(window, size.x * 0.5f, size.y * 0.5f);
	int x, y;
	SDL_GetMouseState(&x, &y);
	y = size.Y - y;
	PreviousCursorPosition.X = CursorPosition.X = x;
	PreviousCursorPosition.Y = CursorPosition.Y = y;
}

const IVector2& MouseEventHandler::GetPreviousCursorPosition()
{
	return PreviousCursorPosition;
}

const IVector2& MouseEventHandler::GetCursorPosition()
{
	return CursorPosition;
}

IVector2 MouseEventHandler::GetCursorOffset()
{
	//Y Axis is inverted because SDL treats screen space with Up = 0 Low = 1;
	return IVector2(CursorPosition.X - PreviousCursorPosition.X, PreviousCursorPosition.Y - CursorPosition.Y);
}

Ray MouseEventHandler::MousePositionToWorldRay()
{
	FVector2 mousePosition = GetCursorPosition();
	FVector2 screenSize = Engine->GetWindowSize();
	UniformCameraObject* uco = LevelGraph::GetInstance()->GetActiveCamera()->GetUCO();
	FVector4 rayOrigin = FVector4
	(
		(mousePosition.X / screenSize.X - 0.5) * 2.0f,
		(mousePosition.Y / screenSize.Y - 0.5) * 2.0f,
		-1.0f,
		1.0f
	);

	FVector4 rayEnd = FVector4
	(
		(mousePosition.X / screenSize.X - 0.5) * 2.0f,
		(mousePosition.Y / screenSize.Y - 0.5) * 2.0f,
		0.0f,
		1.0f
	);

	FMatrix4 inverse = (uco->Projection * uco->View).GetInverse();

	FVector4 worldRayOrigin = inverse * rayOrigin;
	worldRayOrigin = worldRayOrigin / worldRayOrigin.W;

	FVector4 worldRayEnd = inverse * rayEnd;
	worldRayEnd = worldRayEnd / worldRayEnd.W;

	FVector3 worldRayDirection(worldRayEnd - worldRayOrigin);
	worldRayDirection.Normalize();
	return Ray(FVector3(worldRayOrigin), worldRayDirection);
}

Ray MouseEventHandler::MousePositionToWorldRay(FVector2 mousePosition, FVector2 screenSize, const UniformCameraObject* uco)
{
	FVector4 rayOrigin = FVector4
	(
		(mousePosition.X / screenSize.X - 0.5) * 2.0f,
		(mousePosition.Y / screenSize.Y - 0.5) * 2.0f,
		-1.0f,
		1.0f
	);

	FVector4 rayEnd = FVector4
	(
		(mousePosition.X / screenSize.X - 0.5) * 2.0f,
		(mousePosition.Y / screenSize.Y - 0.5) * 2.0f,
		0.0f,
		1.0f
	);

	FMatrix4 inverse = (uco->Projection * uco->View).GetInverse();
	
	FVector4 worldRayOrigin = inverse * rayOrigin;
	worldRayOrigin = worldRayOrigin / worldRayOrigin.W;

	FVector4 worldRayEnd = inverse * rayEnd;
	worldRayEnd = worldRayEnd / worldRayEnd.W;

	FVector3 worldRayDirection(worldRayEnd - worldRayOrigin);
	worldRayDirection.Normalize();
	return Ray(worldRayOrigin, worldRayDirection);
}

void MouseEventHandler::UpdateCursorPosition()
{
	int x, y, w, h;
	SDL_GetMouseState(&x, &y);

	y = Engine->GetWindowSize().Y - y;	//Y Axis is inverted because SDL treats screen space with Up = 0 Low = 1;
	PreviousCursorPosition = CursorPosition;
	CursorPosition = { x, y };
}


