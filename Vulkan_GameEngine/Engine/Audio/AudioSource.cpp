#include "AudioSource.h"
#include "AudioHandler.h"
#include "../Objects/GameObjects/GameObject.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Math/FVector3.h"



AudioSource::AudioSource(O_GameObject* parent_) : O_Component(parent_)
{
	Owner = parent_;
	channelID = -1;
}

AudioSource::~AudioSource()
{

}

bool AudioSource::OnCreate(const std::string name_, bool loop_, bool is3D_, bool stream_)
{
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

void AudioSource::PlaySound(const std::string name_, float volume_, bool loop_, bool is3D_, bool stream_)
{
	FVector3 O_pos = Owner->GetPosition();
	FVector3 O_vec = Owner->GetComponentOfClass<C_PhysicsComponent>()->GetVelocity();
	channelID = AudioHandler::GetInstance()->PlaySound(name_, glm::vec3(O_pos.X, O_pos.Y, O_pos.Z), glm::vec3(O_vec.X, O_vec.Y, O_vec.Z), volume_, loop_, is3D_, stream_);
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
