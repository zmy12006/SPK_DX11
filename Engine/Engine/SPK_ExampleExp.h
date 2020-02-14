#pragma once
// SPARK lib
#include <SPK.h>
#include <SPK_DX11.h>

#include "SPK_ExampleBase.h"

using namespace SPK;
using namespace SPK::DX11;

class SPK_ExampleExp : public SPK_ExampleBase
{
public:
	SPK_ExampleExp() {}
	void Init(SPK::DX11::DX11RenderShader* shader);
	void Render();
	void Update(float dt);

private:
	System* createParticleSystem(const Vector3D& pos);

private:
	list<System*> particleSystems;
	SPK_ID BaseSystemID = NO_ID;

};

