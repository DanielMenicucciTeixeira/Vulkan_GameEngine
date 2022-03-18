#include "BackgroundSound.h"
#include "../Objects/Components/CameraComponent.h"
#include "AudioHandler.h"

BackgroundSound::BackgroundSound(C_CameraComponent* camera_)
{
	camera = camera_;
	channelID = -1;
}

BackgroundSound::~BackgroundSound()
{
}

bool BackgroundSound::OnCreate()
{
	return true;
}

void BackgroundSound::Update(const float deltaTime_)
{
}

void BackgroundSound::PlaySound(const std::string name_, float volume_, bool loop_, bool is3D_, bool stream_)
{
	FVector3 cam_pos = camera->GetComponentAbsolutePosition();
	//FVector3 cam_pos = FVector3();
	FVector3 cam_vec = FVector3();
	
	channelID = AudioHandler::GetInstance()->PlaySound(name_, glm::vec3(cam_pos.X, cam_pos.Y, cam_pos.Z), glm::vec3(cam_vec.X, cam_vec.Y, cam_vec.Z), volume_, loop_, is3D_, stream_);
}

bool BackgroundSound::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(channelID);
}
