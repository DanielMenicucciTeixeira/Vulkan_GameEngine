#ifndef GAME_H
#define GAME_H

#include <set>
#include <map>
#include <string>

class Clock;
class O_Object;
class O_GameObject;
class O_Level;
class Renderer;
class SDLManager;
struct S_Mesh;
struct S_Material;
struct RenderInitializationData;

class Game
{
public:
	Game();
	virtual ~Game();

	virtual void Start();
	virtual void HandleEvents();
	virtual void Update();
	void SetPause(const bool& pause);
	inline bool IsPaused() { return Paused; };
	
	void LoadLevel(O_Level* level);
	void LoadLevel(std::string levelName);
	void LoadMesh(S_Mesh* mesh);
	void LoadMaterial(S_Material* material);
	void LoadLevelObjects(std::set<O_Object*> levelObjects);

	int Run();

	S_Mesh* GetMesh(std::string meshName);
	S_Material* GetMaterial(std::string materialName);

protected:
	Clock* GameClock;
	O_Level* CurrentLevel;
	SDLManager* InterfaceManager;
	Renderer* GameRenderer;
	
	std::map<std::string, O_Level*> Levels;
	std::map<std::string, S_Mesh*> Meshes;
	std::map<std::string, S_Material*> Materials;
	RenderInitializationData* RenderData;

	bool Paused = false;
};

#endif

