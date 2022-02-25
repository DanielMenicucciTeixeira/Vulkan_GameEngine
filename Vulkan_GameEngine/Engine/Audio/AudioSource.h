#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "../Objects/Components/Component.h"
#include <string>


class AudioSource : public O_Component {
public:
	AudioSource(O_GameObject* parent_);
	~AudioSource();

	bool OnCreate(const std::string name_,
		bool loop_, bool is3D_, bool stream_);
	void Update(const float deltaTime_) override;

	void PlaySound();
	bool IsPlaying();

private:
	std::string fileName;
	int channelID;

	//void Draw() override;
	//bool FindContainingPoint() override;


};

#endif // !AUDIOSOURCE_H