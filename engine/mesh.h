/**
 * @file		mesh.h
 * @brief	Mesh class header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Standard libraries
#include <glm/gtc/packing.hpp>
#include <memory>
#include <vector>

// Project-specific headers
#include "node.h"
#include "shader.h"
#include "fbo.h"

 /**
  * @brief Class representing a 3D Mesh object.
  *
  * The Mesh class provides functionality for rendering 3D meshes, parsing mesh data,
  * and managing associated vertices and materials.
  */
class ENG_API Mesh : public Node {
public:
    /**
     * @brief Constructor for the Mesh class.
     * @param name The name of the mesh. Defaults to "defaultMesh".
     */
    Mesh(const std::string& name = "defaultMesh");

    /**
     * @brief Copy constructor for the Mesh class.
     * @param other The Mesh object to copy.
     */
    Mesh(const Mesh& other);

    /**
     * @brief Deleted assignment operator to prevent copying.
     * @return A reference to the Mesh object.
     */
    Mesh& operator=(const Mesh&) = delete;

    /**
     * @brief Renders the mesh using the provided transformation matrix.
     * @param matrix The transformation matrix to apply during rendering.
     */
    void render(const glm::mat4& matrix) override;

    /**
     * @brief Parses mesh data from a binary buffer.
     * @param data A pointer to the binary data buffer.
     * @param position A reference to the current position in the buffer.
     * @return The number of child nodes parsed.
     */
    const unsigned int parse(const char* data, unsigned int& position) override;

    /**
     * @brief Creates a clone of the current Mesh object.
     * @return A pointer to the newly cloned Mesh object.
     */
    Node* clone() const override;

    /**
     * @brief Retrieves the list of vertices in the mesh.
     * @return A vector containing the vertices as glm::vec3 objects.
     */
    std::vector<glm::vec3> getVertices();

    /**
     * @brief Destructor for the Mesh class.
     */
    ~Mesh();

    /**
     * @brief Computes the bounding sphere for the mesh.
     * 
     * This method calculates the center point and radius of the smallest sphere that
     * completely contains all vertices of the mesh. This is useful for collision detection
     * and culling operations.
     */
    void computeBoundingSphere();

    /**
     * @brief Gets the center point of the mesh's bounding sphere.
     * 
     * @return A glm::vec3 containing the coordinates of the bounding sphere's center.
     * @note This value is only valid after computeBoundingSphere() has been called.
     */
    glm::vec3 getBoundingCenter();

    /**
     * @brief Gets the radius of the mesh's bounding sphere.
     * 
     * @return The radius of the bounding sphere as a float value.
     * @note This value is only valid after computeBoundingSphere() has been called.
     */
    float getBoundingRadius();

private:
    /**
     * @brief Reserved structure for internal data storage.
     *
     * This structure is used to manage internal data, such as vertex and face information,
     * for the Mesh class.
     */
    struct Reserved;

    /**
     * @brief Pointer to the reserved internal data structure.
     */
    std::unique_ptr<Reserved> m_reserved;

    /**
     * @brief OpenGL vertex array object identifier.
     */
    unsigned int VAO;

    /**
     * @brief OpenGL vertex buffer object identifiers for different vertex attributes.
     */
    unsigned int verticesVbo, normalVerticesVbo, uvVerticesVbo, faceVbo;

    /**
     * @brief Vector containing the indices for face rendering.
     */
    std::vector<unsigned int> faceIndices;

    /**
     * @brief Vector containing the 3D coordinates of all vertices.
     */
    std::vector<glm::vec3> coorVertices;

    /**
     * @brief Vector containing the normal vectors for all vertices.
     */
    std::vector<glm::vec3> normalVertices;

    /**
     * @brief Vector containing the UV texture coordinates for all vertices.
     */
    std::vector<glm::vec2> uvVertices;

    /**
     * @brief Center point of the mesh's bounding sphere.
     */
    glm::vec3 boundingCenter;

    /**
     * @brief Radius of the mesh's bounding sphere.
     */
    float boundingRadius;

    /**
     * @brief Sets up the OpenGL buffers and vertex attributes for the mesh.
     * 
     * This method initializes the VAO and VBOs, uploads vertex data to the GPU,
     * and configures the vertex attribute pointers for position, normal, and UV coordinates.
     */
    void setupMesh();
};
