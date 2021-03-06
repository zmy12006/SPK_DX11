//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2009 - Julien Fryer - julienfryer@gmail.com				//
//                           matthieu foulon - stardeath@wanadoo.fr				//
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


#ifndef H_SPK_DX11QUADRENDERER
#define H_SPK_DX11QUADRENDERER

//#include "RenderingAPIs/DX11/Math/Misc.h"
//#include "RenderingAPIs/DX11/Math/SimpleMath.h"

#include "RenderingAPIs/DX11/SPK_DX11Renderer.h"
#include "Extensions/Renderers/SPK_QuadRendererInterface.h"
#include "Extensions/Renderers/SPK_Oriented3DRendererInterface.h"
#include "Core/SPK_Vector3D.h"
#include "Core/SPK_Particle.h"
#include "Core/SPK_Model.h"

using namespace DirectX;

namespace SPK
{
namespace DX11
{
	struct SPKVertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	//// déclaration du vertex descriptor sans coordonnées de texture
	//const D3DVERTEXELEMENT9 QuadVertexDecl[3] =
	//{
	//	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	//	{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	//	D3DDECL_END()
	//};

	//// déclaration du vertex descriptor avec coordonnées de textures 2d
	//const D3DVERTEXELEMENT9 QuadVertexDecl2D[4] =
	//{
	//	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	//	{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	//	{2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	//	D3DDECL_END()
	//};

	//// déclaration du vertex descriptor avec coordonnées de textures 3d
	//const D3DVERTEXELEMENT9 QuadVertexDecl3D[4] =
	//{
	//	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	//	{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	//	{2, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	//	D3DDECL_END()
	//};

	class DX11RenderShader
	{
	public:
		virtual void SPKRender(int indexCount, DirectX::XMFLOAT4X4  worldMatrix, DirectX::XMFLOAT4X4  viewMatrix,
			DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture) {}
	};

	class SPK_DX11_PREFIX DX11QuadRenderer : public DX11Renderer, public QuadRendererInterface, public Oriented3DRendererInterface
	{
		SPK_IMPLEMENT_REGISTERABLE(DX11QuadRenderer)

	public :

		//////////////////
		// Constructors //
		//////////////////

		DX11QuadRenderer(float scaleX = 1.0f, float scaleY = 1.0f);

		virtual ~DX11QuadRenderer();

		static DX11QuadRenderer* create(float scaleX = 1.0f,float scaleY = 1.0f);

		/////////////
		// Setters //
		/////////////

		virtual bool setTexturingMode(TexturingMode mode);

		void setTexture(ID3D11ShaderResourceView* textureIndex);


		/////////////
		// Getters //
		/////////////

		ID3D11ShaderResourceView* getTexture() const;

		///////////////
		// Interface //
		///////////////

		virtual void createBuffers(const Group& group);
		virtual void destroyBuffers(const Group& group);

		virtual bool DX11CreateBuffers(const Group& group);
		virtual bool DX11DestroyBuffers(const Group& group);

		virtual void render(const Group& group);
	public:
		void SetRender(DX11RenderShader* rs) { mRenderShader = rs; }
	protected:

		virtual bool checkBuffers(const Group& group);

		virtual bool DX11CheckBuffers(const Group& group);

	private :

		mutable float modelView[16];
		mutable float invModelView[16];

		DX11RenderShader*	mRenderShader = NULL;

		ID3D11ShaderResourceView* textureIndex;

		// vertex buffers and iterators
		static SPKVertexType* vertexBuffer;
		static SPKVertexType* vertexIterator;
		//static DWORD* colorBuffer;
		//static DWORD* colorIterator;
		//static float* textureBuffer;
		//static float* textureIterator;
		static short* indexBuffer;
		static short* indexIterator;

		//static LPDIRECT3DVERTEXBUFFER9 DX11VertexBuffer;
		//static LPDIRECT3DVERTEXBUFFER9 DX11ColorBuffer;
		//static LPDIRECT3DVERTEXBUFFER9 DX11TextureBuffer;
		//static LPDIRECT3DINDEXBUFFER9 DX11IndexBuffer;

		static ID3D11Buffer *DX11VertexBuffer; 
		static ID3D11Buffer *DX11IndexBuffer;

		static ID3D11InputLayout* InputLayout;

		static short offsetIndex;

		//static SPKVertexType* m_vertices;

		// vertex declaration
		//static LPDIRECT3DVERTEXDECLARATION9 pVertexDecl;
		//static LPDIRECT3DVERTEXDECLARATION9 pVertexDecl2D;
		//static LPDIRECT3DVERTEXDECLARATION9 pVertexDecl3D;

		// buffers names
		static const std::string VERTEX_BUFFER_NAME;
		//static const std::string COLOR_BUFFER_NAME;
		static const std::string TEXTURE_BUFFER_NAME;
		static const std::string INDEX_BUFFER_NAME;

		
		float* createTextureBuffer(const Group& group) const;


		void DX11CallColorAndVertex(const Particle& particle) const;	// DX11 calls for color and position
		void DX11CallTexture2DAtlas(const Particle& particle) const;	// DX11 calls for 2D atlastexturing 
		void DX11CallTexture3D(const Particle& particle) const;		// DX11 calls for 3D texturing

		static void (DX11QuadRenderer::*renderParticle)(const Particle&)  const;	// pointer to the right render method

