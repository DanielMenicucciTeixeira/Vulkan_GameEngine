#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <string>
#include <map>
#include <memory>
#include <fmod.hpp>
#include <glm/glm.hpp> 
#include "../Core/DebugLogger.h"
#include "AudioSource.h"
#include "BackgroundSound.h"

class AudioHandler {
public:
	AudioHandler(const AudioHandler&) = delete;
	AudioHandler(AudioHandler&&) = delete;
	AudioHandler& operator =(const AudioHandler&) = delete;
	AudioHandler& operator =(AudioHandler&&) = delete;

	static AudioHandler* GetInstance();

	bool Initialize(glm::vec3 position_ = glm::vec3(0), glm::vec3 velocity_ = glm::vec3(0),
		glm::vec3 forward_ = glm::vec3(0), glm::vec3 up_ = glm::vec3(0));
	void OnDestroy();
	void Update(const float deltaTime_);

private:
	AudioHandler();
	~AudioHandler();

	static std::unique_ptr<AudioHandler> audioInstance;
	friend std::default_delete<AudioHandler>;

	friend class AudioSource;
	friend class BackgroundSound;

	FMOD::System * systemPtr;
	static std::map<std::string, FMOD::Sound* > soundPtrList;
	static std::map<int, FMOD::Channel*> channelList;
	int channelCount;

	FMOD_VECTOR glmToFMOD(glm::vec3 vec_);

	void LoadSound(const std::string name_, bool loop_, bool is3D_, bool stream_);

	FMOD::Sound* GetSound(std::string name_);

	//Function to play sound that returns the channel ID
	int PlaySound(std::string name_, glm::vec3 position_ = glm::vec3(0),
		glm::vec3 velocity_ = glm::vec3(0), float volume_ = 1.0f);

	void UpdateChannelPositionVelocity(int channelID_, glm::vec3 position_ = glm::vec3(0),
		glm::vec3 velocity_ = glm::vec3(0));

	//Function to check if a specific channel is playing a sound
	bool IsPlaying(int channelID_);

	FMOD_VECTOR fmod_pos;
	FMOD_VECTOR fmod_vel;
	FMOD_VECTOR fmod_forward;
	FMOD_VECTOR fmod_up;
};
#endif // !AUDIOHANDLER_H