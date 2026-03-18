#include "ResourceManager.h"
#include "Render/Mesh.h"
#include "Types.h"
#include "Data/Vertex.h"

ResourceManager::ResourceManager() { }

ResourceManager::~ResourceManager() { }

bool ResourceManager::LoadObj(const char* path, shared_ptr<class Mesh> mesh)
{
	vector<Vertex> temp_vertices;
	vector<uint32> temp_indices;
	vector<Vector3> temp_normals;

	FILE* file = NULL;

	if (fopen_s(&file, path, "r") != NULL) return false;

	while (true)
	{
		char line_header[128];

		if (fscanf_s(file, "%s", line_header, 128) == EOF)
			break;

		if (strcmp(line_header, "v") == 0)
		{
			Vertex vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.position.x, &vertex.position.y, &vertex.position.z);

			vertex.uv = Vector2(0, 0);
			vertex.color = Color(0.5f, 0.5f, 0.5f, 1.f);

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(line_header, "f") == 0)
		{
			uint32 vertexIndex[3];
			int matches = fscanf_s(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			if (matches != 3) return false;

			temp_indices.push_back(vertexIndex[0] - 1);
			temp_indices.push_back(vertexIndex[1] - 1);
			temp_indices.push_back(vertexIndex[2] - 1);
		}
	}

	fclose(file);

	temp_normals.resize(temp_vertices.size());

	for (size_t i = 0; i < temp_indices.size(); i += 3)
	{
		uint32 i0 = temp_indices[i];
		uint32 i1 = temp_indices[i + 1];
		uint32 i2 = temp_indices[i + 2];

		Vector3 v0 = temp_vertices[i0].position;
		Vector3 v1 = temp_vertices[i1].position;
		Vector3 v2 = temp_vertices[i2].position;

		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;

		Vector3 normal = v0v1.Cross(v0v2);
		normal.Normalize();

		temp_normals[i0] += normal;
		temp_normals[i1] += normal;
		temp_normals[i2] += normal;
	}

	for (size_t i = 0; i < temp_vertices.size(); i++)
	{
		temp_normals[i].Normalize();
		temp_vertices[i].normal = temp_normals[i];
	}

	mesh->SetVertices(temp_vertices);
	mesh->SetIndices(temp_indices);

	mesh->CreateBuffers();
	mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
