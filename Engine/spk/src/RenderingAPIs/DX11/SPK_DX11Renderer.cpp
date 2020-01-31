//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2009 - foulon matthieu - stardeath@wanadoo.fr					//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////


#include "RenderingAPIs/DX11/SPK_DX11Renderer.h"

namespace SPK
{
namespace DX11
{
	bool DX11Renderer::statesSaved = false;

	DX11Renderer::DX11Renderer() :
		Renderer(),
		blendingEnabled(false),
		srcBlending(D3D11_BLEND_SRC_ALPHA),
		destBlending(D3D11_BLEND_INV_SRC_ALPHA)
		//textureBlending(D3DTOP_MODULATE)
	{}

	DX11Renderer::~DX11Renderer() {DX11Info::DX9ReleaseRenderer(this);}

	void DX11Renderer::setBlending(BlendingMode blendMode)
	{
		switch(blendMode)
		{
		case BLENDING_NONE :
			srcBlending = D3D11_BLEND_ONE;
			destBlending = D3D11_BLEND_ZERO;
			blendingEnabled = false;
			break;

		case BLENDING_ADD :
			srcBlending = D3D11_BLEND_SRC_ALPHA;
			destBlending = D3D11_BLEND_ONE;
			blendingEnabled = true;
			break;

		case BLENDING_ALPHA :
			srcBlending = D3D11_BLEND_SRC_ALPHA;
			destBlending = D3D11_BLEND_INV_SRC_ALPHA;
			blendingEnabled = true;
			break;
		}
	}
}}
