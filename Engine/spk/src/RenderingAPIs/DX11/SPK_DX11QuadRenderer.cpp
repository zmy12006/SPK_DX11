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


#include "RenderingAPIs/DX11/SPK_DX11QuadRenderer.h"
#include "Core/SPK_Particle.h"
#include "Core/SPK_Group.h"
#include "Core/SPK_ArrayBuffer.h"

using namespace DirectX;

namespace SPK
{
namespace DX11
{
	const std::string DX11QuadRenderer::VERTEX_BUFFER_NAME("SPK_DX11QuadRenderer_Vertex");
	//const std::string DX11QuadRenderer::COLOR_BUFFER_NAME("SPK_DX11QuadRenderer_Color");
	const std::string DX11QuadRenderer::TEXTURE_BUFFER_NAME("SPK_DX11QuadRenderer_Texture");
	const std::string DX11QuadRenderer::INDEX_BUFFER_NAME("SPK_DX11QuadRenderer_Index");

	SPKVertexType* DX11QuadRenderer::vertexBuffer = NULL;
	SPKVertexType* DX11QuadRenderer::vertexIterator = NULL;
	//DWORD* DX11QuadRenderer::colorBuffer = NULL;
	//DWORD* DX11QuadRenderer::colorIterator = NULL;
	//float* DX11QuadRenderer::textureBuffer = NULL;
	//float* DX11QuadRenderer::textureIterator = NULL;
	short* DX11QuadRenderer::indexBuffer = NULL;
	short* DX11QuadRenderer::indexIterator = NULL;

	//LPDIRECT3DVERTEXBUFFER9 DX11QuadRenderer::DX11VertexBuffer = NULL;
	//LPDIRECT3DVERTEXBUFFER9 DX11QuadRenderer::DX11ColorBuffer = NULL;
	//LPDIRECT3DVERTEXBUFFER9 DX11QuadRenderer::DX11TextureBuffer = NULL;
	//LPDIRECT3DINDEXBUFFER9 DX11QuadRenderer::DX11IndexBuffer = NULL;

	ID3D11Buffer *DX11QuadRenderer::DX11VertexBuffer = NULL;
	ID3D11Buffer *DX11QuadRenderer::DX11IndexBuffer = NULL;

	short DX11QuadRenderer::offsetIndex = 0;

	//LPDIRECT3DVERTEXDECLARATION9 DX11QuadRenderer::pVertexDecl = NULL;
	//LPDIRECT3DVERTEXDECLARATION9 DX11QuadRenderer::pVertexDecl2D = NULL;
	//LPDIRECT3DVERTEXDECLARATION9 DX11QuadRenderer::pVertexDecl3D = NULL;

	void (DX11QuadRenderer::*DX11QuadRenderer::renderParticle)(const Particle&) const = NULL;

	DX11QuadRenderer::DX11QuadRenderer(float scaleX,float scaleY) :
		DX11Renderer(),
		QuadRendererInterface(scaleX,scaleY),
		Oriented3DRendererInterface(),
		textureIndex(0)
	{}

	DX11QuadRenderer::~DX11QuadRenderer()
	{
		//SAFE_RELEASE( pVertexDecl );
		//SAFE_RELEASE( pVertexDecl2D );
		//SAFE_RELEASE( pVertexDecl3D );
	}

