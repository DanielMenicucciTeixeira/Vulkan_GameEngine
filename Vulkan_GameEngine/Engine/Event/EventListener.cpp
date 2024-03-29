#include "EventListener.h"
#include "../Core/CoreEngine.h"
#include "../Objects/Object.h"
#include "../Renderers/Renderer.h"
#include "../Core/LevelGraph.h"
#include "MouseEventHandler.h"
#include "../Core/DebugLogger.h"

#include <SDL.h>
#include <string>

std::unordered_map<inputKey, std::set<eventName_t>, EventListener::HASH_InputKey> EventListener::EventsByInput;
std::unordered_map<inputKey, functionMap_t, EventListener::HASH_InputKey> EventListener::InputMap;
std::unordered_map<eventName_t, std::set<inputFunction_t>> EventListener::EventMap;
functionMap_t EventListener::FunctionMap;

void EventListener::HandleEvents()
{
	SDL_Event event;
	inputKey key;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EventType::SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				CoreEngine::GetInstance()->GetRenderer()->FramebufferResizeCallback();
			}
			break;
		case SDL_EventType::SDL_QUIT:
			CoreEngine::GetInstance()->Quit(&event);
		}
		
		key.first = event.type;

		//If the event is keyboard or button realted, set key.second
		switch (key.first)
		{
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERBUTTONDOWN:
			key.second = event.cbutton.button;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			key.second = event.key.keysym.sym;
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			key.second = event.button.button;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventHandler::UpdateCursorPosition();
			break;
		default:
			key.second = SDLK_UNKNOWN;
			break;
		}
		//Then find the function in the map using the event type and keycode as directions, if it exists, call it.
		CallFunctions(key, &event);
	}
}

void EventListener::CallFunctions(inputKey key, SDL_Event* event)
{
	for (const auto& Event : EventsByInput[key])
	{
		for (const auto& function : EventMap[Event])
		{
			auto iterator = FunctionMap[function].begin();
			while (iterator != FunctionMap[function].end())
			{
				if (*iterator)
				{
					function(*iterator, event);
					iterator++;
				}
				else
				{
					FunctionMap[function].erase(*iterator);
				}
			}
			//for (const auto& object : FunctionMap[function]) function(object, event);
		}
	}

	for (auto& function : InputMap[key])
	{
		auto iterator = function.second.begin();
		while (iterator != function.second.end())
		{
			if (*iterator)
			{
				function.first(*iterator, event);
				iterator++;
			}
			else
			{
				function.second.erase(*iterator);
			}
		}
		//for (const auto& object : function.second) function.first(object, event);
	}
}

void EventListener::AddEvent(const char* event)
{
	if (!EventMap.count(event)) EventMap[event] = std::set<inputFunction_t>();
}

bool EventListener::AddInputToEvent(const char* event, sdlEventType type, sdlKeycode keyCode)
{
	if (!EventMap.count(event))
	{
		DebugLogger::Error("No event found with name: " + std::string(event) + ".", "Event/EventListener.cpp", __LINE__);
		return false;
	}
	inputKey key(type, keyCode);
	if (!EventsByInput.count(key)) EventsByInput[key] = std::set<eventName_t>();
	EventsByInput[key].insert(event);
	return true;
}

bool EventListener::AddObjectToFunctionMap(inputFunction_t function, void* object)
{
	if (!function)
	{
		DebugLogger::Error("Invalid function in EventListener::AddObjectToFunctionMap", "Event/EventListener.cpp", __LINE__);
		return false;
	}
	else
	{
		if (!FunctionMap.count(function)) FunctionMap[function] = std::set<void*>();
		FunctionMap[function].insert(object);
		return true;
	}
}

void EventListener::RemoveObjectToFunctionMap(inputFunction_t function, void* object)
{
	if (!object || !function || !!FunctionMap.count(function)) return;
	else
	{
		FunctionMap[function].erase(object);
	}
}

void EventListener::Initialize()
{
	MouseEventHandler::Initialize();
}

EventListener::~EventListener()
{
}

bool EventListener::AddFunctionByInput(void* object, inputFunction_t function, sdlEventType type, sdlKeycode keyCode)
{
	if (!function)
	{

		DebugLogger::Error("Invalid function, no function added!", "Event/EventListener.cpp", __LINE__);
		return false;
	}
	else
	{
		inputKey key(type, keyCode);
		if (!InputMap.count(key)) InputMap[key] = functionMap_t();
		if (!InputMap[key].count(function)) InputMap[key][function] = std::set<void*>();
		InputMap[key][function].insert(object);
		return true;
	}
}

void EventListener::RemoveObjectFromInput(void* object, inputFunction_t function, sdlEventType type, sdlKeycode keyCode)
{
	if (!function || !object) return;
	inputKey key(type, keyCode);
	if (!InputMap.count(key) || !InputMap[key].count(function)) return;
	else InputMap[key][function].erase(object);
}

bool EventListener::AddFunctionByEvent(void* object, inputFunction_t function, eventName_t event)
{
	if (!EventMap.count(event))
	{
		DebugLogger::Error("Undefined Event in EventListener::AddFunctionByEvent", "Event/EventListener.cpp", __LINE__);
		return false;
	}
	else
	{
		EventMap[event].insert(function);
		return AddObjectToFunctionMap(function, object);
	}
}

void EventListener::RemoveFunctionFromEvent(void* object, inputFunction_t function, eventName_t event)
{
	if (!object || !function || !FunctionMap.count(function)) return;
	FunctionMap[function].erase(object);
}

size_t EventListener::HASH_InputKey::operator()(inputKey const& key) const
{
	size_t result = (size_t(key.first) << 32) + size_t(key.second);
	result *= 1231231557ull; // "random" uneven integer 
	result ^= (result >> 32);
	return result;
}
