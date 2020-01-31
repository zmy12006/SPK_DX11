#include "RenderingAPIs/DX11/SPK_DX11RenderStates.h"
#include "RenderingAPIs/DX11/SPK_DX11_DEF.h"
//#include "Common/AppUtil.h"

// ID3D11RasterizerState* RenderStates::WireFrameRS(NULL);
// ID3D11RasterizerState* RenderStates::NoCullRS(NULL);
// ID3D11RasterizerState* RenderStates::CounterClockFrontRS(NULL);
// 
// ID3D11BlendState* RenderStates::TransparentBS(NULL);
// ID3D11BlendState* RenderStates::NoColorWriteBS(NULL);
// ID3D11BlendState* RenderStates::AlphaToCoverageBS(NULL);
// ID3D11BlendState* RenderStates::NoRenderTargetWritesBS(NULL);
// 
// 
// ID3D11DepthStencilState* RenderStates::MarkMirrorDSS(NULL);
// ID3D11DepthStencilState* RenderStates::DrawReflectionDSS(NULL);
// ID3D11DepthStencilState* RenderStates::NoDoubleBlendingDSS(NULL);
// ID3D11DepthStencilState* RenderStates::DisableDepthDSS(NULL);
RenderStates* RenderStates::m_pStates = nullptr;
#define LogMessage(X)
RenderStates::RenderStates()
{
	WireFrameRS = nullptr;
	NoCullRS = nullptr;
	CounterClockFrontRS = nullptr;

	TransparentBS = nullptr;
	NoColorWriteBS = nullptr;
	AlphaToCoverageBS = nullptr;
	NoRenderTargetWritesBS = nullptr;

	MarkMirrorDSS = nullptr;
	DrawReflectionDSS = nullptr;
	NoDoubleBlendingDSS = nullptr;

	DisableDepthDSS = nullptr;
}

RenderStates::~RenderStates()
{
	ReleaseAll();
}

