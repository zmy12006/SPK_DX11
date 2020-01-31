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


#ifndef H_SPK_DX11BUFFERHANDLER
#define H_SPK_DX11BUFFERHANDLER

#include "RenderingAPIs/DX11/SPK_DX11_DEF.h"
#include "RenderingAPIs/DX11/SPK_DX11Info.h"


namespace SPK
{
namespace DX11
{
	enum {
		DX11_NONE = 0, // invalid
		DX11_VERTEX_BUFFER_KEY = 1 << 0,
		DX11_COLOR_BUFFER_KEY = 1 << 1,
		DX11_INDEX_BUFFER_KEY = 1 << 2,
		DX11_TEXTURE_BUFFER_KEY = 1 << 3
	};

	class SPK_DX11_PREFIX DX9BufferHandler
	{
	public :

		virtual ~DX9BufferHandler() {}

		virtual bool DX9CreateBuffers(const Group& group) {return true;};

		virtual bool DX9DestroyBuffers(const Group& group) {return true;};

	protected :

		// The constructor is private so that the class is not instanciable
		DX9BufferHandler() {}

		bool DX9PrepareBuffers(const Group& group);

		bool DX9Bind(const Group& group, int key, void** to);

		bool DX9Release(const Group& group, int key);

		bool DX9Create();

		virtual bool DX9CheckBuffers(const Group& group);

		std::map<std::pair<const Group *, int>, ID3D11Buffer*> DX9Buffers;//vb ib buffer
		std::map<std::pair<const Group *, int>, ID3D11Buffer*>::iterator DX9BuffersIt;
		std::pair<const Group *, int> DX9BuffersKey;

	};

	inline bool DX9BufferHandler::DX9CheckBuffers(const Group& group)
	{
		return true;
	}
}}

#endif
