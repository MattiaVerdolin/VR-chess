/**
 * @file    node.h
 * @brief   Declaration of the Node class, representing a hierarchical 3D scene graph node.
 *
 * This file defines the Node class, which inherits from Object and provides additional
 * functionality for hierarchical transformations, parent-child relationships, and materials.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Standard libraries
#include <vector>

// Project-specific headers
#include "object.h"
#include "material.h"
#include "notificationService.h"

/////////////////////
// MAIN NODE CLASS //
/////////////////////

/**
 * @brief Represents a node in a hierarchical 3D scene graph.
 *
 * A Node stores its local transformation matrix, maintains parent-child relationships,
 * and can hold a material for rendering. It supports recursive operations for
 * transformations and notifications.
 */
class ENG_API Node : public Object {
public:
    /**
     * @brief Constructor for the Node class.
     * @param name The name of the node (default is "defaultNode").
     */
    Node(const std::string& name = "defaultNode");

    /**
     * @brief Destructor for the Node class.
     */
    virtual ~Node();

    /**
     * @brief Deleted copy assignment operator.
     */
    Node& operator=(const Node&) = delete;

    /**
     * @brief Set the local transformation matrix for this node.
     * @param matrix The new transformation matrix.
     */
    void setMatrix(const glm::mat4& matrix);

    /**
     * @brief Get the local transformation matrix of this node.
     * @return A constant reference to the local transformation matrix.
     */
    const glm::mat4& getMatrix() const;

    /**
     * @brief Compute the final transformation matrix by combining with parent matrices.
     * @return The final transformation matrix.
     */
    virtual const glm::mat4 getFinalMatrix() const;

    /**
     * @brief Find a child node by its name.
     * @param name The name of the node to find.
     * @return A pointer to the found node, or nullptr if not found.
     */
    const Node* findNodeByName(const std::string& name) const;

    /**
     * @brief Retrieve the main camera node from the hierarchy.
     * @return A pointer to the main camera node, or nullptr if not found.
     */
    Node* getMainCamera() const;

    /**
     * @brief Get the parent node of this node.
     * @return A constant pointer to the parent node.
     */
    const Node* getParent() const;

    /**
     * @brief Add a child node to this node.
     * @param child The child node to add.
     * @return True if the child was successfully added, false otherwise.
     */
    bool addChild(Node* child);

    /**
     * @brief Remove a child node from this node.
     * @param child The child node to remove.
     * @return True if the child was successfully removed, false otherwise.
     */
    bool removeChild(Node* child);

    /**
     * @brief Remove a child node by its position in the child list.
     * @param position The position of the child to remove.
     * @return A pointer to the removed child, or nullptr if the position is invalid.
     */
    Node* removeChildByPosition(const unsigned int& position);

    /**
     * @brief Get the number of children of this node.
     * @return The number of child nodes.
     */
    const unsigned int getNumberOfChildren() const;

    /**
     * @brief Get the child nodes of this node.
     * @return A vector of pointers to the child nodes.
     */
    virtual const std::vector<Node*> getChildren() const;

    /**
     * @brief Assign a material to this node.
     * @param material The material to assign.
     */
    void setMaterial(Material* material);

    /**
     * @brief Get the material assigned to this node.
     * @return A pointer to the material.
     */
    Material* getMaterial() const;

    /**
     * @brief Render the node, printing its name.
     * @param transform The transformation matrix to apply (default is the identity matrix).
     */
    void virtual render(const glm::mat4& transform = glm::mat4(1.0f)) override { std::cout << this->getName() << std::endl; };

    /**
     * @brief Parse the node's data from a buffer.
     * @param data The buffer containing the node data.
     * @param position The current offset in the buffer.
     * @return The new offset after parsing the node.
     */
    const virtual unsigned int parse(const char* data, unsigned int& position) override;

    /**
     * @brief Create a deep copy of this node.
     * @return A pointer to the cloned node.
     */
    virtual Node* clone() const;

    /**
     * @brief Enable or disable lighting for this node.
     * @param val True to enable lighting, false to disable.
     */
    void setEnableLighting(const bool& val);

protected:
    /**
     * @brief Copy constructor for the Node class.
     * @param other The Node instance to copy.
     */
    Node(const Node& other);

    /**
     * @brief Recursively clone this node's children.
     * @param newNode The cloned parent node.
     */
    virtual void recursiveClone(Node* newNode) const;

    /**
     * @brief Notify the hierarchy of a matrix change.
     */
    void recursiveNotifyChangeMatrix();

    /**
     * @brief Check if lighting is enabled for this node.
     * @return True if lighting is enabled, false otherwise.
     */
    bool isEnableLighting() const;

    /// Local transformation matrix of the node.
    glm::mat4 m_matrix;

    /// Parent node of this node.
    Node* m_parent;

    /// Child nodes of this node.
    std::vector<Node*> m_children;

    /// Material assigned to this node.
    Material* m_material;

    /// Lighting enable flag.
    bool m_enableLighting;

private:
    /**
     * @brief Set the parent node for this node.
     * @param parent The parent node.
     */
    void setParent(Node* parent);

    /// Reference to the notification service instance.
    NotificationService& m_notificationService;

    /// Indicates whether the matrix was parsed from external data.
    bool m_isMatrixParsed;
};
