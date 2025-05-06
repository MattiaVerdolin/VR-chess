/**
 * @file		fbo.h
 * @brief	Framebuffer Object header file
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

#include "object.h"


///////////////
// CLASS Fbo //
///////////////

/**
 * @brief Frame buffer class to deal with OpenGL FBOs.
 */
class ENG_API Fbo
{
	//////////
public: //
	//////////

	// Enums:
	/**
	 * @brief Enumeration for specifying which eye to render for stereo rendering.
	 */
	enum Eye
	{
		EYE_LEFT = 0,  ///< Left eye rendering
		EYE_RIGHT = 1, ///< Right eye rendering

		// Terminator:
		EYE_LAST,      ///< Last eye index
	};

	   // Constants:
	static const unsigned int MAX_ATTACHMENTS = 8; ///< Maximum number of available render buffers or textures per FBO	

	// Enumerations:
	/**
	 * @brief Enumeration for specifying the type of binding operation.
	 */
	enum : unsigned int ///< Kind of operation
	{
		BIND_DEPTHBUFFER = 0,  ///< Bind depth buffer
		BIND_COLORTEXTURE,     ///< Bind color texture
		BIND_DEPTHTEXTURE,     ///< Bind depth texture
	};

	// Const/dest:	 
	/**
	 * @brief Constructor for the FBO class.
	 * 
	 * Initializes a new framebuffer object with default values.
	 */
	Fbo();

	/**
	 * @brief Destructor for the FBO class.
	 * 
	 * Cleans up any resources associated with the framebuffer object.
	 */
	~Fbo();

	// Get/set:   
	/**
	 * @brief Get the texture ID for a specific texture number.
	 * 
	 * @param textureNumber The index of the texture to retrieve.
	 * @return The OpenGL texture ID for the specified texture.
	 */
	unsigned int getTexture(unsigned int textureNumber);

	/**
	 * @brief Get the width of the framebuffer.
	 * 
	 * @return The width in pixels.
	 */
	int getSizeX();

	/**
	 * @brief Get the height of the framebuffer.
	 * 
	 * @return The height in pixels.
	 */
	int getSizeY();

	/**
	 * @brief Get the depth of the framebuffer.
	 * 
	 * @return The depth in pixels.
	 */
	int getSizeZ();

	/**
	 * @brief Get the OpenGL handle for this framebuffer object.
	 * 
	 * @return The OpenGL framebuffer object ID.
	 */
	unsigned int getHandle();


	// Management:
	/**
	 * @brief Check if the framebuffer object is valid and ready for use.
	 * 
	 * @return True if the framebuffer is valid, false otherwise.
	 */
	bool isOk();

	/**
	 * @brief Bind a texture to the framebuffer object.
	 * 
	 * @param textureNumber The index of the texture attachment point.
	 * @param operation The type of binding operation (BIND_DEPTHBUFFER, BIND_COLORTEXTURE, or BIND_DEPTHTEXTURE).
	 * @param texture The OpenGL texture ID to bind.
	 * @param param1 Optional parameter for the binding operation.
	 * @param param2 Optional parameter for the binding operation.
	 * @return True if the binding was successful, false otherwise.
	 */
	bool bindTexture(unsigned int textureNumber, unsigned int operation, unsigned int texture, int param1 = 0, int param2 = 0);

	/**
	 * @brief Bind a renderbuffer to the framebuffer object.
	 * 
	 * @param renderBuffer The index of the renderbuffer attachment point.
	 * @param operation The type of binding operation.
	 * @param sizeX The width of the renderbuffer.
	 * @param sizeY The height of the renderbuffer.
	 * @return True if the binding was successful, false otherwise.
	 */
	bool bindRenderBuffer(unsigned int renderBuffer, unsigned int operation, int sizeX, int sizeY);

	// Rendering:     
	/**
	 * @brief Render the framebuffer contents.
	 * 
	 * @param data Optional pointer to additional data needed for rendering.
	 * @return True if rendering was successful, false otherwise.
	 */
	bool render(void* data = nullptr);

	/**
	 * @brief Disable the current framebuffer object.
	 * 
	 * This static method unbinds the current framebuffer and returns to the default framebuffer.
	 */
	static void disable();



	///////////
private: //
	///////////

	// Generic data:
	int sizeX, sizeY, sizeZ;	         				///< FBO width, height and depth
	unsigned int texture[MAX_ATTACHMENTS];             ///< Attached textures
	int drawBuffer[MAX_ATTACHMENTS];       		      ///< Set color attachment per texture

	// OGL stuff:
	unsigned int glId;                                 ///< OpenGL ID
	unsigned int glRenderBufferId[MAX_ATTACHMENTS];    ///< Render buffer IDs

	// MRT cache:   
	int nrOfMrts;                                      ///< Number of MRTs
	unsigned int* mrt;                                 ///< Cached list of buffers 

	// Cache:
	/**
	 * @brief Update the Multiple Render Target (MRT) cache.
	 * 
	 * @return True if the cache was successfully updated, false otherwise.
	 */
	bool updateMrtCache();
};