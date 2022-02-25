#include "BackgroundSound.h"
#include "../Objects/Components/CameraComponent.h"
#include "AudioHandler.h"

BackgroundSound::BackgroundSound(C_CameraComponent* camera_)
{
	camera = camera_;
}

BackgroundSound::~BackgroundSound()
{
}

bool BackgroundSound::OnCreate(const std::string name_, bool loop_, bool is3D_, bool stream_)
{
	channelID = -1;
	fileName = name_;

	AudioHandler::GetInstance()->LoadSound(name_, loop_, is3D_, stream_);

	return true;
}

void BackgroundSound::Update(const float deltaTime_)
{
}

void BackgroundSound::PlaySound()
{
	FVector3 pos = camera->GetComponentAbsolutePosition();
	channelID = AudioHandler::GetInstance()->PlaySound(fileName, glm::vec3(pos.X,pos.Y,pos.Z));
}

bool BackgroundSound::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(channelID);
}
