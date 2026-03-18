#include "MeshSampler.h"
#include "Render/Mesh.h"
#include "Utils/Math/Triangle3D.h"
#include <random>

vector<Vertex> MeshSampler::GenerateMeshVertices(shared_ptr<class Mesh> base_mesh, int amount)
{
	vector<Vertex> base_vertices = base_mesh->GetVertices();
	vector<Vertex> shuffled_vertices = base_vertices;

	random_device rd;
	mt19937 gen(rd());
	shuffle(shuffled_vertices.begin(), shuffled_vertices.end(), gen);

	amount = min(amount, base_vertices.size());

	vector<Vertex> result;
	for (int i = 0; i < amount; i++)
	{
		result.push_back(shuffled_vertices[i]);
	}
	
	return result;
}

vector<Vertex> MeshSampler::GenerateMeshTriangleCenters(shared_ptr<class Mesh> base_mesh, int amount)
{
	if (base_mesh->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex> base_vertices = base_mesh->GetVertices();
	vector<uint32> base_indices = base_mesh->GetIndices();

	vector<int> triangle_first_indices;

	for (int i = 0; i < base_indices.size() / 3; i++)
	{
		triangle_first_indices.push_back(i * 3);
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(triangle_first_indices.begin(), triangle_first_indices.end(), gen);

	amount = min(amount, triangle_first_indices.size());

	vector<Vertex> result;
	for (int i = 0; i < amount; i++)
	{
		int triangle_first_index = triangle_first_indices[i];
		int a_index = base_indices[triangle_first_index];
		int b_index = base_indices[triangle_first_index + 1];
		int c_index = base_indices[triangle_first_index + 2];

		Vector3 a = base_vertices[a_index].position;
		Vector3 b = base_vertices[b_index].position;
		Vector3 c = base_vertices[c_index].position;
		Vector3 triangle_center = Triangle3D::CalculateCenterPosition(a, b, c);
		
		Vector3 a_normal = base_vertices[a_index].normal;
		Vector3 b_normal = base_vertices[b_index].normal;
		Vector3 c_normal = base_vertices[c_index].normal;
		Vector3 triangle_center_normal = a_normal + b_normal + c_normal;
		triangle_center_normal.Normalize();
		
		result.push_back(Vertex{ triangle_center, triangle_center_normal });
	}
	
	return result;
}

vector<Vertex> MeshSampler::GenerateScatterPoints(shared_ptr<class Mesh> base_mesh, int amount)
{
	if (base_mesh->GetTopology() != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) throw;

	vector<Vertex> base_vertices = base_mesh->GetVertices();
	vector<uint32> base_indices = base_mesh->GetIndices();

	vector<int> triangle_first_indices;

	for (int i = 0; i < base_indices.size() / 3; i++)
	{
		triangle_first_indices.push_back(i * 3);
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(triangle_first_indices.begin(), triangle_first_indices.end(), gen);

	amount = min(amount, triangle_first_indices.size());

	vector<Vertex> result;
	for (int i = 0; i < amount; i++)
	{
		int triangle_first_index = triangle_first_indices[i];
		int a_index = base_indices[triangle_first_index];
		int b_index = base_indices[triangle_first_index + 1];
		int c_index = base_indices[triangle_first_index + 2];

		Vector3 a = base_vertices[a_index].position;
		Vector3 b = base_vertices[b_index].position;
		Vector3 c = base_vertices[c_index].position;
		Vector3 random_point_in_triangle = Triangle3D::SelectRandomPointInTriangle(a, b, c);
		
		Vector3 a_normal = base_vertices[a_index].normal;
		Vector3 b_normal = base_vertices[b_index].normal;
		Vector3 c_normal = base_vertices[c_index].normal;
		Vector3 random_point_in_triangle_normal = a_normal + b_normal + c_normal;
		random_point_in_triangle_normal.Normalize();
		
		result.push_back(Vertex{ random_point_in_triangle, random_point_in_triangle_normal });
	}
	
	return result;
}