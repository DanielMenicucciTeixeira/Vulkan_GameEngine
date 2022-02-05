#include "SceneManager.h"


//currentScene(nullptr),
SceneManager::SceneManager() : GameInterface(),  currentSceneNum(0)
{

}


SceneManager::~SceneManager()
{
	//delete currentScene;
	//currentScene = nullptr;
}

bool SceneManager::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0) {
		//currentScene = new StartScene();
		//if (!currentScene->OnCreate()) {
		//	DebugManager::FatalError("Scene has failed on create", "Game1", __LINE__);
		//	return false;
		//}
		return true;
	}
	
	DebugLogger::FatalError("Engine scene number is not zero", "Game1.cpp", __LINE__);
	return false;
}

void SceneManager::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene()) {
		BuildScene();
	}
	//currentScene->Update(deltaTime_);
}

void SceneManager::Render()
{
	//currentScene->Render();
}

//void SceneManager::Draw()
//{
	//currentScene->Draw();
//}

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
