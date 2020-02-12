#include <iostream>
#include "WICTextureLoader.h"
#include "SimpleMath.h"
#include "SPK_Example.h"

using namespace std;


void SPK_Example::Init(SPK::DX11::DX11RenderShader* shader)
{
	////////////////////////////
	// Loads particle texture //
	////////////////////////////

	ID3D11ShaderResourceView* textureParticle = NULL;
	HRESULT hr = DirectX::CreateWICTextureFromFile(DX11Info::getDevice(), L"res/point.bmp", NULL, &textureParticle);
	//hr = D3DXCreateTextureFromFile(g_pD3DDevice, L"res/point.bmp", &textureParticle);
	if (FAILED(hr))
		cout << "erreur chargement texture" << endl;

	/////////////////////////////
	// Creates particle system //
	/////////////////////////////
//*

	quadRenderer = DX11QuadRenderer::create(); // quad renderer
	quadRenderer->enableBlending(true);
	//quadRenderer->setBlendingFunctions(D3DBLEND_SRCALPHA, D3DBLEND_ONE);
	quadRenderer->setTexturingMode(TEXTURE_2D);
	quadRenderer->setTexture(textureParticle);
	//quadRenderer->setTextureBlending(D3DTOP_MODULATE);
	//quadRenderer->setScale(0.05f, 0.05f);
	quadRenderer->setScale(0.2f, 0.2f);
	//*/
	quadRenderer->SetRender(shader);

		// Model
	particleModel = Model::create(FLAG_RED | FLAG_SIZE | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA);
	particleModel->setParam(PARAM_ALPHA, 0.8f); // constant alpha
	particleModel->setLifeTime(8.0f, 8.0f);
	particleModel->setParam(PARAM_SIZE, 5.0, 10.0f);

	// Emitter
	particleEmitter = SphericEmitter::create(Vector3D(0.0f, 1.0f, 0.0f), 0.1f * MATH_PI, 0.1f * MATH_PI);
	particleEmitter->setZone(Point::create(Vector3D(0.0f, 0.016f, 0.0f)));
	particleEmitter->setFlow(250);
	particleEmitter->setForce(1.5f, 1.5f);

	// Modifier
	groundObstacle = Obstacle::create(Plane::create(), INTERSECT_ZONE, 0.6f, 1.0f);

	// Group
	particleGroup = Group::create(particleModel, 2100);
	particleGroup->addEmitter(particleEmitter);
	particleGroup->addModifier(groundObstacle);
	particleGroup->setRenderer(quadRenderer);
	particleGroup->setGravity(Vector3D(0.0f, -0.8f, 0.0f));

	// System
	particleSystem = System::create();
	particleSystem->addGroup(particleGroup);
}

void SPK_Example::Render()
{
	if (particleSystem != NULL)
	{
		particleSystem->render();
		//pShader->Render( DX11Info::getContext(), )
	}
}

void SPK_Example::Update(float dt)
{
	if (particleSystem == NULL) return;

	float deltatime = 0.01f;
	static float step = 0.0f;
	step += 0.01f * 0.5f;
	particleModel->setParam(PARAM_RED, 0.6f + 0.4f * sin(step));
	particleModel->setParam(PARAM_GREEN, 0.6f + 0.4f * sin(step + MATH_PI * 2.0f / 3.0f));
	particleModel->setParam(PARAM_BLUE, 0.6f + 0.4f * sin(step + MATH_PI * 4.0f / 3.0f));

	// Updates particle system
	particleSystem->update(deltatime);	// 1 defined as a second
}