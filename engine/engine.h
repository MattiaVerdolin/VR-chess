/**
 * @file    engine.h
 * @brief   Main include file for the graphics engine.
 *
 * This file defines the main class for the graphics engine, `Base`, which follows the Singleton design pattern.
 * The class provides methods for initialization, scene management, and rendering. It also includes functions
 * for handling user input and managing FPS.
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
#include "camera.h"
#include "fbo.h"
#include "ovr.h"


/////////////
// VERSION //
/////////////

// Library versioning and debugging information
#ifdef _DEBUG
   #define LIB_NAME      "My Graphics Engine v0.1a (debug)"   ///< Library credits for debug version
#else
   #define LIB_NAME      "My Graphics Engine v0.1a"   ///< Library credits for release version
#endif
   #define LIB_VERSION   10                           ///< Library version (divide by 10)

   // Export API linkage based on platform
#ifdef _WINDOWS
   // Specifies I/O linkage for Windows (VC++ spec)
   #ifdef ENGINE_EXPORTS
      #define ENG_API __declspec(dllexport)
   #else
      #define ENG_API __declspec(dllimport)
   #endif      

   // Disable specific warnings for DLL export/import
   #pragma warning(disable : 4251) 
#else // For Linux
   #define ENG_API
#endif



///////////////
// NAMESPACE //
///////////////

namespace Eng {

//////////////
// #INCLUDE //
//////////////   

   // You can add more headers specific to the engine here

///////////////////////
// MAIN ENGINE CLASS //
///////////////////////

/**
 * @class Base
 * @brief Main class for the graphics engine. Implements the Singleton pattern.
 *
 * The `Base` class is responsible for initializing the engine, managing scenes, handling user input, 
 * and managing frame rate. It is designed as a singleton to ensure only one instance of the engine exists.
 */
class ENG_API Base final
{
//////////
public: //
//////////

   /**
    * @brief Deleted copy constructor to prevent copying of the Base instance.
    */
   Base(Base const &) = delete;

   /**
    * @brief Destructor for the Base class.
    *
    * Cleans up resources used by the graphics engine.
    */
   ~Base();

   /**
    * @brief Deleted copy assignment operator to prevent assignment of the Base instance.
    * @return A reference to the Base instance.
    */
   void operator=(Base const &) = delete;

   /**
    * @brief Get the singleton instance of the Base class.
    * @return The singleton instance of Base.
    */
   static Base &getInstance();

   /**
    * @brief Initialize the engine.
    * @return `true` if initialization is successful, `false` otherwise.
    */
   bool init(void (*closeCallBack)() = nullptr);

   /**
    * @brief Set the keyboard callback function for user input.
    * @param callback The callback function to handle keyboard events.
    */
   void setKeyboardCallback(void (*callback)(unsigned char, int, int));

   /**
    * @brief Set the special key callback function for user input.
    * @param callback The callback function to handle special key events.
    */
   void setSpecialCallback(void (*callback)(int, int, int));

   /**
    * @brief Free resources used by the engine.
    * @return `true` if the engine was successfully freed, `false` otherwise.
    */
   bool free();

   /**
    * @brief Clear the current scene.
    */
   void clear();

   /**
    * @brief Get the current frames per second (FPS) value.
    * @return The current FPS value.
    */
   const float& getFPS();

   /**
    * @brief Load a scene from a specified file.
    * @param filename The path to the scene file.
    * @return A pointer to the root node of the loaded scene.
    */
   Node* load(const std::string& filename);

   /**
    * @brief Pass a scene to the engine for rendering.
    * @param rootNode A pointer to the root node of the scene.
    */
   void passScene(Node* rootNode);

   /**
    * @brief Clear the current scene.
    */
   void clearScene();

   /**
    * @brief Start 3D rendering.
    * @param camera1 A pointer to the first camera.
    * @param camera2 A pointer to the second camera.
    * @param layers A list of layers to be rendered.
    */
   void begin3D(Camera* camera1, Camera* camera2, const std::list<std::string>& layers);

   /**
    * @brief End 3D rendering.
    */
   void end3D();

   /**
    * @brief Swap the buffers for rendering.
    */
   void swap();


private:

   /**
    * @brief Private method to handle window reshaping.
    * @param width The new width of the window.
    * @param height The new height of the window.
    */
   void handleReshape(int width, int height);

   Fbo* getCurrent(int numEye);

   // Reserved:
   static Eng::Base instance; ///< The singleton instance of the engine
   struct Reserved;           ///< Reserved structure for internal use
   std::unique_ptr<Reserved> reserved; ///< Unique pointer to the reserved structure

   /**
    * @brief Private constructor for the singleton instance.
    * Initializes internal resources for the engine.
    */
   Base();

   Shader* vs = nullptr;
   Shader* fs = nullptr;
   Shader* shader = nullptr;

   OvVR* ovr = nullptr;

   // Enums:
   enum Eye
   {
       EYE_LEFT = 0,
       EYE_RIGHT = 1,

       // Terminator:
       EYE_LAST,
   };

   unsigned int texId = 0;
   unsigned int fboTexId[EYE_LAST] = { 0, 0 };
   unsigned int fboWidth = 0;
   unsigned int fboHeight = 0;

   Fbo* fbo[EYE_LAST] = { nullptr, nullptr };

};

}; // end of namespace Eng::