#include "mesh.h"
#include "utils.h"
#include "camera.h"

#include <string>
#include <sys/stat.h>
#include <cstring>

Mesh::Mesh()
{
}

void Mesh::Clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
}

void Mesh::Render(int primitive)
{
	// Render the mesh using your rasterizer
	assert(vertices.size() && "No vertices in this mesh");

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0]);
	}

	glDrawArrays(primitive, 0, static_cast<GLsizei>(vertices.size()));
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::CreateQuad()
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	// Create six vertices (3 for upperleft triangle and 3 for lowerright)
	vertices.push_back(Vector3(1, 1, 0));
	vertices.push_back(Vector3(1, -1, 0));
	vertices.push_back(Vector3(-1, -1, 0));
	vertices.push_back(Vector3(-1, 1, 0));
	vertices.push_back(Vector3(1, 1, 0));
	vertices.push_back(Vector3(-1, -1, 0));

	// Texture coordinates
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));
}

void Mesh::CreatePlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	// Create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(size, 0, size));
	vertices.push_back(Vector3(size, 0, -size));
	vertices.push_back(Vector3(-size, 0, -size));
	vertices.push_back(Vector3(-size, 0, size));
	vertices.push_back(Vector3(size, 0, size));
	vertices.push_back(Vector3(-size, 0, -size));

	// All of them have the same normal
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	// Texture coordinates
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));
}

void Mesh::CreateCube(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	
	vertices.push_back(Vector3(size,  size, size));
	vertices.push_back(Vector3(size,  size, -size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(size,  size, size));
	vertices.push_back(Vector3(-size, size, -size));

	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(size, size, -size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(size, -size, -size));

	vertices.push_back(Vector3(size,  size , size ));
	vertices.push_back(Vector3(size, -size,  size ));
	vertices.push_back(Vector3(-size, -size, size ));
	vertices.push_back(Vector3(-size, size,  size ));
	vertices.push_back(Vector3(size, size,   size ));
	vertices.push_back(Vector3(-size, -size, size ));

	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(-size, -size, -size));

	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(-size, -size, -size));

	vertices.push_back(Vector3(size, size, -size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(size, size, -size));
	vertices.push_back(Vector3(-size, -size, -size));

	// All of them have the same normal
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(1, 0, 0));

	normals.push_back(Vector3(0, 0, 1));
	normals.push_back(Vector3(0, 0, 1));
	normals.push_back(Vector3(0, 0, 1));
	normals.push_back(Vector3(0, 0, 1));
	normals.push_back(Vector3(0, 0, 1));
	normals.push_back(Vector3(0, 0, 1));

	// All of them have the same normal
	normals.push_back(Vector3(0, -1, 0));
	normals.push_back(Vector3(0, -1, 0));
	normals.push_back(Vector3(0, -1, 0));
	normals.push_back(Vector3(0, -1, 0));
	normals.push_back(Vector3(0, -1, 0));
	normals.push_back(Vector3(0, -1, 0));

	normals.push_back(Vector3(-1, 0, 0));
	normals.push_back(Vector3(-1, 0, 0));
	normals.push_back(Vector3(-1, 0, 0));
	normals.push_back(Vector3(-1, 0, 0));
	normals.push_back(Vector3(-1, 0, 0));
	normals.push_back(Vector3(-1, 0, 0));

	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));
	normals.push_back(Vector3(0, 0, -1));

	// Texture coordinates
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));
}

bool Mesh::LoadOBJ(const char* filename)
{
	struct stat stbuffer;
	std::cout << "Loading mesh: " << filename << std::endl;

	std::string relPath = absResPath(filename);

	FILE* f = fopen(relPath.c_str(), "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(relPath.c_str(), &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	std::vector<Vector3> indexed_positions;
	std::vector<Vector3> indexed_normals;
	std::vector<Vector2> indexed_uvs;

	const float max_float = 10000000;
	const float min_float = -10000000;

	unsigned int vertex_i = 0;

	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		std::memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = tokenize(line, " ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3 v(std::stof(tokens[1].c_str()), std::stof(tokens[2].c_str()), std::stof(tokens[3].c_str()));
			indexed_positions.push_back(v);
		}
		else if (tokens[0] == "vt" && (tokens.size() == 4 || tokens.size() == 3))
		{
			Vector2 v(std::stof(tokens[1].c_str()), std::stof(tokens[2].c_str()));
			indexed_uvs.push_back(v);
		}
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3 v(std::stof(tokens[1].c_str()), std::stof(tokens[2].c_str()), std::stof(tokens[3].c_str()));
			indexed_normals.push_back(v);
		}
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3 v1, v2, v3;
			v1 = parseVector3(tokens[1].c_str(), '/');

			for (size_t iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2 = parseVector3(tokens[iPoly].c_str(), '/');
				v3 = parseVector3(tokens[iPoly + 1].c_str(), '/');

				vertices.push_back(indexed_positions[(unsigned int)(v1.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v2.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v3.x) - 1]);
				//triangles.push_back( VECTOR_INDICES_TYPE(vertex_i, vertex_i+1, vertex_i+2) ); //not needed
				vertex_i += 3;

				if (indexed_uvs.size() > 0)
				{
					uvs.push_back(indexed_uvs[(unsigned int)(v1.y) - 1]);
					uvs.push_back(indexed_uvs[(unsigned int)(v2.y) - 1]);
					uvs.push_back(indexed_uvs[(unsigned int)(v3.y) - 1]);
				}

				if (indexed_normals.size() > 0)
				{
					normals.push_back(indexed_normals[(unsigned int)(v1.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v2.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v3.z) - 1]);
				}
			}
		}
	}

	delete[] data;

	return true;
}
