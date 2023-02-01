#include "entity.h"
#include "mesh.h"
#include "framework.h"

Entity::Entity() {
	mesh = new Mesh();

}

Entity::Entity(Mesh * m) {
	mesh = m;
}

Entity::Entity(char* dir) {
	mesh = new Mesh();
	mesh->LoadOBJ(dir);
}

//Destructor
Entity::~Entity() {

}

//Render function to render mesh object
void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	
	/*Matrix44 P;
	camera->SetOrthographic
	model._11 = 2 / (camera->right - camera->left);
	model._22 = 2 / (camera->top - camera->bottom);
	model._33 = 2 / (camera->far_plane - camera->near_plane);

	model._41 = -(camera->right + camera->left) / (camera->right - camera->left);
	model._42 = -(camera->top + camera->bottom) / (camera->top - camera->bottom);
	model._43 = -(camera->far_plane + camera->near_plane) / (camera->far_plane - camera->near_plane);
	model._44 = 1;
	*/
	
	std::vector<Vector3> vertices = mesh->GetVertices();

	for (int i = 0; i < (vertices.size()-3); i+=3) {
		Vector3 v1 = vertices[i];
		Vector3 v2 = vertices[i+1];
		Vector3 v3 = vertices[i+2];
		//Transforma local to World
		v1 = model * v1;
		v2 = model * v2;
		v3 = model * v3;

		//World Space to Clip Space
		//Calculates if z is outside the camera
		bool negZ1;
		bool negZ2;
		bool negZ3;
		v1 = camera->ProjectVector(v1, negZ1);
		v2 = camera->ProjectVector(v2, negZ2);
		v3 = camera->ProjectVector(v3, negZ3);

		//Convert clip space positions to screen space
		if (!negZ1 && !negZ2 && !negZ3) {

			v1.x = v1.x * (framebuffer->width - 1);
			v1.y = v1.y * (framebuffer->height - 1);

			v2.x = v2.x * (framebuffer->width - 1);
			v2.y = v2.y * (framebuffer->height - 1);

			v3.x = v3.x * (framebuffer->width - 1);
			v3.y = v3.y * (framebuffer->height - 1);

			//Set Line Bresenham to buffer
			framebuffer->DrawLineBresenham(v1.x, v1.y, v2.x, v2.y, c);
			framebuffer->DrawLineBresenham(v2.x, v2.y, v3.x, v3.y, c);
			framebuffer->DrawLineBresenham(v3.x, v3.y, v1.x, v1.y, c);
		}
		
	}
	//Alternative
	/*
	std::vector<Vector3> vertices(3);
	for (Vector3 v : mesh->GetVertices()) {
		//Transforma local to World
		v = model * v;
		
		//World Space to Clip Space
		bool negZ;//Calculates if z is outside the camera
		v = camera->ProjectVector(v, negZ);

		//Convert clip space positions to screen space
		v.x = v.x * (framebuffer->width - 1);
		v.y = v.y * (framebuffer->height - 1);

		if (negZ) {
			vertices.clear();
			continue;
		}
		vertices.push_back(v);

		if (vertices.size() == 3){
			framebuffer->DrawLineBresenham(vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y, c);
			framebuffer->DrawLineBresenham(vertices[1].x, vertices[1].y, v.x, v.y, c);
			framebuffer->DrawLineBresenham(v.x, v.y, vertices[0].x, vertices[0].y, c);
			vertices.clear();
		}

	}*/
}


