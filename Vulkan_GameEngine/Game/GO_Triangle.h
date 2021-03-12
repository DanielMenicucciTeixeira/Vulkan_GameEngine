#ifndef GO_TRIANGLE_H
#define GO_TRIANGLE_H
#include "Objects/GameObjects/GameObject.h"

class C_StaticMeshComponent;

class GO_Triangle :  public O_GameObject
{
public:
	GO_Triangle(O_Level* level);

	C_StaticMeshComponent* Mesh;
};
#endif
