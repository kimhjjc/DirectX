#include "FireModelClass.h"

FireModelClass::FireModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture1 = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
	m_model = 0;
}

FireModelClass::FireModelClass(const FireModelClass & other)
{
}

FireModelClass::~FireModelClass()
{
}

bool FireModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2,
	WCHAR* textureFilename3)
{
	bool result;


	// Load in the model data,
	//result = LoadModel(modelFilename);

	result = ReadFileCounts(modelFilename);
	if (!result)
	{
		return false;
	}
	result = LoadDataStructures(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

void FireModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void FireModelClass::Render(ID3D11DeviceContext * deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int FireModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * FireModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}

ID3D11ShaderResourceView * FireModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}

ID3D11ShaderResourceView * FireModelClass::GetTexture3()
{
	return m_Texture3->GetTexture();
}

bool FireModelClass::InitializeBuffers(ID3D11Device *device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void FireModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void FireModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool FireModelClass::LoadTextures(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	bool result;


	// Create the texture object.
	m_Texture1 = new TextureClass;
	if (!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, textureFilename1);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture2 = new TextureClass;
	if (!m_Texture2)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture2->Initialize(device, textureFilename2);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture3 = new TextureClass;
	if (!m_Texture3)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture3->Initialize(device, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

void FireModelClass::ReleaseTextures()
{
	// Release the texture objects.
	if (m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	if (m_Texture2)
	{
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	if (m_Texture3)
	{
		m_Texture3->Shutdown();
		delete m_Texture3;
		m_Texture3 = 0;
	}

	return;
}

bool FireModelClass::LoadModel(char *filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void FireModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool FireModelClass::ReadFileCounts(char* filename)
{
	ifstream fin;
	char input;
	bool checkFaceCountIncrease;
	// Initialize the counts.
	m_vertexCount = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
	m_faceShape = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		checkFaceCountIncrease = false;
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { m_vertexCount++; }
			if (input == 't') { m_textureCount++; }
			if (input == 'n') { m_normalCount++; }
		}
		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { m_faceCount++; checkFaceCountIncrease = true; }
		}
		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (checkFaceCountIncrease && input == ' ') { m_faceShape++; }
		}
		if (m_faceShape > 4)
			m_faceShape /= 2;
		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	return true;
}

bool FireModelClass::LoadDataStructures(char* filename)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;


	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	texcoords = new VertexType[m_textureCount];
	if (!texcoords)
	{
		return false;
	}
	normals = new VertexType[m_normalCount];
	if (!normals)
	{
		return false;
	}
	faces = new FaceType[m_faceCount];
	if (!faces)
	{
		return false;
	}
	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].position.x >> vertices[vertexIndex].position.y >>
					vertices[vertexIndex].position.z;
				// Invert the Z vertex to change to left hand system.
					vertices[vertexIndex].position.z = vertices[vertexIndex].position.z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].texture.x >> texcoords[texcoordIndex].texture.y;
				// Invert the V texture coordinates to left hand system.
					texcoords[texcoordIndex].texture.y = 1.0f - texcoords[texcoordIndex].texture.y;
				texcoordIndex++;
			}
			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].normal.x >> normals[normalIndex].normal.y >>
					normals[normalIndex].normal.z;
				// Invert the Z normal to change to left hand system.
					normals[normalIndex].normal.z = normals[normalIndex].normal.z * -1.0f;
				normalIndex++;
			}
		}
		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				if (m_faceShape == 3)
				{
					fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
						input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
						faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
						faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
						faces[faceIndex].nIndex1;
					faceIndex++;
				}
				else if (m_faceShape == 4)
				{
					fin >> faces[faceIndex].vIndex4 >> input2 >> faces[faceIndex].tIndex4 >>
						input2 >> faces[faceIndex].nIndex4 >> faces[faceIndex].vIndex3 >>
						input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3 >>
						faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >>
						faces[faceIndex].nIndex2 >> faces[faceIndex].vIndex1 >> input2 >>
						faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
					faceIndex++;
				}
			}
		}
		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	if (m_faceShape == 3)
		m_vertexCount = m_faceCount * 3;
	else if (m_faceShape == 4)
		m_vertexCount = m_faceCount * 6;
	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Initialize the four data structures.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	// »ï°¢ ·»´õ
	if (m_faceShape == 3)
		for (int i = 0; i < faceIndex; i++)
		{
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;
			m_model[i * 3].x = vertices[vIndex].position.x;
			m_model[i * 3].y = vertices[vIndex].position.y;
			m_model[i * 3].z = vertices[vIndex].position.z;
			m_model[i * 3].tu = texcoords[tIndex].texture.x;
			m_model[i * 3].tv = texcoords[tIndex].texture.y;
			m_model[i * 3].nx = normals[nIndex].normal.x;
			m_model[i * 3].ny = normals[nIndex].normal.y;
			m_model[i * 3].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;
			m_model[i * 3 + 1].x = vertices[vIndex].position.x;
			m_model[i * 3 + 1].y = vertices[vIndex].position.y;
			m_model[i * 3 + 1].z = vertices[vIndex].position.z;
			m_model[i * 3 + 1].tu = texcoords[tIndex].texture.x;
			m_model[i * 3 + 1].tv = texcoords[tIndex].texture.y;
			m_model[i * 3 + 1].nx = normals[nIndex].normal.x;
			m_model[i * 3 + 1].ny = normals[nIndex].normal.y;
			m_model[i * 3 + 1].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;
			m_model[i * 3 + 2].x = vertices[vIndex].position.x;
			m_model[i * 3 + 2].y = vertices[vIndex].position.y;
			m_model[i * 3 + 2].z = vertices[vIndex].position.z;
			m_model[i * 3 + 2].tu = texcoords[tIndex].texture.x;
			m_model[i * 3 + 2].tv = texcoords[tIndex].texture.y;
			m_model[i * 3 + 2].nx = normals[nIndex].normal.x;
			m_model[i * 3 + 2].ny = normals[nIndex].normal.y;
			m_model[i * 3 + 2].nz = normals[nIndex].normal.z;
		}
	// »ç°¢ ·»´õ
	if (m_faceShape == 4)
		for (int i = 0; i < faceIndex; i++)
		{
			//Ã¹¹øÂ° »ï°¢Çü
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;
			m_model[i * 6].x = vertices[vIndex].position.x;
			m_model[i * 6].y = vertices[vIndex].position.y;
			m_model[i * 6].z = vertices[vIndex].position.z;
			m_model[i * 6].tu = texcoords[tIndex].texture.x;
			m_model[i * 6].tv = texcoords[tIndex].texture.y;
			m_model[i * 6].nx = normals[nIndex].normal.x;
			m_model[i * 6].ny = normals[nIndex].normal.y;
			m_model[i * 6].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;
			m_model[i * 6 + 1].x = vertices[vIndex].position.x;
			m_model[i * 6 + 1].y = vertices[vIndex].position.y;
			m_model[i * 6 + 1].z = vertices[vIndex].position.z;
			m_model[i * 6 + 1].tu = texcoords[tIndex].texture.x;
			m_model[i * 6 + 1].tv = texcoords[tIndex].texture.y;
			m_model[i * 6 + 1].nx = normals[nIndex].normal.x;
			m_model[i * 6 + 1].ny = normals[nIndex].normal.y;
			m_model[i * 6 + 1].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;
			m_model[i * 6 + 2].x = vertices[vIndex].position.x;
			m_model[i * 6 + 2].y = vertices[vIndex].position.y;
			m_model[i * 6 + 2].z = vertices[vIndex].position.z;
			m_model[i * 6 + 2].tu = texcoords[tIndex].texture.x;
			m_model[i * 6 + 2].tv = texcoords[tIndex].texture.y;
			m_model[i * 6 + 2].nx = normals[nIndex].normal.x;
			m_model[i * 6 + 2].ny = normals[nIndex].normal.y;
			m_model[i * 6 + 2].nz = normals[nIndex].normal.z;

			// µÎ¹øÂ° »ï°¢Çü
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;
			m_model[i * 6 + 3].x = vertices[vIndex].position.x;
			m_model[i * 6 + 3].y = vertices[vIndex].position.y;
			m_model[i * 6 + 3].z = vertices[vIndex].position.z;
			m_model[i * 6 + 3].tu = texcoords[tIndex].texture.x;
			m_model[i * 6 + 3].tv = texcoords[tIndex].texture.y;
			m_model[i * 6 + 3].nx = normals[nIndex].normal.x;
			m_model[i * 6 + 3].ny = normals[nIndex].normal.y;
			m_model[i * 6 + 3].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;
			m_model[i * 6 + 4].x = vertices[vIndex].position.x;
			m_model[i * 6 + 4].y = vertices[vIndex].position.y;
			m_model[i * 6 + 4].z = vertices[vIndex].position.z;
			m_model[i * 6 + 4].tu = texcoords[tIndex].texture.x;
			m_model[i * 6 + 4].tv = texcoords[tIndex].texture.y;
			m_model[i * 6 + 4].nx = normals[nIndex].normal.x;
			m_model[i * 6 + 4].ny = normals[nIndex].normal.y;
			m_model[i * 6 + 4].nz = normals[nIndex].normal.z;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;
			m_model[i * 6 + 5].x = vertices[vIndex].position.x;
			m_model[i * 6 + 5].y = vertices[vIndex].position.y;
			m_model[i * 6 + 5].z = vertices[vIndex].position.z;
			m_model[i * 6 + 5].tu = texcoords[tIndex].texture.x;
			m_model[i * 6 + 5].tv = texcoords[tIndex].texture.y;
			m_model[i * 6 + 5].nx = normals[nIndex].normal.x;
			m_model[i * 6 + 5].ny = normals[nIndex].normal.y;
			m_model[i * 6 + 5].nz = normals[nIndex].normal.z;
		}
	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	return true;
}
