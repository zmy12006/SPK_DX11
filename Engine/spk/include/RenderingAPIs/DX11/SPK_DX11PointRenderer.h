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


#ifndef H_SPK_DX11POINTRENDERER
#define H_SPK_DX11POINTRENDERER

#include "SPK_DX11Renderer.h"
#include "Extensions/Renderers/SPK_PointRendererInterface.h"
#include "RenderingAPIs/DX11/Math/SimpleMath.h"

using namespace DirectX;

namespace SPK
{
namespace DX11
{
	struct PointVertex
	{
		SimpleMath::Vector3 position;
		SimpleMath::Color color;
	};

	class SPK_DX11_PREFIX DX9PointRenderer : public DX11Renderer
	{
		
		SPK_IMPLEMENT_REGISTERABLE(DX9PointRenderer)

	public :

		//////////////////
		// Constructors //
		//////////////////

		/** 
		* @brief Constructor of DX9PointRenderer
		* @param size : the size of the points
		*/
		DX9PointRenderer(float size = 1.0f);

		static DX9PointRenderer* create(float size = 1.0f);

		/////////////
		// Setters //
		/////////////

		bool setType(PointType type);

		void setSize(float size);

		void setTexture(ID3D11ShaderResourceView* textureIndex);

		bool enableWorldSize(bool worldSizeEnabled);

		static void setPixelPerUnit(float fovy,int screenHeight);

		void enablePointParameter(float size,bool distance);

		/////////////
		// Getters //
		/////////////

		PointType getType() const;

		float getSize() const;

		ID3D11ShaderResourceView* getTexture() const;

		bool isWorldSizeEnabled() const;

		///////////////
		// Interface //
		///////////////

		virtual void createBuffers(const Group& group);
		virtual void destroyBuffers(const Group& group);

		virtual bool DX9CreateBuffers(const Group& group);
		virtual bool DX9DestroyBuffers(const Group& group);

		virtual void render(const Group& group);

	private :

		virtual bool checkBuffers(const Group& group);

		virtual bool DX9CheckBuffers(const Group& group);

		PointType type;
		float size;
		ID3D11ShaderResourceView* textureIndex;
		bool worldSize;

		// buffers names
		static const std::string VERTEX_BUFFER_NAME;

		// vertex buffer
		static PointVertex* vertexBuffer;
		static PointVertex* vertexIterator;

		static ID3D11Buffer* DX9VertexBuffer;

		//////////////////////
		// Point Parameters //
		//////////////////////

		static const float POINT_SIZE_CURRENT;
		static const float POINT_SIZE_MIN;
		static const float POINT_SIZE_MAX;

		static float pixelPerUnit;

		static const float QUADRATIC_SCREEN[3];
	};

	inline DX9PointRenderer* DX9PointRenderer::create(float size)
	{
		DX9PointRenderer* obj = new DX9PointRenderer(size);
		registerObject(obj);
		DX11Info::DX9RegisterRenderer(obj);
		return obj;
	}

	inline void DX9PointRenderer::setSize(float size)
	{
		this->size = size;
	}

	inline void DX9PointRenderer::setTexture(ID3D11ShaderResourceView* textureIndex)
	{
		this->textureIndex = textureIndex;
	}

	inline PointType DX9PointRenderer::getType() const
	{
		return type;
	}

	inline float DX9PointRenderer::getSize() const
	{
		return size;
	}

	inline ID3D11ShaderResourceView* DX9PointRenderer::getTexture() const
	{
		return textureIndex;
	}

	inline bool DX9PointRenderer::isWorldSizeEnabled() const
	{
		return worldSize;
	}
}}

#endif
