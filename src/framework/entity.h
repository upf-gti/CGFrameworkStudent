#pragma once
#include "mesh.h"
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

};