		void render2D(const Particle& particle) const;			// Rendering for particles with texture 2D or no texture
		void render2DRot(const Particle& particle) const;		// Rendering for particles with texture 2D or no texture and rotation
		void render3D(const Particle& particle) const;			// Rendering for particles with texture 3D
		void render3DRot(const Particle& particle) const;		// Rendering for particles with texture 3D and rotation
		void render2DAtlas(const Particle& particle) const;		// Rendering for particles with texture 2D atlas
		void render2DAtlasRot(const Particle& particle) const;	// Rendering for particles with texture 2D atlas and rotation
	};


	inline DX11QuadRenderer* DX11QuadRenderer::create(float scaleX,float scaleY)
	{
		DX11QuadRenderer* obj = new DX11QuadRenderer(scaleX,scaleY);
		registerObject(obj);
		DX11Info::DX11RegisterRenderer(obj);
		return obj;
	}

	inline void DX11QuadRenderer::setTexture(ID3D11ShaderResourceView* textureIndex)
	{
		this->textureIndex = textureIndex;
	}

	inline ID3D11ShaderResourceView* DX11QuadRenderer::getTexture() const
	{
		return textureIndex;
	}

	inline void DX11QuadRenderer::DX11CallColorAndVertex(const Particle& particle) const
	{
		computeAtlasCoordinates(particle);

		float x = particle.position().x;
		float y = particle.position().y;
		float z = particle.position().z;

		float psize = 0.2f;

		// top left vertex
		(vertexIterator)->texture.x = textureAtlasU0();
		(vertexIterator)->texture.y = textureAtlasV0();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x + quadSide().x + quadUp().x;//x - psize;//
		(vertexIterator)->position.y =  y + quadSide().y + quadUp().y;//y + psize;//
		(vertexIterator++)->position.z = z + quadSide().z + quadUp().z;

		// top right vertex
		(vertexIterator)->texture.x = textureAtlasU1();
		(vertexIterator)->texture.y = textureAtlasV0();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x - quadSide().x + quadUp().x;//x + psize;//
		(vertexIterator)->position.y =  y - quadSide().y + quadUp().y;//y + psize;//
		(vertexIterator++)->position.z = z - quadSide().z + quadUp().z;

		// bottom right vertex
		(vertexIterator)->texture.x = textureAtlasU1();
		(vertexIterator)->texture.y = textureAtlasV1();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x - quadSide().x - quadUp().x;//x + psize;//
		(vertexIterator)->position.y =  y - quadSide().y - quadUp().y;//y - psize;//
		(vertexIterator++)->position.z = z - quadSide().z - quadUp().z;

		//---------------------------------------------------------------------------

		// top left vertex
		(vertexIterator)->texture.x = textureAtlasU0();
		(vertexIterator)->texture.y = textureAtlasV0();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x + quadSide().x + quadUp().x;//x - psize;//
		(vertexIterator)->position.y =  y + quadSide().y + quadUp().y;//y + psize;//
		(vertexIterator++)->position.z = z + quadSide().z + quadUp().z;

		// bottom right vertex
		(vertexIterator)->texture.x = textureAtlasU1();
		(vertexIterator)->texture.y = textureAtlasV1();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x - quadSide().x - quadUp().x;//x + psize;//
		(vertexIterator)->position.y =  y - quadSide().y - quadUp().y;//y - psize;//
		(vertexIterator++)->position.z = z - quadSide().z - quadUp().z;

		// bottom left vertex
		(vertexIterator)->texture.x = textureAtlasU0();
		(vertexIterator)->texture.y = textureAtlasV1();
		(vertexIterator)->color = XMFLOAT4(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		(vertexIterator)->position.x =  x + quadSide().x - quadUp().x;//x - psize;//
		(vertexIterator)->position.y =  y + quadSide().y - quadUp().y;//y - psize;//
		(vertexIterator++)->position.z = z + quadSide().z - quadUp().z;

		// TODO : éviter la duplication de l'information couleur
		//DWORD color = D3DCOLOR_COLORVALUE(particle.getR(), particle.getG(), particle.getB(), particle.getParamCurrentValue(PARAM_ALPHA));
		//*(colorIterator++) = color;
		//*(colorIterator++) = color;
		//*(colorIterator++) = color;
		//*(colorIterator++) = color;


	}

	inline void DX11QuadRenderer::DX11CallTexture2DAtlas(const Particle& particle) const
	{
		//computeAtlasCoordinates(particle);

		//*(textureIterator++) = textureAtlasU0();
		//*(textureIterator++) = textureAtlasV0();

		//*(textureIterator++) = textureAtlasU1();
		//*(textureIterator++) = textureAtlasV0();

		//*(textureIterator++) = textureAtlasU1();
		//*(textureIterator++) = textureAtlasV1();

		//*(textureIterator++) = textureAtlasU0();
		//*(textureIterator++) = textureAtlasV1();	
	}

	inline void DX11QuadRenderer::DX11CallTexture3D(const Particle& particle) const
	{
		float textureIndex = particle.getParamCurrentValue(PARAM_TEXTURE_INDEX);

		//*(textureIterator + 2) = textureIndex;
		//*(textureIterator + 5) = textureIndex;
		//*(textureIterator + 8) = textureIndex;
		//*(textureIterator + 11) = textureIndex;
		//textureIterator += 12;
	}
}}

#endif
