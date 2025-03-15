#pragma once
#define _XM_NO_INTRINSICS_

// Base
#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma	comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex64d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex64d.lib")
#endif

// Engine
#include "Graphics.h"
#include "Renderer.h"
#include "VertexData.h"