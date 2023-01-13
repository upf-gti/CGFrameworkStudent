/*
	This class wraps the behaviour of a camera. A Camera helps to set the point of view from where we will render the scene.
	The most important attributes are  eye and center which say where is the camera and where is it pointing.
	This class also stores the matrices used to do the transformation and projection of the scene.
*/
#pragma once

#include "framework.h"

class Camera
{
	// OpenGL methods to fill matrices
	// Only to test Draw3DEntity
	void SetExampleViewMatrix();
	void SetExampleProjectionMatrix();

public:

	// Types of cameras available
	enum { PERSPECTIVE, ORTHOGRAPHIC }; 
	char type;

	// Vectors to define the orientation of the camera
	Vector3 eye;	// Where is the camera
	Vector3 center; // Where is it pointing
	Vector3 up;		// The up pointing up

	// Properties of the projection of the camera
	float fov;			// View angle in degrees (1/zoom)
	float aspect;		// Aspect ratio (width/height)
	float near_plane;	// Near plane
	float far_plane;	// Far plane

	// For orthogonal projection
	float left, right, top, bottom;

	// Matrices
	Matrix44 view_matrix;
	Matrix44 projection_matrix;
	Matrix44 viewprojection_matrix;

	Camera();

	// Setters
	void SetAspectRatio(float aspect) { this->aspect = aspect; };

	// Translate and rotate the camera
	void Move(Vector3 delta);
	void Rotate(float angle, const Vector3& axis);

	// Transform a local camera vector to world coordinates
	Vector3 GetLocalVector(const Vector3& v);

	// Project 3D Vectors to 2D Homogeneous Space
	// If negZ is true, the projected point IS NOT inside the frustum, 
	// so it does not have to be rendered!
	Vector3 ProjectVector(Vector3 pos, bool& negZ);

	// Set the info for each projection
	void SetPerspective(float fov, float aspect, float near_plane, float far_plane);
	void SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane);
	void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	// Compute the matrices
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	Matrix44 GetViewProjectionMatrix();
};