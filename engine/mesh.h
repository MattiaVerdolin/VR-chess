/**
 * @file	mesh.h
 * @brief	Header file for the Mesh class.
 *
 * This file contains the declaration of the Mesh class, which extends the Node class
 * and represents a 3D mesh in the scene. The class includes methods for rendering, cloning,
 * parsing, and retrieving vertex data.
 *
 * @authors	Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
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


    void setupParameters(Shader* shader, int matEmissionLoc, int matAmbientLoc, int matDiffuseLoc, int matSpecularLoc, int matShininessLoc);

    /**
     * @brief Destructor for the Mesh class.
     */
    ~Mesh();

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

    unsigned int VAO, verticesVbo, normalVerticesVbo, uvVerticesVbo, faceVbo;
    std::vector<unsigned int> faceIndices;
    std::vector<glm::vec3> coorVertices;
    std::vector<glm::vec3> normalVertices;
    std::vector<glm::vec2> uvVertices;

    void setupMesh();
};
