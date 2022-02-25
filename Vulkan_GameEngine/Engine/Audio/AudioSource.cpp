#include "AudioSource.h"
#include "AudioHandler.h"
#include "../Objects/GameObjects/GameObject.h"
#include "Math/FVector3.h"



AudioSource::AudioSource(O_GameObject* parent_) : O_Component(parent_)
{
	Owner = parent_;
}

AudioSource::~AudioSource()
{

}

bool AudioSource::OnCreate(const std::string name_,
	bool loop_, bool is3D_, bool stream_)
{
	channelID = -1;
	fileName = name_;

	AudioHandler::GetInstance()->LoadSound(name_, loop_, is3D_, stream_);

	return true;
}

//TODO: make easy way for glm and FVector to translate.
void AudioSource::Update(const float deltaTime_)
{
	FVector3 pos = Owner->GetPosition();
	AudioHandler::GetInstance()->UpdateChannelPositionVelocity(channelID, glm::vec3(pos.X, pos.Y,pos.Z));
}

void AudioSource::PlaySound()
{
	FVector3 pos = Owner->GetPosition();
	channelID = AudioHandler::GetInstance()->PlaySound(fileName, glm::vec3(pos.X, pos.Y, pos.Z));
}

bool AudioSource::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(channelID);
}

/*
void AudioSource::Draw()
{
}

bool AudioSource::FindContainingPoint()
{
	return false;
}
*/
