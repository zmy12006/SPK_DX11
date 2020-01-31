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


#include "RenderingAPIs/DX11/SPK_DX11Info.h"
#include "RenderingAPIs/DX11/SPK_DX11Renderer.h"

namespace SPK
{
namespace DX11
{
	ID3D11Device* DX11Info::device = NULL;
	ID3D11DeviceContext* DX11Info::context = NULL;

	std::vector<DX11Renderer *> DX11Info::renderers = std::vector<DX11Renderer *>();

	void DX11Info::DX9DestroyAllBuffers()
	{
		for(size_t i = 0; i < renderers.size(); i++)
		{
			DX11Info::renderers[i]->DX9DestroyAllBuffers();
		}
	}
}}
