// Glew (include it before GL.h):
#include <GL/glew.h>
#include "fbo.h"


Fbo::Fbo()
{
    memset(glRenderBufferId, 0, sizeof(unsigned int) * MAX_ATTACHMENTS);
    for (unsigned int c = 0; c < Fbo::MAX_ATTACHMENTS; c++)
    {
        texture[c] = 0;
        drawBuffer[c] = -1;
    }
    nrOfMrts = 0;
    mrt = nullptr;
    glGenFramebuffers(1, &glId);
}

Fbo::~Fbo()
{
    if (mrt)
        delete[] mrt;
    for (unsigned int c = 0; c < Fbo::MAX_ATTACHMENTS; c++)
        if (glRenderBufferId[c])
            glDeleteRenderbuffers(1, &glRenderBufferId[c]);
    glDeleteFramebuffers(1, &glId);
}

ENG_API unsigned int Fbo::getTexture(unsigned int textureNumber)
{
    if (textureNumber < Fbo::MAX_ATTACHMENTS)
        return texture[textureNumber];
    else
        return 0;
}

ENG_API int Fbo::getSizeX()
{
    return sizeX;
}

ENG_API int Fbo::getSizeY()
{
    return sizeY;
}

ENG_API int Fbo::getSizeZ()
{
    return sizeZ;
}

ENG_API unsigned int Fbo::getHandle()
{
    return glId;
}

ENG_API bool Fbo::isOk()
{
    render();
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[ERROR] FBO not complete (error: " << status << ")" << std::endl;
        return false;
    }
    return true;
}

ENG_API bool Fbo::bindTexture(unsigned int textureNumber, unsigned int operation, unsigned int texture, int param1, int param2)
{
    if (textureNumber >= Fbo::MAX_ATTACHMENTS)
    {
        std::cout << "[ERROR] Invalid params" << std::endl;
        return false;
    }
    render();
    switch (operation)
    {
    case BIND_COLORTEXTURE:
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + param1, GL_TEXTURE_2D, texture, 0);
        drawBuffer[textureNumber] = param1;
        break;
    case BIND_DEPTHTEXTURE:
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        break;
    default:
        std::cout << "[ERROR] Invalid operation" << std::endl;
        return false;
    }
    this->texture[textureNumber] = texture;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sizeX);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &sizeY);
    return updateMrtCache();
}

ENG_API bool Fbo::bindRenderBuffer(unsigned int renderBuffer, unsigned int operation, int sizeX, int sizeY)
{
    if (renderBuffer >= Fbo::MAX_ATTACHMENTS)
    {
        std::cout << "[ERROR] Invalid params" << std::endl;
        return false;
    }
    render();
    if (glRenderBufferId[renderBuffer])
        glDeleteRenderbuffers(1, &glRenderBufferId[renderBuffer]);
    glGenRenderbuffers(1, &glRenderBufferId[renderBuffer]);
    glBindRenderbuffer(GL_RENDERBUFFER, glRenderBufferId[renderBuffer]);
    switch (operation)
    {
    case BIND_DEPTHBUFFER:
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, sizeX, sizeY);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glRenderBufferId[renderBuffer]);
        break;
    default:
        std::cout << "[ERROR] Invalid operation" << std::endl;
        return false;
    }
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    return updateMrtCache();
}

ENG_API bool Fbo::updateMrtCache()
{
    if (mrt)
        delete[] mrt;
    nrOfMrts = 0;
    for (unsigned int c = 0; c < Fbo::MAX_ATTACHMENTS; c++)
        if (drawBuffer[c] != -1)
            nrOfMrts++;
    if (nrOfMrts)
    {
        mrt = new GLenum[nrOfMrts];
        int bufferPosition = 0;
        for (int c = 0; c < Fbo::MAX_ATTACHMENTS; c++)
            if (drawBuffer[c] != -1)
            {
                mrt[bufferPosition] = GL_COLOR_ATTACHMENT0 + drawBuffer[c];
                bufferPosition++;
            }
    }
    return true;
}

ENG_API void Fbo::disable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ENG_API bool Fbo::render(void* data)
{
    glBindFramebuffer(GL_FRAMEBUFFER, glId);
    if (nrOfMrts)
    {
        glDrawBuffers(nrOfMrts, mrt);
        glViewport(0, 0, sizeX, sizeY);
    }
    return true;
}


