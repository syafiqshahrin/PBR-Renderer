#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Vector.h"
#include <string>
#include <vector>
#include <dwmapi.h>

class Renderer;

struct vertex
{
    vertex(Vector3 p, Vector3 n, Vector2 uv, Vector4 t, float r, float g, float b)
    {

        pos[0] = p.x;
        pos[1] = p.y;
        pos[2] = p.z;

        norm[0] = n.x;
        norm[1] = n.y;
        norm[2] = n.z;

        tan[0] = t.x;
        tan[1] = t.y;
        tan[2] = t.z;
        tan[3] = t.w;

        texc[0] = uv.x;
        texc[1] = uv.y;

        col[0] = r;
        col[1] = g;
        col[2] = b;
        //col[3] = a;
    }
    //Vector3 p;
    float pos[3] = {};
    float norm[3] = {};
    float texc[2] = {};
    float col[3] = {};
    float tan[4] = {};
};

struct SubMesh
{
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector4> tangents;
    std::vector<Vector2> uvs;
    std::vector<unsigned int> indices;
    std::vector<vertex> VertexList;

    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
};

class Mesh
{
public:
    Mesh();
	Mesh(std::string const &filePath);
	~Mesh();
	bool CreateMeshFromFile(Renderer* renderer);
	void BindMesh(int submesh, Renderer* renderer);
    void UpdateVertexList();
    int GetIndexListSize(int submesh);
private:    
    std::vector<SubMesh> Submeshes;
	std::string FilePath;

	void LoadMeshFromFile();
    bool CreateBuffers(Renderer* renderer);
};