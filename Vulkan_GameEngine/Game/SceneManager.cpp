#include "SceneManager.h"



SceneManager::SceneManager() : GameInterface(),  currentSceneNum(0), currentLevel(nullptr)
{

}


SceneManager::~SceneManager()
{
	delete currentLevel;
	currentLevel = nullptr;
}

bool SceneManager::OnCreate()
{


	if (CoreEngine::GetInstance()->GetCurrentScene() == 0) {
		currentLevel = new L_TetrahedronLevel();
		if (!currentLevel->Initialize()) {
			DebugLogger::FatalError("Scene has failed on create", "Game1", __LINE__);
			return false;
		}
		currentLevel->Start();
		return true;
	}
	
	DebugLogger::FatalError("Engine scene number is not zero", "Game1.cpp", __LINE__);
	return false;
}

void SceneManager::PreUpdate(const float deltaTime_)
{
	currentLevel->PreUpdate(deltaTime_);
}

void SceneManager::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}
	currentLevel->Update(deltaTime_);
}

void SceneManager::PostUpdate(const float deltaTime_)
{
	currentLevel->PostUpdate(deltaTime_);
}

void SceneManager::Render()
{
	currentLevel->Render();
}

void SceneManager::BuildScene()
{
	//delete currentScene;
	//currentScene = nullptr;

	switch (CoreEngine::GetInstance()->GetCurrentScene()) {

	case 1:
		//currentScene = new GameScene();
		break;
	default:
		//currentScene = new StartScene();
		break;
	}
	/*
	if (!currentScene->OnCreate()) {
		DebugManager::FatalError("Scene has failed on create", "Game1", __LINE__);
		CoreEngine::GetInstance()->Exit();
	}
	*/
	currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
}
