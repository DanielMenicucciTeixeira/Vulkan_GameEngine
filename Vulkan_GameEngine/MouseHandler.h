#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include "Auxiliary/Singleton.h"

class CoreEngine;
class BaseGame;
class IVector2;
union SDL_Event;

class MouseHandler : public StaticClass<MouseHandler>
{
public:
	static void SetEngineReference(CoreEngine* engine);
	static void SetGameReference(BaseGame* game);
	static void Initialize();

	static const IVector2& GetPreviousCursorPosition();
	static const IVector2& GetCursorPosition();
	static IVector2 GetCursorOffset();

protected:
	static CoreEngine* Engine;
	static BaseGame* Game;
	static IVector2 CursorPosition, PreviousCursorPosition;
	static void UpdateCursorPosition();
	friend class EventHandler;

};
#endif

