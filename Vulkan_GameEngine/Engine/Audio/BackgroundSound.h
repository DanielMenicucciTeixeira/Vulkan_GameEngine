#pragma once
#include <string>
class C_CameraComponent;

class BackgroundSound {
public: 
	BackgroundSound(C_CameraComponent* camera_);
	~BackgroundSound();

	bool OnCreate();
	void Update(const float deltaTime_);

	void PlaySound(const std::string name_, float volume_ ,bool loop_, bool is3D_, bool stream_);
	bool IsPlaying();

private:
	std::string fileName;
	int channelID;

	C_CameraComponent* camera;
};