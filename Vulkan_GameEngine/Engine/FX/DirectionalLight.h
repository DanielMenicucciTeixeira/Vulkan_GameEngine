#ifndef GO_DIRECTIONALLIGHT_H
#define GO_DIRECTIONALLIGHT_H

#include "../FX/LightSource.h"

class GO_DirectionalLight : public GO_LightSource
{
public:
	GO_DirectionalLight(std::string name = "");
	void SetLightType(E_LightType type)  = delete;
};
#endif

