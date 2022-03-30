#ifndef SKYBOX_H
#define SKYBOX_H

#include "GameObject.h"

class C_StaticMeshComponent;

class GO_Skybox : public O_GameObject
{
public:
    GO_Skybox(L_Level* level, std::string name);

    C_StaticMeshComponent* Cube;
};
#endif

