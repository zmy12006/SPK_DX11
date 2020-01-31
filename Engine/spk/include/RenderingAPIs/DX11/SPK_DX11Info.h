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

#ifndef H_SPK_DX11INFO
#define H_SPK_DX11INFO

#include "RenderingAPIs/DX11/SPK_DX11_DEF.h"
#include "Core/SPK_Group.h"

#include <vector>
#include <map>

namespace SPK
{
namespace DX11
{
	class DX11Renderer;

	class SPK_DX11_PREFIX DX11Info
	{
	private:
		//static LPDIRECT3DDEVICE9 device;

		static ID3D11Device* device;
		static ID3D11DeviceContext* context;

		static std::vector<DX11Renderer *> renderers;

	public:
		static ID3D11Device* getDevice();
		//static D3DPOOL getPool();
		static ID3D11DeviceContext* getContext(){ return context; }

		static void setDevice(ID3D11Device* device);
		//static void setPool(D3DPOOL pool);
		static void setContext(ID3D11DeviceContext* pContext){ context = pContext; }

		static void DX9RegisterRenderer(DX11Renderer *renderer);
		static void DX9ReleaseRenderer(DX11Renderer *renderer);

		static void DX9DestroyAllBuffers();
	};

	inline void DX11Info::DX9RegisterRenderer(DX11Renderer *renderer)
	{
		renderers.push_back(renderer);
	}

	inline void DX11Info::DX9ReleaseRenderer(DX11Renderer *renderer)
	{
		for(std::vector<DX11Renderer *>::iterator it = renderers.begin(); it != renderers.end(); )
		{
			if (*it == renderer)
				it = renderers.erase(it);
			else
				++it;
		}
	}

	inline ID3D11Device* DX11Info::getDevice()
	{
		return DX11Info::device;
	}

	inline void DX11Info::setDevice(ID3D11Device* device)
	{
		DX11Info::device = device;
	}
}}

#endif