	bool DX11QuadRenderer::checkBuffers(const Group& group)
	{
		ArrayBuffer<SPKVertexType>* pvbBuffer = NULL;
		ArrayBuffer<DWORD>* dwColorBuffer = NULL;
		ArrayBuffer<short>* sIndexBuffer = NULL;

		if ((pvbBuffer = dynamic_cast<ArrayBuffer<SPKVertexType>*>(group.getBuffer(VERTEX_BUFFER_NAME))) == NULL)
			return false;

		//if ((dwColorBuffer = dynamic_cast<ArrayBuffer<DWORD>*>(group.getBuffer(COLOR_BUFFER_NAME))) == NULL)
		//	return false;

		if ((sIndexBuffer = dynamic_cast<ArrayBuffer<short>*>(group.getBuffer(INDEX_BUFFER_NAME))) == NULL)
			return false;

		//if( texturingMode != TEXTURE_NONE )
		//{
		//	FloatBuffer* fTextureBuffer;

		//	if ((fTextureBuffer = dynamic_cast<FloatBuffer*>(group.getBuffer(TEXTURE_BUFFER_NAME,texturingMode))) == NULL)
		//		textureBuffer = createTextureBuffer(group);

		//	textureIterator = textureBuffer = fTextureBuffer->getData();
		//}

		vertexIterator = vertexBuffer = pvbBuffer->getData();
		//colorIterator = colorBuffer = dwColorBuffer->getData();
		indexIterator = indexBuffer = sIndexBuffer->getData();

		return true;
	}

	void DX11QuadRenderer::createBuffers(const Group& group)
	{
		ArrayBuffer<SPKVertexType>* vbVertexBuffer = dynamic_cast<ArrayBuffer<SPKVertexType>*>(group.createBuffer(VERTEX_BUFFER_NAME, ArrayBufferCreator<SPKVertexType>(6), 0, false));
		//ArrayBuffer<DWORD>* vbColorBuffer = dynamic_cast<ArrayBuffer<DWORD>*>(group.createBuffer(COLOR_BUFFER_NAME, ArrayBufferCreator<DWORD>(4),0,false));
		ArrayBuffer<short>* ibIndexBuffer  = dynamic_cast<ArrayBuffer<short>*>(group.createBuffer(INDEX_BUFFER_NAME, ArrayBufferCreator<short>(6),0,false));
		vertexIterator = vertexBuffer = vbVertexBuffer->getData();
		//colorIterator = colorBuffer = vbColorBuffer->getData();
		indexIterator = indexBuffer = ibIndexBuffer->getData();

		//if( texturingMode != TEXTURE_NONE )
		//	textureIterator = textureBuffer = createTextureBuffer(group);

		offsetIndex = 0;

		// initialisation de l'index buffer
		for(size_t i = 0; i < group.getParticles().getNbReserved(); i++)
		{
			*(indexIterator++) = 0 + offsetIndex;
			*(indexIterator++) = 1 + offsetIndex;
			*(indexIterator++) = 2 + offsetIndex;
			*(indexIterator++) = 3 + offsetIndex;
			*(indexIterator++) = 4 + offsetIndex;
			*(indexIterator++) = 5 + offsetIndex;

			offsetIndex += 6;
		}
		offsetIndex = 0;
	}

	void DX11QuadRenderer::destroyBuffers(const Group& group)
	{
		group.destroyBuffer(VERTEX_BUFFER_NAME);
		//group.destroyBuffer(COLOR_BUFFER_NAME);
		group.destroyBuffer(TEXTURE_BUFFER_NAME);
		group.destroyBuffer(INDEX_BUFFER_NAME);
		offsetIndex = 0;
	}

	float* DX11QuadRenderer::createTextureBuffer(const Group& group) const
	{
		FloatBuffer* fbuffer = NULL;

		switch(texturingMode)
		{
			case TEXTURE_2D :
				fbuffer = dynamic_cast<FloatBuffer*>(group.createBuffer(TEXTURE_BUFFER_NAME,FloatBufferCreator(8),TEXTURE_2D,false));
				if (!group.getModel()->isEnabled(PARAM_TEXTURE_INDEX))
				{
					float t[8] = {0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f};
					for (size_t i = 0; i < group.getParticles().getNbReserved() << 3; ++i)
						fbuffer->getData()[i] = t[i & 7];
				}
				break;

			case TEXTURE_3D :
				fbuffer = dynamic_cast<FloatBuffer*>(group.createBuffer(TEXTURE_BUFFER_NAME,FloatBufferCreator(12),TEXTURE_3D,false));
				float t[12] =  {0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f};
				for (size_t i = 0; i < group.getParticles().getNbReserved() * 12; ++i)
					fbuffer->getData()[i] = t[i % 12];
				break;
		}

		return fbuffer->getData();
	}

