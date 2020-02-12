#pragma once

// SPARK lib
#include <SPK.h>
#include <SPK_DX11.h>

#include "SPK_ExampleBase.h"

using namespace SPK;
using namespace SPK::DX11;

class SPK_Example : public SPK_ExampleBase
{
public:
	SPK_Example() {}
	void Init(SPK::DX11::DX11RenderShader* shader);
	void Render();
	void Update(float dt);
	 
private:
	SPK::System* particleSystem = NULL;
	SPK::Group* particleGroup = NULL;
	SPK::Model* particleModel = NULL;

	//*
	SPK::DX11::DX11QuadRenderer* quadRenderer = NULL;
	//*/

	SPK::SphericEmitter *particleEmitter = NULL;
	SPK::Obstacle* groundObstacle = NULL;
};

