/// shader.h
#pragma once

// GLM:
#include <glm/glm.hpp>

// C/C++:
#include <iostream>
#include "object.h"

class ENG_API Shader
{
public:
    static const unsigned int MAX_LOGSIZE = 4096;

    enum
    {
        TYPE_UNDEFINED = 0,
        TYPE_VERTEX,
        TYPE_FRAGMENT,
        TYPE_PROGRAM,
        TYPE_LAST
    };

    Shader();
    ~Shader();

    int getParamLocation(const char* name);
    void setMatrix(int param, const glm::mat4& mat);
    void setFloat(int param, float value);
    void setInt(int param, int value);
    void setVec3(int param, const glm::vec3& vect);
    void setVec4(int param, const glm::vec4& vect);

    bool loadFromMemory(int kind, const char* data);
    bool loadFromFile(int kind, const char* filename);
    bool build(Shader* vertexShader, Shader* fragmentShader);
    void bind(int location, const char* attribName);

    bool render(void* data = nullptr);
    void setMatrix3(int param, const glm::mat3& mat);
    static Shader* getCurrent();

private:
    int type;
    unsigned int glId;
    static Shader* current;

};