#ifndef _RENDER_STATES_H_
#define _RENDER_STATES_H_

#include <D3D11.h>

//Here we define the most frequently used render states
class RenderStates
{
public:
	static RenderStates* GetInstance()
	{
		if (m_pStates == nullptr)
		{
			m_pStates = new RenderStates();
			//m_pStates->InitAll();//***//need initial in somewhere...
		}

		return m_pStates;
	}
private:
	static RenderStates* m_pStates;
public:
	RenderStates();
	~RenderStates();
	//Initialize all the states
	bool InitAll(ID3D11Device* device);
private:
	void ReleaseAll();

public:
	ID3D11RasterizerState	*WireFrameRS;			//Wire frame mode
	ID3D11RasterizerState	*NoCullRS;				//No back face culling
	ID3D11RasterizerState	*CounterClockFrontRS;	//Set the counter-clock wise as front face
	
	ID3D11BlendState			*TransparentBS;			//Transparent blending state
	ID3D11BlendState			*NoColorWriteBS;		//Forbid color writing
	ID3D11BlendState			*AlphaToCoverageBS;
	ID3D11BlendState			*NoRenderTargetWritesBS;

	ID3D11DepthStencilState	*MarkMirrorDSS;			//Mark the mirror area(Used to draw mirrors)
	ID3D11DepthStencilState	*DrawReflectionDSS;		//Draw reflected objects in the mirror(Used to draw mirrors)
	ID3D11DepthStencilState	*NoDoubleBlendingDSS;	//Avoid double-blending(Used to draw planar-shadows)

	ID3D11DepthStencilState	*DisableDepthDSS;
};

#endif