	bool DX11QuadRenderer::setTexturingMode(TexturingMode mode)
	{
		texturingMode = mode;
		return true;
	}

	void DX11QuadRenderer::render(const Group& group)
	{
		HRESULT hr = 0;
		int nb_part = group.getNbParticles();

		if (!DX11PrepareBuffers(group))
			return;

		if( !prepareBuffers(group) )
			return;

		//D3DXMATRIX view,world,modelView;
		//DX11Info::getDevice()->GetTransform(D3DTS_VIEW, &view);
		//DX11Info::getDevice()->GetTransform(D3DTS_WORLD, &world);
		//modelView = world * view;
		//D3DXMatrixInverse((D3DXMATRIX*)&invModelView, NULL, &modelView);

		//***initBlending();

		//DX11Info::getDevice()->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		//DX11Info::getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//switch(texturingMode)
		//{
		//	case TEXTURE_2D :
		//		DX11Info::getDevice()->SetTexture(0, textureIndex);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLOROP, getTextureBlending());
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		//		DX11Info::getDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//		DX11Info::getDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		//		if (!group.getModel()->isEnabled(PARAM_TEXTURE_INDEX))
		//		{
		//			if (!group.getModel()->isEnabled(PARAM_ANGLE))
		//				renderParticle = &DX11QuadRenderer::render2D;
		//			else
		//				renderParticle = &DX11QuadRenderer::render2DRot;
		//		}
		//		else
		//		{
		//			if (!group.getModel()->isEnabled(PARAM_ANGLE))
		//				renderParticle = &DX11QuadRenderer::render2DAtlas;
		//			else
		//				renderParticle = &DX11QuadRenderer::render2DAtlasRot;
		//		}
		//		break;

		//	case TEXTURE_3D :
		//		DX11Info::getDevice()->SetTexture(0, textureIndex);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLOROP, getTextureBlending());
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		//		DX11Info::getDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		//		DX11Info::getDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//		DX11Info::getDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		//		if (!group.getModel()->isEnabled(PARAM_ANGLE))
		//			renderParticle = &DX11QuadRenderer::render3D;
		//		else
		//			renderParticle = &DX11QuadRenderer::render3DRot;
		//		break;

		//	case TEXTURE_NONE :
		//		DX11Info::getDevice()->SetRenderState(D3DRS_COLORVERTEX, true);
		//		if (!group.getModel()->isEnabled(PARAM_ANGLE))
		//			renderParticle = &DX11QuadRenderer::render2D;
		//		else
		//			renderParticle = &DX11QuadRenderer::render2DRot;
		//		break;
		//}

		if (!group.getModel()->isEnabled(PARAM_ANGLE))
			renderParticle = &DX11QuadRenderer::render2D;
		else
			renderParticle = &DX11QuadRenderer::render2DRot;

		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				invModelView[i * 4 + j] = DX11Info::MatWorldInv.m[i][j];
			}
		}

		bool globalOrientation = precomputeOrientation3D(
			group,
			Vector3D(invModelView[8],invModelView[9],invModelView[10]),
			Vector3D(invModelView[4],invModelView[5],invModelView[6]),
			Vector3D(invModelView[12],invModelView[13],invModelView[14])
		);

		if (globalOrientation)
		{
			computeGlobalOrientation3D();

			for (size_t i = 0; i < group.getNbParticles(); ++i)
				(this->*renderParticle)(group.getParticle(i));
		}
		else
		{
			for (size_t i = 0; i < group.getNbParticles(); ++i)
			{
				const Particle& particle = group.getParticle(i);
				computeSingleOrientation3D(particle);
				(this->*renderParticle)(particle);
			}
		}

		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			result = DX11Info::getContext()->Map(DX11VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return;
			}

