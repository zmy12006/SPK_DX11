////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include "WICTextureLoader.h"
#include "graphicsclass.h"

#include "SPK_Example.h"
#include "SPK_ExampleExp.h"

//#define Math_PI 3.1415926f

using namespace DirectX;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, -2.0f, -10.0f);
	
	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"../Engine/data/star.dds");
	if(!result)
	{
		return false;
	}

#ifdef USE_SPARK
	SPK_Init();
#endif // USE_SPARK


	return true;
}


void GraphicsClass::Shutdown()
{
#ifdef USE_SPARK
	SPK_Release();
#endif // USE_SPARK
	// Release the particle system object.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(float frameTime)
{
	bool result;


#ifdef USE_SPARK
	SPK_Move();
#endif // USE_SPARK

	//if (particleSystem && particleSystem->getGroup(0))
	//{
	//	const Pool<Particle>& particles = particleSystem->getGroup(0)->getParticles();
	//	std::vector<SimpleMath::Vector3> posSet;
	//	for (int i = 0; i < particles.size(); i++)
	//	{
	//		SimpleMath::Vector3 pos;
	//		pos.x = particles[i].position().x;
	//		pos.y = particles[i].position().y;
	//		pos.z = particles[i].position().z;
	//		posSet.push_back(pos);
	//	}
	//	m_ParticleSystem->FrameEx(frameTime, m_D3D->GetDeviceContext(), posSet);
	//}
		

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	SimpleMath::Matrix worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	SimpleMath::Matrix tmpWorld = viewMatrix;
	tmpWorld._41 = tmpWorld._42 = tmpWorld._43 = 0.0f;
	worldMatrix = tmpWorld.Invert();

	// Turn on alpha blending.
	m_D3D->EnableAlphaBlending();

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	//result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
	//								  m_ParticleSystem->GetTexture());
	//if(!result)
	//{
	//	return false;
	//}

#ifdef USE_SPARK
	SimpleMath::Matrix MatWorld;
	m_D3D->GetWorldMatrix(MatWorld);
	DX11Info::MatWorld = MatWorld;
	DX11Info::MatWorldInv = worldMatrix;
	DX11Info::MatView = viewMatrix;
	DX11Info::MatProj = projectionMatrix;

	//SPK_Init();
	//if (particleSystem != NULL)
	//{
	//	particleSystem->render();
	//	// Render the model using the texture shader.
	//	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//		m_ParticleSystem->GetTexture());
	//}
	if (exampleBase != NULL)
	{
		exampleBase->Render();
	}

#endif // USE_SPARK

	// Turn off alpha blending.
	m_D3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::SPKRender(int indexCount, DirectX::XMFLOAT4X4  worldMatrix1, DirectX::XMFLOAT4X4  viewMatrix1,
	DirectX::XMFLOAT4X4 projectionMatrix1, ID3D11ShaderResourceView* texture)
{
	SimpleMath::Matrix worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//SimpleMath::Matrix tmpWorld = viewMatrix;
	//tmpWorld._41 = tmpWorld._42 = tmpWorld._43 = 0.0f;
	//worldMatrix = tmpWorld.Invert();

	m_ParticleShader->Render(m_D3D->GetDeviceContext(), indexCount, worldMatrix, viewMatrix, projectionMatrix,
		texture);
}

#ifdef USE_SPARK
void GraphicsClass::SPK_Init()
{
	/////////////////////////
// SPARK initial setup //
/////////////////////////

// random seed
	unsigned int randomSeed = static_cast<unsigned int>(time(NULL));

	// Sets the update step
	System::setClampStep(true, 0.1f);			// clamp the step to 100 ms
	System::useAdaptiveStep(0.001f, 0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)

	// Sets the device for SPARK DX11 rendering
	DX11Info::setDevice(m_D3D->GetDevice());
	DX11Info::setContext(m_D3D->GetDeviceContext());
	
//	////////////////////////////
//	// Loads particle texture //
//	////////////////////////////
//
//	ID3D11ShaderResourceView* textureParticle = NULL;
//	HRESULT hr = DirectX::CreateWICTextureFromFile(m_D3D->GetDevice(), L"res/point.bmp", NULL, &textureParticle);
//	//hr = D3DXCreateTextureFromFile(g_pD3DDevice, L"res/point.bmp", &textureParticle);
//	if (FAILED(hr))
//		cout << "erreur chargement texture" << endl;
//
//	/////////////////////////////
//	// Creates particle system //
//	/////////////////////////////
////*
//
//	quadRenderer = DX11QuadRenderer::create(); // quad renderer
//	quadRenderer->enableBlending(true);
//	//quadRenderer->setBlendingFunctions(D3DBLEND_SRCALPHA, D3DBLEND_ONE);
//	quadRenderer->setTexturingMode(TEXTURE_2D);
//	quadRenderer->setTexture(textureParticle);
//	//quadRenderer->setTextureBlending(D3DTOP_MODULATE);
//	//quadRenderer->setScale(0.05f, 0.05f);
//	quadRenderer->setScale(0.2f, 0.2f);
//	//*/
//
//		// Model
//	particleModel = Model::create(FLAG_RED | FLAG_SIZE | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA);
//	particleModel->setParam(PARAM_ALPHA, 0.8f); // constant alpha
//	particleModel->setLifeTime(8.0f, 8.0f);
//	particleModel->setParam(PARAM_SIZE, 5.0, 10.0f);
//
//	// Emitter
//	particleEmitter = SphericEmitter::create(Vector3D(0.0f, 1.0f, 0.0f), 0.1f * MATH_PI, 0.1f * MATH_PI);
//	particleEmitter->setZone(Point::create(Vector3D(0.0f, 0.016f, 0.0f)));
//	particleEmitter->setFlow(250);
//	particleEmitter->setForce(1.5f, 1.5f);
//
//	// Modifier
//	groundObstacle = Obstacle::create(Plane::create(), INTERSECT_ZONE, 0.6f, 1.0f);
//
//	// Group
//	particleGroup = Group::create(particleModel, 2100);
//	particleGroup->addEmitter(particleEmitter);
//	particleGroup->addModifier(groundObstacle);
//	particleGroup->setRenderer(quadRenderer);
//	particleGroup->setGravity(Vector3D(0.0f, -0.8f, 0.0f));
//
//	// System
//	particleSystem = System::create();
//	particleSystem->addGroup(particleGroup);

	exampleBase = new SPK_ExampleExp();
	exampleBase->Init(this);

	/////////////////////////////////////
	// Traces SPARK registered objects //
	/////////////////////////////////////

	cout << "\nSPARK FACTORY AFTER INIT :" << endl;
	SPKFactory::getInstance().traceAll();
}

void GraphicsClass::SPK_Move()
{
	if (exampleBase == NULL) return;

	float deltatime = 0.01f;
	
	exampleBase->Update(deltatime);
}

void GraphicsClass::SPK_Release()
{
	cout << "\nSPARK FACTORY BEFORE DESTRUCTION :" << endl;
	SPKFactory::getInstance().traceAll();
	SPKFactory::getInstance().destroyAll();
	cout << "\nSPARK FACTORY AFTER DESTRUCTION :" << endl;
	SPKFactory::getInstance().traceAll();
	SPKFactory::destroyInstance();
}

#endif