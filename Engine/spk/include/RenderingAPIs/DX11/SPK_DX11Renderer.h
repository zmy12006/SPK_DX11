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


#ifndef H_SPK_DX11RENDERER
#define H_SPK_DX11RENDERER

#include "RenderingAPIs/DX11/SPK_DX11_DEF.h"
#include "RenderingAPIs/DX11/SPK_DX11Info.h"
#include "Core/SPK_Renderer.h"
#include "RenderingAPIs/DX11/SPK_DX11BufferHandler.h"
#include "RenderingAPIs/DX11/SPK_DX11RenderStates.h"

namespace SPK
{
namespace DX11
{
	class SPK_DX11_PREFIX DX11Renderer : public Renderer, public DX11BufferHandler
	{
	public :

		/////////////////
		// Constructor //
		/////////////////

		/** @brief Constructor of DX11Renderer */
		DX11Renderer();

		////////////////
		// Destructor //
		////////////////

		/** @brief Destructor of DX11Renderer */
		virtual ~DX11Renderer();

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Enables or disables the blending of this DX11Renderer
		* @param blendingEnabled true to enable the blending, false to disable it
		*/
		virtual void enableBlending(bool blendingEnabled);

		/**
		* @brief Sets the blending functions of this DX11Renderer
		*
		* the blending functions are one of the DX blending functions.
		*
		* @param src : the source blending function of this DX11Renderer
		* @param dest : the destination blending function of this DX11Renderer
		*/
		void setBlendingFunctions(int src,int dest);
		virtual void setBlending(BlendingMode blendMode);

		/**
		* @brief Sets the texture blending function of this DX11Renderer
		*
		* the texture blending function is one of the DX texture blending functions.
		*
		* @param textureBlending : the texture blending function of this DX11Renderer
		*/
		void setTextureBlending(int textureBlending);

		/////////////
		// Getters //
		/////////////

		/**
		* @brief Tells whether blending is enabled for this DX11Renderer
		* @return true if blending is enabled, false if it is disabled
		*/
		bool isBlendingEnabled() const;

		/**
		* @brief Gets the source blending function of this DX11Renderer
		* @return the source blending function of this DX11Renderer
		*/
		int getSrcBlendingFunction() const;

		/**
		* @brief Gets the destination blending function of this DX11Renderer
		* @return the source destination function of this DX11Renderer
		*/
		int getDestBlendingFunction() const;

		/**
		* @brief Gets the texture blending function of this DX11Renderer
		* @return the texture blending function of this DX11Renderer
		*/
		int getTextureBlending() const;


		/*virtual */bool DX11DestroyAllBuffers();

	protected :

		/** @brief Inits the blending of this DX11Renderer */
		void initBlending() const;

		void initRenderingHints() const;

		//std::map<std::pair<const Group *, int>, ID3D11Buffer *> DX11Buffers;

	private :

		static bool statesSaved;

		bool blendingEnabled;
		int srcBlending;
		int destBlending;

		int textureBlending;
	};


	inline void DX11Renderer::enableBlending(bool blendingEnabled)
	{
		this->blendingEnabled = blendingEnabled;
	}

	inline void DX11Renderer::setBlendingFunctions(int src,int dest)
	{
		srcBlending = src;
		destBlending = dest;
	}

	inline void DX11Renderer::setTextureBlending(int textureBlending)
	{
		this->textureBlending = textureBlending;
	}

	inline bool DX11Renderer::isBlendingEnabled() const
	{
		return blendingEnabled;
	}

	inline int DX11Renderer::getSrcBlendingFunction() const
	{
		return srcBlending;
	}

	inline int DX11Renderer::getDestBlendingFunction() const
	{
		return destBlending;
	}

	inline int DX11Renderer::getTextureBlending() const
	{
		return textureBlending;
	}

	inline void DX11Renderer::initBlending() const
	{
		if (blendingEnabled)
		{
			//DX11Info::getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			//DX11Info::getDevice()->SetRenderState(D3DRS_SRCBLEND, srcBlending);
			//DX11Info::getDevice()->SetRenderState(D3DRS_DESTBLEND, destBlending);
			//DX11Info::getContext()->(RenderStates::GetInstance()->TransparentBS);

			float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
			DX11Info::getContext()->OMSetBlendState(RenderStates::GetInstance()->TransparentBS, blendFactor, 0xffffffff);
		}
		else
		{
			//DX11Info::getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			DX11Info::getContext()->OMSetBlendState(0, 0, 0xffffffff);
		}
			
	}

	inline void DX11Renderer::initRenderingHints() const
	{
		// alpha test
		//if (isRenderingHintEnabled(ALPHA_TEST))
		//	DX11Info::getDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		//else
		//	DX11Info::getDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, false);

		// depth test//***
		//if (isRenderingHintEnabled(DEPTH_TEST))
		//	DX11Info::getDevice()->SetRenderState(D3DRS_ZENABLE, true);
		//else
		//	DX11Info::getDevice()->SetRenderState(D3DRS_ZENABLE, false);

		// depth write
		//DX11Info::getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, isRenderingHintEnabled(DEPTH_WRITE));
		if (isRenderingHintEnabled(DEPTH_WRITE) == false)
		{
			DX11Info::getContext()->OMSetDepthStencilState(RenderStates::GetInstance()->DisableDepthDSS, 1);
		}
		else
		{
			DX11Info::getContext()->OMSetDepthStencilState(0, 0);
		}
	}

	inline bool DX11Renderer::DX11DestroyAllBuffers()
	{
		std::map<std::pair<const Group *, int>, ID3D11Buffer *>::iterator it = DX11Buffers.begin();
		while( it != DX11Buffers.end() )
		{
			SAFE_RELEASE( it->second );
			it++;
		}
		DX11Buffers.clear();
		return true;
	}
}}

#endif
