////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#define USE_SPARK

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"

#include "SPK_ExampleBase.h"

#ifdef USE_SPARK
// SPARK lib
#include <SPK.h>
#include <SPK_DX11.h>
#endif // USE_SPARK

#ifdef USE_SPARK
using namespace SPK;
using namespace SPK::DX11;
#endif // USE_SPARK

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass : public DX11RenderShader
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float);

private:
	bool Render();
	void SPKRender(int indexCount, DirectX::XMFLOAT4X4  worldMatrix1, DirectX::XMFLOAT4X4  viewMatrix1,
		DirectX::XMFLOAT4X4 projectionMatrix1, ID3D11ShaderResourceView* texture);

#ifdef USE_SPARK
	void SPK_Init();
	void SPK_Move();
	void SPK_Release();
#endif

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;

#ifdef USE_SPARK
	//SPK::System* particleSystem = NULL;
	//SPK::Group* particleGroup = NULL;
	//SPK::Model* particleModel = NULL;

	////*
	//SPK::DX11::DX11QuadRenderer* quadRenderer = NULL;
	////*/

	//SPK::SphericEmitter *particleEmitter = NULL;
	//SPK::Obstacle* groundObstacle = NULL;

	SPK_ExampleBase* exampleBase = NULL;
#endif // USE_SPARK
};

#endif