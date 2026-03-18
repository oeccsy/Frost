#pragma once

#include "Core.h"
#include <memory>

class ENGINE_API ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

public:
	static bool LoadObj(const char* path, shared_ptr<class Mesh> mesh);
};