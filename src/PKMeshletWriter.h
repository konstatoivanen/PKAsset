#pragma once
#include "PKAssets/PKAsset.h"
#include "PKAssetWriter.h"

namespace PK::Assets::Mesh
{
    WritePtr<Meshlet::PKMesh> CreateMeshletMesh(PKAssetBuffer& buffer,
                                                const std::vector<PKSubmesh>& submeshes,
                                                float* vertices, 
                                                uint32_t* indices, 
                                                uint32_t offsetPosition,
                                                uint32_t offsetTexcoord,
                                                uint32_t offsetNormal,
                                                uint32_t offsetTangent,
                                                uint32_t vertexStride,
                                                uint32_t vertexCount,
                                                uint32_t indexCount);
}
