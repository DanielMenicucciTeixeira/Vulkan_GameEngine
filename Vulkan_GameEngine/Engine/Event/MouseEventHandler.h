#ifndef MOUSEEVENTHANDLER_H
#define MOUSEEVENTHANDLER_H


class CoreEngine;
class BaseGame;
class IVector2;
class FVector2;
class Ray;
struct UniformCameraObject;
union SDL_Event;

class MouseEventHandler
{
public:
	MouseEventHandler(const MouseEventHandler&) = delete;
	MouseEventHandler(MouseEventHandler&&) = delete;
	MouseEventHandler& operator =(const MouseEventHandler&) = delete;
	MouseEventHandler& operator =(MouseEventHandler&&) = delete;

	MouseEventHandler() = delete;
	~MouseEventHandler();


	static void SetEngineReference(CoreEngine* engine);
	static void SetGameReference(BaseGame* game);
	static void Initialize();

	static const IVector2& GetPreviousCursorPosition();
	static const IVector2& GetCursorPosition();
	static IVector2 GetCursorOffset();
	static Ray MousePositionToWorldRay();
	static Ray MousePositionToWorldRay(FVector2 mousePosition, FVector2 screenSize, const UniformCameraObject* uco);

protected:
	static CoreEngine* Engine;
	static BaseGame* Game;
	static IVector2 CursorPosition, PreviousCursorPosition;
	static void UpdateCursorPosition();
	friend class EventListener;

};
#endif

