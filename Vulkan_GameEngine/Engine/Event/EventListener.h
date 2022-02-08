#ifndef EVENTHANDLER_H
#define ENVENTHANDLER_H

#include <map>
#include <unordered_map>
#include <set>

class O_Object;
union SDL_Event;
typedef uint32_t sdlEventType;
typedef int32_t sdlKeycode;
typedef const char* eventName_t;
using inputKey = std::pair<sdlEventType, sdlKeycode>;
using inputFunction_t = void(*)(O_Object*, SDL_Event*);
using functionMap_t = std::unordered_map <inputFunction_t, std::set<O_Object*>>;

class EventListener
{
public:
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) = delete;
	EventListener& operator =(const EventListener&) = delete;
	EventListener& operator =(EventListener&&) = delete;

	EventListener() = delete;
	~EventListener();

	static bool AddFunctionByInput(O_Object* object, inputFunction_t function, sdlEventType type, sdlKeycode keyCode = 0);
	static void RemoveObjectFromInput(O_Object* object, inputFunction_t function, sdlEventType type, sdlKeycode keyCode = 0);
	static bool AddFunctionByEvent(O_Object* object, inputFunction_t function, eventName_t event);
	static void RemoveFunctionFromEvent(O_Object* object, inputFunction_t function, eventName_t event);
	inline static std::unordered_map<eventName_t, std::set<inputFunction_t>> GetEventMap() { return EventMap; }

protected:
	static void AddEvent(const char* event);
	static bool AddInputToEvent(const char* event, sdlEventType type, sdlKeycode keyCode = 0);
	static bool AddObjectToFunctionMap(inputFunction_t function, O_Object* object);
	static void RemoveObjectToFunctionMap(inputFunction_t function, O_Object* object);
	static void Initialize();
	static void HandleEvents();
	static void CallFunctions(inputKey key, SDL_Event* event);


	
	struct HASH_InputKey
	{
		size_t operator()(inputKey const& vertex) const;
	};

	static std::unordered_map<inputKey, std::set<eventName_t>, HASH_InputKey> EventsByInput;
	static std::unordered_map<inputKey, functionMap_t, HASH_InputKey> InputMap;
	static std::unordered_map<eventName_t, std::set<inputFunction_t>> EventMap;
	static functionMap_t FunctionMap;
	friend class CoreEngine;
};
#endif

