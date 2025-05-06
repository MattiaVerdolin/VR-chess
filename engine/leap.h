/**
 * @file		leap.h
 * @brief	Leap Motion controller header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */
#pragma once



 //////////////
 // #INCLUDE //
 //////////////

	// GLM:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>   

// C/C++:
#include <iostream>

// Leap Motion SDK:
#include <LeapC.h>



////////////////
// CLASS Leap //
////////////////

/**
 * @brief Leap Motion wrapper.
 * 
 * This class provides a wrapper for the Leap Motion controller, handling connection,
 * initialization, and frame updates for hand tracking.
 */
class Leap
{
	//////////
public: //
	//////////

		/**
		 * @brief Constructor for the Leap Motion wrapper.
		 * 
		 * Initializes the connection, current frame, and last frame ID to default values.
		 */
	Leap();

	/**
	 * @brief Destructor for the Leap Motion wrapper.
	 * 
	 * Cleans up any resources associated with the Leap Motion controller.
	 */
	~Leap();

	/**
	 * @brief Initialize the Leap Motion controller connection.
	 * 
	 * This method establishes a connection to the Leap Motion controller and verifies
	 * that exactly one device is available. It handles the creation and opening of
	 * the connection, and retrieves a reference to the device.
	 * 
	 * @return True if initialization was successful, false otherwise.
	 * @note The method will output error messages to std::cout if initialization fails.
	 */
	bool init();

	/**
	 * @brief Release resources associated with the Leap Motion controller.
	 * 
	 * This method closes and destroys the connection to the Leap Motion controller,
	 * cleaning up all associated resources.
	 * 
	 * @return True if cleanup was successful, false otherwise.
	 */
	bool free();

	/**
	 * @brief Update the current frame data from the Leap Motion controller.
	 * 
	 * This method polls the Leap Motion controller for new tracking events and updates
	 * the current frame data. It handles frame skipping and lag detection.
	 * 
	 * @return True if the update was successful, false otherwise.
	 * @note The method will output warning messages to std::cout if significant lag is detected.
	 */
	bool update();

	/**
	 * @brief Get the current tracking frame data.
	 * 
	 * @return A pointer to the current LEAP_TRACKING_EVENT structure containing the latest
	 *         hand tracking data, or nullptr if no frame is available.
	 */
	const LEAP_TRACKING_EVENT* getCurFrame() const;


	///////////	 
private:	//
	///////////			

	   // Leap Motion:
	LEAP_CONNECTION connection;
	LEAP_DEVICE_REF leapDevice;
	LEAP_TRACKING_EVENT curFrame;
	signed long long lastFrameId;
};
