#include "AudioHandler.h"


std::unique_ptr<AudioHandler> AudioHandler::audioInstance = nullptr;
std::map<std::string, FMOD::Sound* > AudioHandler::soundPtrList = std::map<std::string, FMOD::Sound*>();
std::map<int, FMOD::Channel*> AudioHandler::channelList = std::map<int, FMOD::Channel*>();

AudioHandler::AudioHandler() : systemPtr(nullptr), channelCount(0)
{

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

	//TODO: Fix requires l-Value 
//systemPtr->set3DListenerAttributes(1, &glmToFMOD(position_),&glmToFMOD(velocity_),
//	&glmToFMOD(forward_), &glmToFMOD(up_));

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

	systemPtr->createSound(path_.c_str(), mode, nullptr, &sound_);




	if (sound_) {
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

int AudioHandler::PlaySound(std::string name_, glm::vec3 position_, glm::vec3 velocity_, float volume_)
{
	int channelID = -1;
	if (!GetSound(name_)) {
		LoadSound(name_, false, true, false);
	}
	FMOD::Channel* channel_ = nullptr;

	systemPtr->playSound(GetSound(name_), nullptr, true, &channel_);
	if (!channel_) {
		DebugLogger::Error("Channel failed to load", "AudioHandler", __LINE__);
		return channelID;
	}
	FMOD_MODE curMode;
	GetSound(name_)->getMode(&curMode);
	if (curMode & FMOD_3D){
		//TODO: Fix requires l-value
		//channel_->set3DAttributes(&glmToFMOD(position_), nullptr);
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

	//TODO: Fix requires l-value
	//channelList[channelID_]->set3DAttributes(&glmToFMOD(position_), &glmToFMOD(velocity_));

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
