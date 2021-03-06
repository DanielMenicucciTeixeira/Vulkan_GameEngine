#ifndef GO_CAMERA_H
#define GO_CAMERA_H

#include "Objects/GameObjects/GameObject.h"

class C_CameraComponent;
class C_MovementComponent;

class GO_Camera : public O_GameObject
{
public:
	GO_Camera(L_Level* level, std::string name = "");
	virtual void Start();
	inline C_CameraComponent* GetCamera() const { return Camera; }

protected:
	C_CameraComponent* Camera;
};
#endif

