#include "mesh.h"
#include <map>

#include <GL/glew.h>
#include "GL/freeglut.h"


struct Mesh::Reserved {
    struct Vertex;
    struct Vao;
    struct Vbo;
    std::vector<std::vector<Reserved::Vertex>> m_faces;
    Mesh::Reserved::Vao vao;

    Reserved() : m_faces{ std::vector<std::vector<Mesh::Reserved::Vertex>>() } {}
};


struct Mesh::Reserved::Vbo {
    unsigned int vbo;
    std::vector<Vertex> vbos;
};

struct Mesh::Reserved::Vao {
    unsigned int vao;
    std::vector<Vbo> vbos;

};

struct Mesh::Reserved::Vertex {
    glm::vec3 v_coords;
    glm::vec3 v_normal;
    glm::vec2 v_textureUV;

    Vertex() = default;

    Vertex(const glm::vec3& coords, const glm::vec3& normal, const glm::vec2& textureUV)
        :   v_coords{coords}, v_normal{normal}, v_textureUV{textureUV} {}
};

struct PhysProps
{
    // Pay attention to 16 byte alignement (use padding):
    unsigned char type;
    unsigned char contCollisionDetection;
    unsigned char collideWithRBodies;
    unsigned char hullType;

    // Vector data:
    glm::vec3 massCenter;

    // Mesh properties:
    float mass;
    float staticFriction;
    float dynamicFriction;
    float bounciness;
    float linearDamping;
    float angularDamping;
    unsigned int nrOfHulls;
    unsigned int _pad;

    // Pointers:
    void* physObj;
    void* hull;
};

ENG_API Mesh::Mesh(const std::string& name)
    : Node(name), m_reserved{ std::make_unique<Mesh::Reserved>() } {}

ENG_API Mesh::~Mesh() = default;

ENG_API Mesh::Mesh(const Mesh& other) : Node(other), m_reserved{ std::make_unique<Mesh::Reserved>() } {
    this->m_reserved->m_faces = other.m_reserved->m_faces;
}

ENG_API Node* Mesh::clone() const {
    Mesh* newMesh = new Mesh(*this);
    this->recursiveClone(newMesh);
    return newMesh;
}

ENG_API std::vector<glm::vec3> Mesh::getVertices() {
    std::vector<glm::vec3> vertices;

    // Itera su tutte le facce
    for (const auto& face : this->m_reserved->m_faces) {
        // Itera su tutti i vertici della faccia
        for (const auto& vertex : face) {
            vertices.push_back(vertex.v_coords);
        }
    }

    return vertices;
}

void ENG_API Mesh::render(const glm::mat4& matrix) {
	//GLLOAD MATRIX
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(matrix));
    

    if (this->m_material != nullptr) {
        this->m_material->setEnableTexture();

        if (this->getMaterial()->getAlpha() < 1.0f) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else if (!this->isEnableLighting()) {
            glDisable(GL_LIGHTING);
            glColor4f(this->getMaterial()->getAmbient().x, this->getMaterial()->getAmbient().y, this->getMaterial()->getAmbient().z, this->getMaterial()->getAlpha());
        }
        else {
            this->m_material->render();
        }
    }

    ////
    glBegin(GL_TRIANGLES);
    for (const auto& face : m_reserved->m_faces) 
        for (const auto& vertex : face) {
            glNormal3fv(glm::value_ptr(vertex.v_normal));
            if(this->m_material->isTextureExists())
                glTexCoord2fv(glm::value_ptr(vertex.v_textureUV));
            glVertex3fv(glm::value_ptr(vertex.v_coords));
        }
    glEnd();
    ////

    if (this->m_material != nullptr) {
        this->m_material->setDisableTexture();
    }

    if (this->getMaterial()->getAlpha() < 1.0f)
        glDisable(GL_BLEND);
        
    if (!this->isEnableLighting())
        glEnable(GL_LIGHTING);
}

const ENG_API unsigned int Mesh::parse(const char* data, unsigned int& position) {
	const unsigned int& children = Node::parse(data, position);

	//subtype
	position += sizeof(unsigned char);

	char materialName[FILENAME_MAX];
	strncpy(materialName, data + position, sizeof(materialName) - 1);
	position += (unsigned int)strlen(materialName) + 1;

    Material* materialOfMesh = strcmp(materialName, "[none]") ? new Material(materialName) : nullptr;

    this->setMaterial(materialOfMesh);

	//Radius
	position += sizeof(float);

	// Mesh bounding box minimum corner:
	position += sizeof(glm::vec3);

	// Mesh bounding box maximum corner:
	position += sizeof(glm::vec3);

	// Optional physics properties:
    unsigned char hasPhysics = '0';
    memcpy(&hasPhysics, data + position, sizeof(unsigned char));
    position += sizeof(unsigned char);

    if (hasPhysics) position += sizeof(PhysProps);


	// Nr. of LODs:
	position += sizeof(unsigned int);

	// Nr. of vertices:
	unsigned int vertices, faces;
	memcpy(&vertices, data + position, sizeof(unsigned int));
	position += sizeof(unsigned int);

    // ...and faces:
    memcpy(&faces, data + position, sizeof(unsigned int));
    position += sizeof(unsigned int);

    std::vector<Reserved::Vertex> m_vertices;

    for (unsigned int c = 0; c < vertices; c++)
    {
        // Vertex coords:
        glm::vec3 vertex;
        memcpy(&vertex, data + position, sizeof(glm::vec3));
        position += sizeof(glm::vec3);

        // Vertex normal:
        unsigned int normalData;
        memcpy(&normalData, data + position, sizeof(unsigned int));
        glm::vec3 normal = glm::unpackSnorm3x10_1x2(normalData);
        position += sizeof(unsigned int);

        // Texture coordinates:
        unsigned int textureData;
        memcpy(&textureData, data + position, sizeof(unsigned int));
        glm::vec2 uv = glm::unpackHalf2x16(textureData);
        position += sizeof(unsigned int);

        // Tangent vector:
        /*unsigned int tangentData;
        memcpy(&tangentData, data + position, sizeof(unsigned int));
        glm::vec4 tangent = glm::unpackSnorm3x10_1x2(tangentData);*/
        position += sizeof(unsigned int);

        m_vertices.push_back(Reserved::Vertex(vertex, normal, uv));
     }

    // Faces:
    for (unsigned int c = 0; c < faces; c++)
    {
        // Face indexes:
        unsigned int face[3];
        memcpy(face, data + position, sizeof(unsigned int) * 3);
        position += sizeof(unsigned int) * 3;

        this->m_reserved->m_faces.push_back(std::vector<Reserved::Vertex>());
        this->m_reserved->m_faces[c].reserve(3);

        for (unsigned int i = 0; i < (sizeof(face) / sizeof(unsigned int)); i++)
            this->m_reserved->m_faces[c].push_back(m_vertices[face[i]]);
    }

    glGenBuffers(1, this->m_reserved);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVbo);

	return children;
}
