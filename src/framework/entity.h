#include "mesh.h"
#include "image.h"
#include "framework.h"

class Entity {
float time;
    
public:

    std::vector<Vector3> lastTriangleVertices; // Almacenar la última posición de los vértices

    // Matriz de modelo
    Matrix44 modelMatrix;

    // Malla
    Mesh mesh;

    // Constructor por defecto
    Entity();

    // Constructor que toma una matriz de modelo
    Entity(const Matrix44& modelMatrix);

    // Constructor que toma una malla
    Entity(const Mesh& mesh);

    // Constructor que toma una matriz de modelo y una malla
    Entity(const Matrix44& modelMatrix, const Mesh& mesh);

    // Método para establecer la matriz del modelo
    void setModelMatrix(const Matrix44& modelMatrix);

    // Método para establecer la malla
    void setMesh(const Mesh& mesh); 

    // Método para renderizar la entidad
    void Render(Image* framebuffer, Camera* camera, const Color& c);

    void Update(float seconds_elapsed);
};
