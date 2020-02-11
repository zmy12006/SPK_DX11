//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2009 - Julien Fryer - julienfryer@gmail.com				//
//                           foulon matthieu - stardeath@wanadoo.fr				//
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


#include "RenderingAPIs/DX11/SPK_DX11BufferHandler.h"

namespace SPK
{
namespace DX11
{
	bool DX11BufferHandler::DX11PrepareBuffers(const Group& group)
	{
		if(!DX11CheckBuffers(group))
		{
			//if (isBuffersCreationEnabled())
			{
				DX11DestroyBuffers(group);
				if( !DX11CreateBuffers(group) )
				{
//					std::cout << "DX11CreateBuffers echec" << std::endl;
					return false;
				}
				return true;
			}
			//return false;
		}
		return true;
	}

	bool DX11BufferHandler::DX11Bind(const Group& group, int key, void** to)
	{
		DX11BuffersKey = std::pair<const Group *, int>(&group, key);
		DX11BuffersIt = DX11Buffers.find(DX11BuffersKey);
		if( DX11BuffersIt == DX11Buffers.end() ) return false;

		switch( key )
		{
			case DX11_VERTEX_BUFFER_KEY:
			case DX11_COLOR_BUFFER_KEY:
			case DX11_TEXTURE_BUFFER_KEY:
				*to = reinterpret_cast<ID3D11Buffer*>(DX11BuffersIt->second);
				break;
			case DX11_INDEX_BUFFER_KEY:
				*to = reinterpret_cast<ID3D11Buffer*>(DX11BuffersIt->second);
				break;
			default:
				return false;
		}
		return true;
	}

	bool DX11BufferHandler::DX11Release(const Group& group, int key)
	{
		DX11BuffersKey = std::pair<const Group *, int>(&group, key);
		DX11BuffersIt = DX11Buffers.find(DX11BuffersKey);
		if( DX11BuffersIt != DX11Buffers.end() )
		{
			SAFE_RELEASE( DX11BuffersIt->second );
			DX11Buffers.erase(DX11BuffersIt);
		}
		return true;
	}
}}
