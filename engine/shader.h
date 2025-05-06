/**
 * @file		shader.h
 * @brief	Shader class header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 */
#pragma once

// GLM:
#include <glm/glm.hpp>

// C/C++:
#include <iostream>
#include "object.h"

/**
 * @brief OpenGL shader management class.
 * 
 * This class provides functionality for loading, compiling, and managing OpenGL shaders.
 * It supports vertex and fragment shaders, and handles shader program creation and uniform variable management.
 */
class ENG_API Shader
{
public:
    /**
     * @brief Maximum size for shader log messages.
     */
    static const unsigned int MAX_LOGSIZE = 4096;

    /**
     * @brief Enumeration for shader types.
     */
    enum
    {
        TYPE_UNDEFINED = 0,  ///< Undefined shader type
        TYPE_VERTEX,         ///< Vertex shader
        TYPE_FRAGMENT,       ///< Fragment shader
        TYPE_PROGRAM,        ///< Shader program
        TYPE_LAST           ///< Terminator
    };

    /**
     * @brief Constructor for the Shader class.
     * 
     * Initializes the shader type as undefined and the OpenGL ID to 0.
     */
    Shader();

    /**
     * @brief Destructor for the Shader class.
     * 
     * Cleans up OpenGL resources. Must be called when the OpenGL context is available.
     */
    ~Shader();

    /**
     * @brief Gets the location of a uniform variable in the shader.
     * 
     * @param name The name of the uniform variable.
     * @return The location ID of the uniform variable, or -1 if not found.
     */
    int getParamLocation(const char* name);

    /**
     * @brief Sets a 4x4 matrix uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param mat The matrix value to set.
     */
    void setMatrix(int param, const glm::mat4& mat);

    /**
     * @brief Sets a float uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param value The float value to set.
     */
    void setFloat(int param, float value);

    /**
     * @brief Sets an integer uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param value The integer value to set.
     */
    void setInt(int param, int value);

    /**
     * @brief Sets a 3D vector uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param vect The vector value to set.
     */
    void setVec3(int param, const glm::vec3& vect);

    /**
     * @brief Sets a 4D vector uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param vect The vector value to set.
     */
    void setVec4(int param, const glm::vec4& vect);

    /**
     * @brief Loads and compiles a shader from source code in memory.
     * 
     * @param kind The type of shader (vertex or fragment).
     * @param data Pointer to the shader source code.
     * @return true if loading and compilation was successful, false otherwise.
     */
    bool loadFromMemory(int kind, const char* data);

    /**
     * @brief Loads and compiles a shader from a file.
     * 
     * @param kind The type of shader (vertex or fragment).
     * @param filename Path to the shader source file.
     * @return true if loading and compilation was successful, false otherwise.
     */
    bool loadFromFile(int kind, const char* filename);

    /**
     * @brief Builds a shader program from vertex and fragment shaders.
     * 
     * @param vertexShader Pointer to the vertex shader, or nullptr if not used.
     * @param fragmentShader Pointer to the fragment shader, or nullptr if not used.
     * @return true if program creation was successful, false otherwise.
     */
    bool build(Shader* vertexShader, Shader* fragmentShader);

    /**
     * @brief Binds a vertex attribute to a specific location.
     * 
     * @param location The attribute location to bind to.
     * @param attribName The name of the attribute in the shader.
     */
    void bind(int location, const char* attribName);

    /**
     * @brief Activates the shader program for rendering.
     * 
     * @param data Optional pointer to custom data (unused in current implementation).
     * @return true if activation was successful, false otherwise.
     */
    bool render(void* data = nullptr);

    /**
     * @brief Sets a 3x3 matrix uniform variable in the shader.
     * 
     * @param param The location of the uniform variable.
     * @param mat The matrix value to set.
     */
    void setMatrix3(int param, const glm::mat3& mat);

    /**
     * @brief Gets the currently active shader program.
     * 
     * @return A pointer to the currently active shader program, or nullptr if none is active.
     */
    static Shader* getCurrent();

private:
    int type;                ///< The type of shader (vertex, fragment, or program)
    unsigned int glId;       ///< OpenGL identifier for the shader or program
    static Shader* current;  ///< Pointer to the currently active shader program
};