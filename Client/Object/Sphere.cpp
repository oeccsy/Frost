#include "pch.h"
#include "Sphere.h"
#include "Data/Vertex.h"
#include "Render/Mesh.h"
#include "Render/Material.h"
#include "Render/Renderer/Renderer.h"

Sphere::Sphere() {}

Sphere::~Sphere() {}

void Sphere::Awake()
{
	Object::Awake();
 
	CreateGeometry();
	CreateShader();
	CreateInputLayout();
}

void Sphere::Update(float delta_time)
{
	Object::Update(delta_time);
}

void Sphere::CreateGeometry()
{
	float radius = 3.0f;
	uint32 stack_count = 10;
	uint32 slice_count = 10;
	float stack_angle = XM_PI / stack_count;
	float slice_angle = XM_2PI / slice_count;
	float du = 1.f / static_cast<float>(slice_count);
	float dv = 1.f / static_cast<float>(stack_count);

	vector<Vertex> vertices;

	vertices.push_back(Vertex{ Vector3(0.0f, radius, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.5f, 0.0f), Color(0.5f, 0.5f, 0.5f, 1.f) });

	for (uint32 y = 1; y <= stack_count - 1; y++)
	{
		float theta_y = y * stack_angle;

		for (uint32 x = 0; x <= slice_count; x++)
		{
			float theta_x = x * slice_angle;

			Vertex vertex;
			vertex.position.x = radius * sinf(theta_y) * cosf(theta_x);
			vertex.position.y = radius * cosf(theta_y);
			vertex.position.z = radius * sinf(theta_y) * sinf(theta_x);

			vertex.normal = Vector3(vertex.position.x / radius, vertex.position.y / radius, vertex.position.z / radius);

			vertex.uv = Vector2(du * x, dv * y);

			vertex.color = Color(0.5f, 0.5f, 0.5f, 1.f);

			vertices.push_back(vertex);
		}
	}

	vertices.push_back(Vertex{ Vector3(0.0f, -radius, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.5f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.f) });


	vector<uint32> indices;

	for (uint32 i = 0; i < slice_count; i++)
	{
		//  [0]
		//   |  
		//  [i+1]-[i+2]
		indices.push_back(0);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
	}

	uint32 ring_vertex_count = slice_count + 1;
	for (uint32 y = 0; y < stack_count - 2; y++)
	{
		for (uint32 x = 0; x < slice_count; x++)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			indices.push_back(1 + (y)*ring_vertex_count + (x));
			indices.push_back(1 + (y)*ring_vertex_count + (x + 1));
			indices.push_back(1 + (y + 1) * ring_vertex_count + (x));

			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			indices.push_back(1 + (y + 1) * ring_vertex_count + (x));
			indices.push_back(1 + (y)*ring_vertex_count + (x + 1));
			indices.push_back(1 + (y + 1) * ring_vertex_count + (x + 1));
		}
	}

	uint32 bottom_index = static_cast<uint32>(vertices.size()) - 1;
	uint32 last_ring_start_index = bottom_index - ring_vertex_count;
	for (uint32 i = 0; i < slice_count; i++)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		indices.push_back(bottom_index);
		indices.push_back(last_ring_start_index + i);
		indices.push_back(last_ring_start_index + i + 1);
	}

	GetMesh()->SetVertices(vertices);
	GetMesh()->SetIndices(indices);

	GetMesh()->CreateBuffers();
	GetMesh()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sphere::CreateShader()
{
	GetMaterial()->CreateVS(L"Shader/Phong.hlsl", "VS", "vs_5_0");
	GetMaterial()->CreatePS(L"Shader/Phong.hlsl", "PS", "ps_5_0");
}

void Sphere::CreateInputLayout()
{
	GetMesh()->CreateInputLayout(GetMaterial());
}
