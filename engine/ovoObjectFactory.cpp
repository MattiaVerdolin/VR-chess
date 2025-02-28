#include "ovoObjectFactory.h"

#include "mesh.h"
#include "spotLight.h"
#include "directLight.h"
#include "omniLight.h"
#include "perspCamera.h"
#include "orthoCamera.h"
#include "material.h"

class ENG_API OvObject
{
public:
    enum class Type : int
    {
        // Foundation types:
        OBJECT = 0,
        NODE,
        OBJECT2D,
        OBJECT3D,
        LIST,

        // Derived classes:
        BUFFER,
        SHADER,
        TEXTURE,
        FILTER,
        MATERIAL,
        FBO,
        QUAD,
        BOX,
        SKYBOX,
        FONT,
        CAMERA,
        LIGHT,
        BONE,
        MESH,	   // Keep them...
        SKINNED, // ...consecutive        
        INSTANCED,
        PIPELINE,
        EMITTER,

        // Animation type
        ANIM,

        // Physics related:
        PHYSICS,

        // Terminator:
        LAST,
    };
};

class ENG_API OvLight
{
public:
    enum class Subtype : int ///< Kind of light
    {
        // Foundation types:
        OMNI = 0,
        DIRECTIONAL,
        SPOT,

        // Terminator:
        LAST,
    };
};



ENG_API Object* OVOObjectFactory::createObjectByChunkID(const unsigned int& chunkID, const char* data) {
    switch ((OvObject::Type)chunkID) {
    case OvObject::Type::NODE: return new Node();
    case OvObject::Type::MATERIAL: return new Material();
    case OvObject::Type::MESH: return new Mesh();
    case OvObject::Type::LIGHT: return createLight(skipToSubtype(data));
    default: return nullptr;
    }
}

const ENG_API unsigned char OVOObjectFactory::skipToSubtype(const char* data) {
    unsigned int position = 0;

    unsigned int nameLength = (unsigned int)strlen(data + position) + 1;
    position += nameLength;

    position += sizeof(glm::mat4);

    position += sizeof(unsigned int);

    unsigned int targetNameLength = (unsigned int)strlen(data + position) + 1;
    position += targetNameLength;

    unsigned char subtype;
    memcpy(&subtype, data + position, sizeof(unsigned char));
    position += sizeof(unsigned char);

    return subtype;
}

ENG_API Object* OVOObjectFactory::createLight(const unsigned char& subtype) {
    switch ((OvLight::Subtype)subtype)
    {
    case OvLight::Subtype::DIRECTIONAL: return new DirectLight();
    case OvLight::Subtype::OMNI: return new OmniLight();
    case OvLight::Subtype::SPOT: return new SpotLight();
    default: return nullptr;
    }
}