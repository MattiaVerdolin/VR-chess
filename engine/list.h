/**
 * @file    list.h
 * @brief   Concrete List include file
 *
 * This file defines the List class, which manages a collection of nodes for rendering purposes.
 * It holds and renders the elements in the scene graph, manages the final transformation matrix of each node,
 * and interacts with a notification system that listens for matrix changes.
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
#include <memory>
#include <list>

// Project-specific includes
#include "node.h"
#include "notificationService.h"
#include "changeMatrixListener.h"

/**
 * @brief Elemento di rendering: nodo + world-matrix.
 */
struct RenderItem {
    Node* node;       // puntatore al nodo
    glm::mat4    matrix;     // trasformazione finale
};

/////////////////////
// MAIN LIST CLASS //
/////////////////////

/**
 * @brief Base List class
 *
 * The List class is responsible for managing a collection of `Node` objects for rendering purposes.
 * It stores the nodes and their transformation matrices, renders them, and listens for any changes
 * in the transformation matrices of the nodes.
 */
class ENG_API List : public ChangeMatrixListener {
public:
    /**
     * @brief Default constructor of the List class.
     */
    List();

    /**
     * @brief Destructor of the List class.
     * Frees memory by resetting the list and unsubscribing from the notification service.
     */
    ~List();

    /**
     * @brief Copy constructor is deleted to prevent copying the List object.
     */
    List(const List&) = delete;

    /**
     * @brief Assignment operator is deleted to prevent assignment of List objects.
     */
    void operator=(const List&) = delete;

    /**
     * @brief Passes a root node to be added to the list for rendering.
     *
     * This method recursively traverses the scene graph and adds nodes to the rendering list.
     *
     * @param rootNode The root node of the scene graph to traverse.
     */
    void pass(const Node* rootNode);

    /**
     * @brief Clears the list and frees all reserved memory.
     */
    void clearList();

    /**
     * @brief Renders all elements in the list.
     *
     * This method iterates through all nodes in the list and renders them using OpenGL.
     *
     * @param cameraInverseFinalMatrix The inverse transformation matrix of the camera.
     */
    void renderElements(const glm::mat4& cameraInverseFinalMatrixr) const;

    void renderElements(const glm::mat4& cameraInverseFinalMatrixr, const glm::vec4 planes[6]) const;

    /**
     * @brief Gets the number of elements in the list.
     *
     * @return The number of elements currently in the list.
     */
    const unsigned int getNumberOfElementsInList() const;

    /**
         * @brief Ritorna una lista di RenderItem pronti per il rendering.
         * @note Viene restituita una copia perché incapsuliamo la struttura interna.
         */
    std::vector<RenderItem> getRenderItems() const;

private:
    /**
     * @brief Helper structure that holds a node and its final transformation matrix.
     */
    struct Reserved;

    /**
     * @brief Adds a node to the list of nodes to render.
     *
     * This method adds the node to the list, and handles special cases for lights and cameras.
     *
     * @param node The node to add to the render list.
     */
    void addRowToListOfNodeToRender(Node* node);

    /**
     * @brief Resets the list and frees all memory used for reserved nodes.
     */
    void resetListAndFreeMemory();

    /**
     * @brief Callback for matrix changes. Updates the final transformation matrix of the changed node.
     *
     * This method is called when a node's matrix has changed. It updates the matrix for the corresponding reserved entry.
     *
     * @param nodeChangedID The ID of the node whose matrix has changed.
     */
    void onMatrixChange(const unsigned int& nodeChangedID) override;

    /**
     * @brief Gets the reserved entry for a node by its ID.
     *
     * @param id The ID of the node to find.
     * @return A pointer to the reserved entry for the node, or `nullptr` if not found.
     */
    Reserved* getReservedById(const unsigned int& id);

    /**
     * @brief Unique pointer to the reserved structure for managing node transformation data.
     */
    std::unique_ptr<Reserved> m_reserved;

    /**
     * @brief List of reserved nodes to be rendered.
     */
    std::list<Reserved*> m_listOfReservedToRender;

    /**
     * @brief Notification service for subscribing and listening to matrix changes.
     */
    NotificationService& m_notificationService;
};
