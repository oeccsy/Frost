#pragma once

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

public:
	static bool LoadObj(const char* path, shared_ptr<Mesh> mesh);
};