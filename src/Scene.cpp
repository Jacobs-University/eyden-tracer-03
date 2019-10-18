#include "Scene.h"
#include <fstream>
#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "PrimTriangle.h"


void CScene::ParseOBJ(const std::string& fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;

	std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE ---
	std::vector<unsigned int> vertexIndices;
	std::vector<Vec3f> temp_vertices;

	std::ifstream file;
	file.open(fileName);
	if (!file.is_open()){
		std::cout << "Impossible to open the file " << std::endl;
		return;
	}

	while(!file.eof()){
		char line = file.get();
		if (line == 'v'){
			Vec3f vertex;
			float a, b ,c;
			file >> a >> b >> c;
			vertex[0] = a * 99.99f;
			vertex[1] = b * 99.99f;
			vertex[2] = c * 99.99f;
			temp_vertices.push_back(vertex);
		}

		else if(line == 'f'){
			unsigned int vertexIndex[3];
			unsigned int a, b, c;
			file >>a >> b >> c;
			vertexIndex[0] = a;
			vertexIndex[1] = b;
			vertexIndex[2] = c;
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
		}
	}

	int i = 0;
	while(i < vertexIndices.size()){
		Vec3f v1,v2,v3;
		v1 = temp_vertices[vertexIndices[i] - 1];
		i++;
		v2 = temp_vertices[vertexIndices[i] - 1];
		i++;
		v3 = temp_vertices[vertexIndices[i] - 1];
		i++;
		Add(std::make_shared<CPrimTriangle>(v1, v2, v3,pShader));
	}
	file.close();


	std::cout << "Finished Parsing" << std::endl;
	return;
}

