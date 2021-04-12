#include "MouseHandler.h"
#include "Game.h"
#include "CoreEngine.h"
#include "Math/IVector2.h"
#include "SDL/SDLManager.h"

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

void MouseHandler::UpdateCursorPosition()
{
	int x, y, w, h;
	SDL_GetMouseState(&x, &y);
	SDL_GetWindowSize(SDLManager::GetInstance()->GetSDLWindowByName(), &w, &h);
	y = h - y;	//Y Axis is inverted because SDL treats screen space with Up = 0 Low = 1;
	PreviousCursorPosition = CursorPosition;
	CursorPosition = { x, y };
}


