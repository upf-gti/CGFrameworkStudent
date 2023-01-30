#include "entity.h"
#include "mesh.h"
#include "framework.h"

Entity::Entity() {
	mesh = new Mesh();
	model.SetIdentity();

}

Entity::Entity(Mesh * m) {
	mesh = m;
	model.SetIdentity();
}

Entity::Entity(char* dir) {
	mesh = new Mesh();
	mesh->LoadOBJ(dir);
	model.SetIdentity();
}

//Destructor
Entity::~Entity() {

}

//Render function to render mesh object
void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	Matrix44 cam_view_Matrix = camera->GetViewProjectionMatrix();
	for (Vector3 v : mesh->GetVertices()) {
		
	}
}


