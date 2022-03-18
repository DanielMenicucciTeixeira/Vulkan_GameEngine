#include "AudioHandler.h"

std::unique_ptr<AudioHandler> AudioHandler::audioInstance = nullptr;
std::map<std::string, FMOD::Sound* > AudioHandler::soundPtrList = std::map<std::string, FMOD::Sound*>();
std::map<int, FMOD::Channel*> AudioHandler::channelList = std::map<int, FMOD::Channel*>();

AudioHandler::AudioHandler() : systemPtr(nullptr), channelCount(0)
{
	fmod_pos.x = 0; fmod_pos.y = 0; fmod_pos.z = 0;
	fmod_vel.x = 0; fmod_vel.y = 0; fmod_vel.z = 0;
	fmod_forward.x = 0; fmod_forward.y = 0; fmod_forward.z = 0;
	fmod_up.x = 0; fmod_up.y = 0; fmod_up.z = 0;
}

AudioHandler::~AudioHandler() {
}

AudioHandler * AudioHandler::GetInstance()
{
	if (audioInstance.get() == nullptr) {
		audioInstance.reset(new AudioHandler);
	}
	return audioInstance.get();
}

bool AudioHandler::Initialize(glm::vec3 position_, glm::vec3 velocity_, glm::vec3 forward_, glm::vec3 up_)
{
	FMOD::System_Create(&systemPtr);
	
	int driverNum;
	systemPtr->getNumDrivers(&driverNum);
	if (driverNum == 0) {
		DebugLogger::Error("No sound drivers detected", "AudioHandler.cpp", __LINE__);
		return false;
	}

	systemPtr->init(10, FMOD_INIT_NORMAL | FMOD_3D | FMOD_INIT_3D_RIGHTHANDED, nullptr);

	fmod_pos = glmToFMOD(position_);
	fmod_vel = glmToFMOD(velocity_);
	fmod_forward = glmToFMOD(forward_);
	fmod_up = glmToFMOD(up_);
    systemPtr->set3DListenerAttributes(1, &fmod_pos, &fmod_vel, &fmod_forward, &fmod_up);
	
    return true;
}

void AudioHandler::OnDestroy()
{
	for (auto& d : soundPtrList) {
		d.second->release();
		d.second = nullptr;
	}
	soundPtrList.clear();

	for (auto& d : channelList) {
		d.second->stop();
		d.second = nullptr;
	}
	channelList.clear();

	systemPtr->release();
	systemPtr = nullptr;
}

void AudioHandler::Update(const float deltaTime_)
{
	systemPtr->update();
}

FMOD_VECTOR AudioHandler::glmToFMOD(glm::vec3 vec_)
{
	FMOD_VECTOR vec;
	vec.x = vec_.x;
	vec.y = vec_.y;
	vec.z = vec_.z;
	return vec;
}

void AudioHandler::LoadSound(const std::string name_, bool loop_, bool is3D_, bool stream_)
{
	if (GetSound(name_)) {
		DebugLogger::Error("Sound already loaded", "AudioHandler.cpp", __LINE__);
		return;
	}

	FMOD_MODE mode = FMOD_DEFAULT;

	mode |= loop_ ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	mode |= is3D_ ? FMOD_3D : FMOD_2D;

	mode |= stream_ ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* sound_ = nullptr;

	std::string path_ = "./Resources/Audio/" + name_;
	
	int result = systemPtr->createSound(path_.c_str(), mode, nullptr, &sound_);

	if (result == FMOD_OK) {
		soundPtrList[name_] = sound_;
		DebugLogger::Info("Sound sucssessfully loaded", "AudioHandler.cpp", __LINE__);
		sound_ = nullptr;
		return;
	}
	DebugLogger::Error("Sound failed to load", "AudioHandler.cpp", __LINE__);

}

FMOD::Sound* AudioHandler::GetSound(std::string name_)
{
	return soundPtrList[name_];
}

int AudioHandler::PlaySound(std::string name_, glm::vec3 position_, glm::vec3 velocity_, float volume_, bool loop_, bool is3D_, bool stream_)
{
	int channelID = -1;
	FMOD::Channel* channel_ = nullptr;

	if (!Initialize(position_, velocity_)) {
		Initialize(position_,velocity_);
	}

	if (!GetSound(name_)) {
		LoadSound(name_, loop_, is3D_, stream_);
	}

	systemPtr->playSound(GetSound(name_), nullptr, true, &channel_);
	
	if (!channel_) {
		DebugLogger::Error("Channel failed to load", "AudioHandler", __LINE__);
		return channelID;
	}

	FMOD_MODE curMode;
	GetSound(name_)->getMode(&curMode);

	if (curMode & FMOD_3D) {
		channel_->set3DAttributes(&fmod_pos, nullptr);
	}
	
	channel_->setVolume(volume_);
	channel_->setPaused(false);

	channelID = channelCount;
	channelCount++;

	channelList.emplace(channelID, channel_);
	channel_ = nullptr;
	return channelID;
}

void AudioHandler::UpdateChannelPositionVelocity(int channelID_, glm::vec3 position_, glm::vec3 velocity_)
{
	if(channelList[channelID_] == nullptr){
		DebugLogger::Error("Channel not found", "AudioHandler.cpp", __LINE__);
		return;
	}
	channelList[channelID_]->set3DAttributes(&fmod_pos, &fmod_vel);
}

bool AudioHandler::IsPlaying(int channelID_)
{
	if (channelList[channelID_] == nullptr) {
		DebugLogger::Error("Channel not found", "AudioHandler.cpp", __LINE__);
		return false;
	}
	bool isPlaying = false;

	channelList[channelID_]->isPlaying(&isPlaying);

	return isPlaying;
}
