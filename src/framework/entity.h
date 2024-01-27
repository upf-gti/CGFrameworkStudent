#include "mesh.h"
#include "image.h"

class Entity {

    Matrix44 modelMatrix;
    Mesh mesh;

public:
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

};


