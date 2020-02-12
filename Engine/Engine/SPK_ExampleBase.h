#pragma once

//// SPARK lib
#include <SPK.h>
#include <SPK_DX11.h>
//
//using namespace SPK;
//using namespace SPK::DX11;
#include "SimpleMath.h"
#include "particleshaderclass.h"

using namespace DirectX;

class SPK_ExampleBase
{
public:
	virtual void Init(SPK::DX11::DX11RenderShader* shader) = 0;
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
};