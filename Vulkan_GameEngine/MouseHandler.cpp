#include "MouseHandler.h"
#include "Game.h"
#include "CoreEngine.h"
#include "Math/IVector2.h"
#include "Math/FVector2.h"
#include "Math/FMatrix4.h"
#include "Math/FVector3.h"
#include "Geometry/Ray.h"
#include "Renderers/UniformBufferObject.h"
#include "SDL/SDLManager.h"
#include "LevelGraph.h"
#include "Objects/Components/CameraComponent.h"

#include <SDL.h>

CoreEngine* MouseHandler::Engine;
BaseGame* MouseHandler::Game;
IVector2 MouseHandler::CursorPosition, MouseHandler::PreviousCursorPosition;

void MouseHandler::SetEngineReference(CoreEngine* engine)
{
	Engine = engine;
}

void MouseHandler::SetGameReference(BaseGame* game)
{
	Game = game;
}

void MouseHandler::Initialize()
{
	SDL_Window* window = SDLManager::GetInstance()->GetSDLWindowByName();
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	SDL_WarpMouseInWindow(window, width * 0.5f, height * 0.5f);
	int x, y;
	SDL_GetMouseState(&x, &y);
	y = height - y;
	PreviousCursorPosition.X = CursorPosition.X = x;
	PreviousCursorPosition.Y = CursorPosition.Y = y;
}

const IVector2& MouseHandler::GetPreviousCursorPosition()
{
	return PreviousCursorPosition;
}

const IVector2& MouseHandler::GetCursorPosition()
{
	return CursorPosition;
}

IVector2 MouseHandler::GetCursorOffset()
{
	//Y Axis is inverted because SDL treats screen space with Up = 0 Low = 1;
	return IVector2(CursorPosition.X - PreviousCursorPosition.X, PreviousCursorPosition.Y - CursorPosition.Y);
}

Ray MouseHandler::MousePositionToWorldRay()
{
	FVector2 mousePosition = GetCursorPosition();
	int w, h;
	SDL_GetWindowSize(SDLManager::GetInstance()->GetSDLWindowByName(), &w, &h);
	FVector2 screenSize(w, h);
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

Ray MouseHandler::MousePositionToWorldRay(FVector2 mousePosition, FVector2 screenSize, const UniformCameraObject* uco)
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

void MouseHandler::UpdateCursorPosition()
{
	int x, y, w, h;
	SDL_GetMouseState(&x, &y);
	SDL_GetWindowSize(SDLManager::GetInstance()->GetSDLWindowByName(), &w, &h);
	y = h - y;	//Y Axis is inverted because SDL treats screen space with Up = 0 Low = 1;
	PreviousCursorPosition = CursorPosition;
	CursorPosition = { x, y };
}


