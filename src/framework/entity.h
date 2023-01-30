#pragma once
#include "mesh.h"
#include "image.h"

class Entity
{
public:
	Mesh * mesh;
	Matrix44 model;

	//Constructors
	Entity();
	Entity(Mesh *);
	Entity(char*);

	//Destructor
	~Entity();

	//Render Function
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);

};

