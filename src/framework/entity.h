#pragma once
#include "mesh.h"
#include "image.h"

class Entity
{
public:
	Mesh* mesh;
	Matrix44 model;

	//Constructors
	Entity();
	Entity(Mesh*);
	Entity(const char*);

	//Destructor
	~Entity();

	//Getters & Setters
	Matrix44 getMatrix() { return model; }
	Mesh* getmesh() { return mesh; }
	void setMatrix(Matrix44 m) { model = m; }
	void setMesh(Mesh* m) { mesh = m; }

	//Render Function
    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Update(float sec);

};

