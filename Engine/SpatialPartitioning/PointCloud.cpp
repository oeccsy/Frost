#include "PointCloud.h"
#include "Render/Mesh.h"
#include <stack>

PointCloud::PointCloud(const BoundingBox bounds) : bounds(bounds) { }

PointCloud::~PointCloud() { }

void PointCloud::Insert(Vector3 point)
{
	if (!IsInBounds(point)) return;
	
	if (IsLeaf())
	{
		points.push_back(point);
		if (points.size() > MAX_POINTS_PER_SECTION) Subdivide();
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->Insert(point);
		}
	}
}

Vector3 PointCloud::GetNearPoint(Vector3 pos)
{
	if (!IsInBounds(pos)) throw;

	// 1. Leaf 노드까지 내려가면서 Near Point의 후보 찾기
	PointCloud* cur_section = this;

	while (true)
	{
		if (cur_section->IsLeaf()) break;
		
		for (int i = 0; i < 8; ++i)
		{
			if (cur_section->children[i]->IsInBounds(pos))
			{
				cur_section = cur_section->children[i].get();
				break;
			}
		}
	}
	
	PointCloud* leaf_section = cur_section;

	float candidate_dist_sq = FLT_MAX;
	Vector3 candidate_point;
	bool found_candidate = false;

	for (Vector3 point : leaf_section->points)
	{
		float temp_dist_sq = Vector3::DistanceSquared(point, pos);
		if (temp_dist_sq < candidate_dist_sq)
		{
			candidate_dist_sq = temp_dist_sq;
			candidate_point = point;
			found_candidate = true;
		}
	}

	if (!found_candidate) throw;

	// 2. 후보까지의 거리를 기준으로 재탐색
	float near_dist_sq = candidate_dist_sq;
	Vector3 near_point = candidate_point;
	BoundingSphere check_bounds(pos, sqrt(candidate_dist_sq));

	stack<PointCloud*> dfs_stack;
	dfs_stack.push(this);

	while (!dfs_stack.empty())
	{
		PointCloud* cur_section = dfs_stack.top();
		dfs_stack.pop();

		if (cur_section->IsLeaf())
		{
			for (Vector3 point : cur_section->points)
			{
				float temp_dist_sq = Vector3::DistanceSquared(point, pos);
				if (temp_dist_sq < near_dist_sq)
				{
					near_dist_sq = temp_dist_sq;
					near_point = point;
					check_bounds.Radius = sqrt(near_dist_sq);
				}
			}
		}
		else
		{
			for (int i = 0; i < 8; ++i)
			{
				PointCloud* next_section = cur_section->children[i].get();

				if (next_section->IntersectsWithBounds(check_bounds)) dfs_stack.push(next_section);
			}
		}
	}

	return near_point;
}

vector<Vector3> PointCloud::GetAllPoints()
{
	vector<Vector3> all_points;

	stack<PointCloud*> dfs_stack;
	dfs_stack.push(this);

	while (!dfs_stack.empty())
	{
		PointCloud* cur_section = dfs_stack.top();
		dfs_stack.pop();

		if (cur_section->IsLeaf())
		{
			for (const Vector3& point : cur_section->points)
			{
				all_points.push_back(point);
			}
		}
		else
		{
			for (int i = 0; i < 8; ++i)
			{
				PointCloud* next_section = cur_section->children[i].get();
				if(next_section) dfs_stack.push(next_section);
			}
		}
	}

	return all_points;
}

bool PointCloud::IntersectsWithBounds(const BoundingSphere& bounding_sphere)
{
	return bounds.Intersects(bounding_sphere);
}

bool PointCloud::IntersectsWithPoints(const BoundingSphere& bounding_sphere)
{
	if (!IntersectsWithBounds(bounding_sphere)) return false;

	if (!IsLeaf())
	{
		for (int i = 0; i < 8; i++)
		{
			if (children[i]->IntersectsWithPoints(bounding_sphere)) return true;
		}

		return false;
	}
	else
	{
		float r_squared = bounding_sphere.Radius * bounding_sphere.Radius;

		for (const Vector3& point : points)
		{
			if (Vector3::DistanceSquared(point, bounding_sphere.Center) <= r_squared) return true;
		}

		return false;
	}
}


void PointCloud::Subdivide()
{
	if (!IsLeaf()) return;

	Vector3 cur_bounds = bounds.Extents;
	Vector3 cur_center = bounds.Center;

	Vector3 next_bounds = cur_bounds * 0.5f;

	for (int i = 0; i < 8; i++)
	{
		Vector3 next_center;

		next_center.x = cur_center.x + ((i & 1) ? next_bounds.x : -next_bounds.x);
		next_center.y = cur_center.y + ((i & 2) ? next_bounds.y : -next_bounds.y);
		next_center.z = cur_center.z + ((i & 4) ? next_bounds.z : -next_bounds.z);

		children[i] = make_shared<PointCloud>(BoundingBox{ next_center, next_bounds });
	}

	for (Vector3 point : points)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->Insert(point);
		}
	}

	points.clear();
}

bool PointCloud::IsInBounds(Vector3 position)
{
	Vector3 center = bounds.Center;
	Vector3 size = bounds.Extents;

	Vector3 max_bounds = center + size;
	Vector3 min_bounds = center - size;

	if (position.x < min_bounds.x || max_bounds.x < position.x) return false;
	if (position.y < min_bounds.y || max_bounds.y < position.y) return false;
	if (position.z < min_bounds.z || max_bounds.z < position.z) return false;

	return true;
}