bool RenderStates::InitAll(ID3D11Device *device)
{
	//Wire frame mode
	D3D11_RASTERIZER_DESC wfDesc;
	ZeroMemory(&wfDesc,sizeof(wfDesc));
	wfDesc.FillMode = D3D11_FILL_WIREFRAME;
	wfDesc.CullMode = D3D11_CULL_BACK;
	wfDesc.FrontCounterClockwise = false;
	wfDesc.DepthClipEnable = true;
	if(FAILED(device->CreateRasterizerState(&wfDesc,&WireFrameRS)))
	{
		LogMessage("Create 'WireFrame' rasterizer state failed!");
		return false;
	}

	//No back face culling
	D3D11_RASTERIZER_DESC ncDesc;
	ZeroMemory(&ncDesc,sizeof(ncDesc));
	ncDesc.CullMode = D3D11_CULL_NONE;
	ncDesc.FillMode = D3D11_FILL_SOLID;
	ncDesc.FrontCounterClockwise = false;
	ncDesc.DepthClipEnable = true;
	if(FAILED(device->CreateRasterizerState(&ncDesc,&NoCullRS)))
	{
		LogMessage("Create 'NoCull' rasterizer state failed!");
		return false;
	}

	//Counter-clock wise as front face
	D3D11_RASTERIZER_DESC ccfDesc;
	ZeroMemory(&ccfDesc,sizeof(ccfDesc));
	ccfDesc.CullMode = D3D11_CULL_BACK;
	ccfDesc.FillMode = D3D11_FILL_SOLID;
	ccfDesc.FrontCounterClockwise = true;
	ccfDesc.DepthClipEnable = true;
	if(FAILED(device->CreateRasterizerState(&ccfDesc,&CounterClockFrontRS)))
	{
		LogMessage("Create 'NoCull' rasterizer state failed!");
		return false;
	}
	//Transparency
	D3D11_BLEND_DESC transDesc;
	transDesc.AlphaToCoverageEnable = false;
	transDesc.IndependentBlendEnable = false;
	transDesc.RenderTarget[0].BlendEnable = true;
	transDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	transDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	if(FAILED(device->CreateBlendState(&transDesc,&TransparentBS)))
	{
		LogMessage("Create 'Transparent' blend state failed!");
		return false;
	}

	//Forbid color writing
	D3D11_BLEND_DESC noColorWriteBlendDesc;
	noColorWriteBlendDesc.AlphaToCoverageEnable = false;
	noColorWriteBlendDesc.IndependentBlendEnable = false;
	noColorWriteBlendDesc.RenderTarget[0].BlendEnable = false;
	noColorWriteBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	if(FAILED(device->CreateBlendState(&noColorWriteBlendDesc,&NoColorWriteBS)))
	{
		LogMessage("Create 'No Color Write' blend state failed!");
		return false;
	}

	//Mark mirror area
	D3D11_DEPTH_STENCIL_DESC markMirrorDSSDesc;
	markMirrorDSSDesc.DepthEnable = true;
	markMirrorDSSDesc.DepthFunc = D3D11_COMPARISON_LESS;
	markMirrorDSSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	markMirrorDSSDesc.StencilEnable = true;
	markMirrorDSSDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	markMirrorDSSDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	markMirrorDSSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	markMirrorDSSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	markMirrorDSSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	markMirrorDSSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	markMirrorDSSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	markMirrorDSSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	markMirrorDSSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	markMirrorDSSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	if(FAILED(device->CreateDepthStencilState(&markMirrorDSSDesc,&MarkMirrorDSS)))
	{
		LogMessage("Create 'MarkMirror' depth stencil state failed!");
		return false;
	}

	//Draw objects inside mirror
	D3D11_DEPTH_STENCIL_DESC drawRefDesc;
	drawRefDesc.DepthEnable = true;
	drawRefDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawRefDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawRefDesc.StencilEnable = true;
	drawRefDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	drawRefDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	drawRefDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	drawRefDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	drawRefDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	drawRefDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	if(FAILED(device->CreateDepthStencilState(&drawRefDesc,&DrawReflectionDSS)))
	{
		LogMessage("Create 'DrawReflection' depth stencil state failed!");
		return false;
	}

	//No double-blending
	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable = true;
	noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.StencilEnable = true;
	noDoubleBlendDesc.StencilReadMask = 0xff;
	noDoubleBlendDesc.StencilWriteMask = 0xff;
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	if(FAILED(device->CreateDepthStencilState(&noDoubleBlendDesc,&NoDoubleBlendingDSS)))
	{
		LogMessage("Create 'NoDoubleBlending' depth stencil state failed!");
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC disableDepthDesc;
	disableDepthDesc.DepthEnable = false;
	disableDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	disableDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
 	disableDepthDesc.StencilEnable = false;
 	disableDepthDesc.StencilReadMask = 0xff;
 	disableDepthDesc.StencilWriteMask = 0xff;
 	disableDepthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
 	disableDepthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
 	disableDepthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
 	disableDepthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
 	disableDepthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
 	disableDepthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
 	disableDepthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
 	disableDepthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	if (FAILED(device->CreateDepthStencilState(&disableDepthDesc, &DisableDepthDSS)))
	{
		LogMessage("Create 'DisableDepthDSS' depth stencil state failed!");
		return false;
	}


	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS)))
	{
		LogMessage("Create 'AlphaToCoverageBS' depth stencil state failed!");
		return false;
	}

	//
	// NoRenderTargetWritesBS
	//

	D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;

	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	if (FAILED(device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS)))
	{
		LogMessage("Create 'NoRenderTargetWritesBS' depth stencil state failed!");
		return false;
	}

	return true;
}

void RenderStates::ReleaseAll()
{
	SAFE_RELEASE(WireFrameRS);
	SAFE_RELEASE(NoCullRS);
	SAFE_RELEASE(CounterClockFrontRS);
	SAFE_RELEASE(TransparentBS);
	SAFE_RELEASE(NoColorWriteBS);
	SAFE_RELEASE(AlphaToCoverageBS);
	SAFE_RELEASE(NoRenderTargetWritesBS);
	SAFE_RELEASE(MarkMirrorDSS);
	SAFE_RELEASE(DrawReflectionDSS);
	SAFE_RELEASE(NoDoubleBlendingDSS);
	SAFE_RELEASE(DisableDepthDSS);
}