			// Get a pointer to the data in the vertex buffer.
			SPKVertexType* verticesPtr = (SPKVertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertexBuffer, (sizeof(SPKVertexType)* 6 * group.getNbParticles()));

			// Unlock the vertex buffer.
			DX11Info::getContext()->Unmap(DX11VertexBuffer, 0);
		}

		//{
		//	HRESULT result;
		//	D3D11_MAPPED_SUBRESOURCE mappedResource;
		//	result = DX11Info::getContext()->Map(DX11IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//	if (FAILED(result))
		//	{
		//		return;
		//	}

		//	// Get a pointer to the data in the vertex buffer.
		//	short* verticesPtr = (short*)mappedResource.pData;

		//	// Copy the data into the vertex buffer.
		//	memcpy(verticesPtr, (void*)indexBuffer, (sizeof(short)* 6 * group.getNbParticles()));

		//	// Unlock the vertex buffer.
		//	DX11Info::getContext()->Unmap(DX11IndexBuffer, 0);
		//}

		{
			unsigned int stride;
			unsigned int offset;


			// Set vertex buffer stride and offset.
			stride = sizeof(SPKVertexType);
			offset = 0;

			// Set the vertex buffer to active in the input assembler so it can be rendered.
			DX11Info::getContext()->IASetVertexBuffers(0, 1, &DX11VertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			DX11Info::getContext()->IASetIndexBuffer(DX11IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer.
			DX11Info::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		
		//// bind buffers and draw
		//{
		//	LPDIRECT3DDEVICE9 device = DX11Info::getDevice();
		//	void *ptr;

		//	switch( texturingMode )
		//	{
		//		case TEXTURE_2D:
		//			{
		//				DX11TextureBuffer->Lock(0, 0, &ptr, 0);
		//				std::memcpy(ptr, textureBuffer, 4 * group.getNbParticles() * sizeof(D3DXVECTOR2));
		//				DX11TextureBuffer->Unlock();

		//				device->SetStreamSource(2, DX11TextureBuffer, 0, sizeof(D3DXVECTOR2));
		//				device->SetVertexDeclaration(pVertexDecl2D);
		//			}
		//			break;

		//		case TEXTURE_3D:
		//			{
		//				DX11TextureBuffer->Lock(0, 0, &ptr, 0);
		//				std::memcpy(ptr, textureBuffer, 4 * group.getNbParticles() * sizeof(D3DXVECTOR3));
		//				DX11TextureBuffer->Unlock();

		//				device->SetStreamSource(2, DX11TextureBuffer, 0, sizeof(D3DXVECTOR3));
		//				device->SetVertexDeclaration(pVertexDecl3D);
		//			}
		//			break;

		//		case TEXTURE_NONE:
		//			{
		//				device->SetVertexDeclaration(pVertexDecl);
		//			}
		//			break;
		//	}

		//	DX11VertexBuffer->Lock(0, 0, &ptr, 0);
		//	std::memcpy(ptr, vertexBuffer, 4 * group.getNbParticles() * sizeof(D3DXVECTOR3));
		//	DX11VertexBuffer->Unlock();
		//	device->SetStreamSource(0, DX11VertexBuffer, 0, sizeof(D3DXVECTOR3));

		//	DX11ColorBuffer->Lock(0, 0, &ptr, 0);
		//	std::memcpy(ptr, colorBuffer, 4 * group.getNbParticles() * sizeof(DWORD));
		//	DX11ColorBuffer->Unlock();
		//	device->SetStreamSource(1, DX11ColorBuffer, 0, sizeof(DWORD));

		//	DX11IndexBuffer->Lock(0, 0, &ptr, 0);
		//	std::memcpy(ptr, indexBuffer, 6 * group.getNbParticles() * sizeof(short));
		//	DX11IndexBuffer->Unlock();
		//	device->SetIndices(DX11IndexBuffer);

		//	this->offsetIndex = 0;
		//	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nb_part<<2, 0, nb_part<<1);
		//}
		//---------------------------------------------------------------------------
	}

	void DX11QuadRenderer::render2D(const Particle& particle) const
	{
		scaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
	}

	void DX11QuadRenderer::render2DRot(const Particle& particle) const
	{
		rotateAndScaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
	}

	void DX11QuadRenderer::render3D(const Particle& particle) const
	{
		scaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
		DX11CallTexture3D(particle);
	}

	void DX11QuadRenderer::render3DRot(const Particle& particle) const
	{
		rotateAndScaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
		DX11CallTexture3D(particle);
	}

	void DX11QuadRenderer::render2DAtlas(const Particle& particle) const
	{
		scaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
		DX11CallTexture2DAtlas(particle);
	}

	void DX11QuadRenderer::render2DAtlasRot(const Particle& particle) const
	{
		rotateAndScaleQuadVectors(particle,scaleX,scaleY);
		DX11CallColorAndVertex(particle);
		DX11CallTexture2DAtlas(particle);
	}

	bool DX11QuadRenderer::DX11CheckBuffers(const Group& group)
	{
		if( !DX11Bind(group, DX11_VERTEX_BUFFER_KEY, (void**)&DX11VertexBuffer) )
		{
			DX11VertexBuffer /*= DX11ColorBuffer = DX11TextureBuffer*/ = NULL;
			DX11IndexBuffer = NULL;
			return false;
		}
		//if( !DX11Bind(group, DX11_COLOR_BUFFER_KEY, (void**)&DX11ColorBuffer) )
		//{
		//	DX11VertexBuffer = DX11ColorBuffer = DX11TextureBuffer = NULL;
		//	DX11IndexBuffer = NULL;
		//	return false;
		//}
		if( !DX11Bind(group, DX11_INDEX_BUFFER_KEY, (void**)&DX11IndexBuffer) )
		{
			DX11VertexBuffer /*= DX11ColorBuffer = DX11TextureBuffer*/ = NULL;
			DX11IndexBuffer = NULL;
			return false;
		}
		//if( texturingMode != TEXTURE_NONE )
		//{
		//	if( !DX11Bind(group, DX11_TEXTURE_BUFFER_KEY, (void**)&DX11TextureBuffer) )
		//	{
		//		DX11VertexBuffer = DX11ColorBuffer = DX11TextureBuffer = NULL;
		//		DX11IndexBuffer = NULL;
		//		return false;
		//	}
		//}

		return true;
	}

	bool DX11QuadRenderer::DX11CreateBuffers(const Group& group)
	{
		std::cout << "DX11QuadRenderer::DX11CreateBuffers" << std::endl;

		if( DX11Info::getDevice() == NULL ) return false;

		SAFE_RELEASE(DX11VertexBuffer);
		SAFE_RELEASE(DX11IndexBuffer);

		SPKVertexType* m_vertices;

		unsigned long* indices;
		int i;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;


		// Set the maximum number of vertices in the vertex array.
		int vertexCount = group.getParticles().getNbReserved() * 6;

		// Set the maximum number of indices in the index array.
		int indexCount = vertexCount;

		// Create the vertex array for the particles that will be rendered.
		m_vertices = new SPKVertexType[vertexCount];
		if (!m_vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[indexCount];
		if (!indices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(m_vertices, 0, (sizeof(SPKVertexType)* vertexCount));

		// Initialize the index array.
		for (i = 0; i<indexCount; i++)
		{
			indices[i] = i;
		}

		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(SPKVertexType)* vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = m_vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now finally create the vertex buffer.
		result = DX11Info::getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &DX11VertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = DX11Info::getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &DX11IndexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the index array since it is no longer needed.
		delete[] indices;
		indices = 0;

		SAFE_DELETE_ARRAY(m_vertices);
		m_vertices = 0;

		std::pair<const Group *, int> key(&group, DX11_VERTEX_BUFFER_KEY);
		DX11Buffers[key] = DX11VertexBuffer;

		key = std::pair<const Group *, int>(&group, DX11_INDEX_BUFFER_KEY);
		DX11Buffers[key] = DX11IndexBuffer;

		return true;

		//if( DX11Buffers.size() == 0 )
		//{
		//	SAFE_RELEASE( pVertexDecl );
		//	SAFE_RELEASE( pVertexDecl2D );
		//	SAFE_RELEASE( pVertexDecl3D );
		//	DX11Info::getDevice()->CreateVertexDeclaration(QuadVertexDecl, &pVertexDecl);
		//	DX11Info::getDevice()->CreateVertexDeclaration(QuadVertexDecl2D, &pVertexDecl2D);
		//	DX11Info::getDevice()->CreateVertexDeclaration(QuadVertexDecl3D, &pVertexDecl3D);
		//}

		//LPDIRECT3DVERTEXBUFFER9 vb;

		//// vertex buffer
		//if( DX11Info::getDevice()->CreateVertexBuffer(group.getParticles().getNbReserved() * 4 * sizeof(D3DXVECTOR3), 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &vb, NULL) != S_OK ) return false;
		//std::pair<const Group *, int> key(&group, DX11_VERTEX_BUFFER_KEY);
		//DX11Buffers[key] = vb;
		//DX11VertexBuffer = vb;
		////-----------------------------------------------------------------------------------------------

		//// color buffer
		//if( DX11Info::getDevice()->CreateVertexBuffer(group.getParticles().getNbReserved() * 4 * sizeof(D3DCOLOR), 0, D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vb, NULL) != S_OK ) return false;
		//key = std::pair<const Group *, int>(&group, DX11_COLOR_BUFFER_KEY);
		//DX11Buffers[key] = vb;
		//DX11ColorBuffer = vb;
		////-----------------------------------------------------------------------------------------------

		//// index buffer
		//LPDIRECT3DINDEXBUFFER9 ib;

		//if( DX11Info::getDevice()->CreateIndexBuffer(group.getParticles().getNbReserved() * 6 * sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL) != S_OK ) return false;
		//key = std::pair<const Group *, int>(&group, DX11_INDEX_BUFFER_KEY);
		//DX11Buffers[key] = ib;
		//DX11IndexBuffer = ib;
		////-----------------------------------------------------------------------------------------------

		//// texture buffer
		//switch(texturingMode)
		//{
		//case TEXTURE_2D :
		//	if( DX11Info::getDevice()->CreateVertexBuffer(group.getParticles().getNbReserved() * 8 * sizeof(float), 0, D3DFVF_TEX1, D3DPOOL_DEFAULT, &vb, NULL) != S_OK ) return false;
		//	key = std::pair<const Group *, int>(&group, DX11_TEXTURE_BUFFER_KEY);
		//	DX11Buffers[key] = vb;
		//	DX11TextureBuffer = vb;
		//	break;
		//case TEXTURE_3D :
		//	if( DX11Info::getDevice()->CreateVertexBuffer(group.getParticles().getNbReserved() * 12 * sizeof(float), 0, D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE1(3), D3DPOOL_DEFAULT, &vb, NULL) != S_OK ) return false;
		//	key = std::pair<const Group *, int>(&group, DX11_TEXTURE_BUFFER_KEY);
		//	DX11Buffers[key] = vb;
		//	DX11TextureBuffer = vb;
		//	break;
		//}
		////-----------------------------------------------------------------------------------------------
		//return true;
	}

	bool DX11QuadRenderer::DX11DestroyBuffers(const Group& group)
	{
		DX11Release(group, DX11_VERTEX_BUFFER_KEY);
		DX11Release(group, DX11_COLOR_BUFFER_KEY);
		DX11Release(group, DX11_INDEX_BUFFER_KEY);
		DX11Release(group, DX11_TEXTURE_BUFFER_KEY);

		DX11VertexBuffer = /*DX11ColorBuffer = DX11TextureBuffer =*/ NULL;
		DX11IndexBuffer = NULL;

		return true;
	}
}}
