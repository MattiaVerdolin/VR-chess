/**
 * @file    notificationservice.h
 * @brief   Declaration of the NotificationService class for managing listeners and notifications.
 *
 * This file defines the NotificationService class, which allows subscribing and unsubscribing listeners
 * for window reshape and matrix change events. It also provides methods to notify all subscribers of these events.
 *
 * @authors Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 *          Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 *          Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch] */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific includes
#include "reshapeListener.h"
#include "changeMatrixListener.h"

// Standard libraries
#include <vector>
#include <algorithm>

#include "shader.h"

////////////////////////////////
// NOTIFICATION SERVICE CLASS //
////////////////////////////////

/**
 * @class NotificationService
 * @brief Singleton class for managing event listeners and notifying them of events.
 *
 * The NotificationService class provides functionality to subscribe and unsubscribe listeners to window
 * reshape and matrix change events. It ensures that when an event occurs, all subscribed listeners are notified.
 */
class ENG_API NotificationService {
public:
    /**
     * @brief Deleted copy constructor to prevent copying of the NotificationService instance.
     */
    NotificationService(const NotificationService&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent assignment of the NotificationService instance.
     * @return A reference to the NotificationService instance.
     */
    NotificationService& operator=(const NotificationService&) = delete;

    /**
     * @brief Destructor for the NotificationService class.
     *
     * Cleans up the resources and ensures proper cleanup of listener subscriptions.
     */
    ~NotificationService();

    /**
     * @brief Get the singleton instance of NotificationService.
     * @return The singleton instance of NotificationService.
     */
    static NotificationService& getInstance();

    /**
     * @brief Subscribe a listener for window reshape events.
     * @param listener The ReshapeListener to subscribe.
     */
    void subscribeListener(ReshapeListener* listener);

    /**
     * @brief Subscribe a listener for matrix change events.
     * @param listener The ChangeMatrixListener to subscribe.
     */
    void subscribeListener(ChangeMatrixListener* listener);

    /**
     * @brief Unsubscribe a listener from window reshape events.
     * @param listener The ReshapeListener to unsubscribe.
     */
    void unsubscribeListener(ReshapeListener* listener);

    /**
     * @brief Unsubscribe a listener from matrix change events.
     * @param listener The ChangeMatrixListener to unsubscribe.
     */
    void unsubscribeListener(ChangeMatrixListener* listener);

    /**
     * @brief Notify all subscribed ReshapeListeners of a window reshape event.
     *
     * This method calls the onWindowReshape method on all subscribed ReshapeListener instances,
     * passing the new window dimensions.
     *
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void notifyOnReshapeWindow(int width, int height, Shader* shader) const;

    /**
     * @brief Notify all subscribed ChangeMatrixListeners of a matrix change event.
     *
     * This method calls the onMatrixChange method on all subscribed ChangeMatrixListener instances,
     * passing the matrix ID that has changed.
     *
     * @param matrixId The ID of the matrix that was changed.
     */
    void notifyOnChangeMatrix(const unsigned int& matrixId) const;

private:
    /**
     * @brief Private constructor for the singleton instance.
     *
     * This constructor is private to ensure that the class can only be instantiated once
     * through the getInstance method.
     */
    NotificationService();

    /// List of subscribers to window reshape events.
    std::vector<ReshapeListener*> m_reshapeListeners;

    /// List of subscribers to matrix change events.
    std::vector<ChangeMatrixListener*> m_changeMatrixListeners;
};
