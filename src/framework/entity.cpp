#include "entity.h"
#include "image.h"

Entity::Entity(const Matrix44& modelMatrix, const Mesh& mesh) : modelMatrix(modelMatrix), mesh(mesh) {
    // Constructor que toma una matriz de modelo y una malla
}

void Entity::setModelMatrix(const Matrix44& modelMatrix) {
    // Establecer la matriz del modelo
    this->modelMatrix = modelMatrix;
}

void Entity::setMesh(const Mesh& mesh) {
    // Establecer la malla
    this->mesh = mesh;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
    // Obtener los vértices de la malla
    const std::vector<Vector3>& vertices = mesh.GetVertices();

    // Iterar a través de todos los triángulos en la malla
    for (int i = 0; i < vertices.size(); i += 3) {
        Vector3 triangleVertices[3];
        bool skipTriangle = false;

        // Iterar a través de los vértices del triángulo
        for (int j = 0; j < 3; j++) {
            Vector3 vertex = modelMatrix * vertices[i + j];
            bool negZ;

            // Proyectar el vértice al espacio de recorte
            vertex = camera->ProjectVector(vertex, negZ);

            // Si el vértice está fuera del frustum, descartar el triángulo entero
            if (negZ) {
                skipTriangle = true;
                break;  // Usamos break aquí para salir del ciclo de vértices
            }

            // Convertir las posiciones del espacio de recorte a espacio de pantalla
            vertex.x = (vertex.x + 1) / 2 * 1280;
            vertex.y = (vertex.y + 1) / 2 * 720;
            triangleVertices[j] = vertex;
        }

        // Si se debe omitir el triángulo, continúa con el siguiente
        if (skipTriangle) {
            continue;
        }

        // Dibujar las líneas del triángulo
        framebuffer->DrawLineDDA(static_cast<int>(triangleVertices[0].x), static_cast<int>(triangleVertices[0].y), static_cast<int>(triangleVertices[1].x), static_cast<int>(triangleVertices[1].y), c);
        framebuffer->DrawLineDDA(static_cast<int>(triangleVertices[1].x), static_cast<int>(triangleVertices[1].y), static_cast<int>(triangleVertices[2].x), static_cast<int>(triangleVertices[2].y), c);
        framebuffer->DrawLineDDA(static_cast<int>(triangleVertices[2].x), static_cast<int>(triangleVertices[2].y), static_cast<int>(triangleVertices[0].x), static_cast<int>(triangleVertices[0].y), c);
    }
}
