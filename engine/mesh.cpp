#include "mesh.h"
#include <map>

#include "GL/glew.h"
#include "GL/freeglut.h"


struct Mesh::Reserved {
    struct Vertex;
    std::vector<std::vector<Reserved::Vertex>> m_faces;

    Reserved() : m_faces{ std::vector<std::vector<Mesh::Reserved::Vertex>>() } {}
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
    : Node(name), m_reserved{ std::make_unique<Mesh::Reserved>() }
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &normalVerticesVbo);
    glGenBuffers(1, &uvVerticesVbo);
    glGenBuffers(1, &verticesVbo);
    glGenBuffers(1, &faceVbo);
}

ENG_API Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &normalVerticesVbo);
    glDeleteBuffers(1, &uvVerticesVbo);
    glDeleteBuffers(1, &verticesVbo);
    glDeleteBuffers(1, &faceVbo);
};

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


void Mesh::setupMesh() {
    glBindVertexArray(VAO);

    // Posizioni (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
    glBufferData(GL_ARRAY_BUFFER, coorVertices.size() * sizeof(glm::vec3), coorVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    // Normali (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, normalVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, normalVertices.size() * sizeof(glm::vec3), normalVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, nullptr);


    // UV (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, uvVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, uvVertices.size() * sizeof(glm::vec2), uvVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(unsigned int), faceIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO
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
    
    
    /*
     *OLD VERSION with OpenGL 1.0
     *
	    glBegin(GL_TRIANGLES);
	    for (const auto& face : m_reserved->m_faces) 
	        for (const auto& vertex : face) {
	            glNormal3fv(glm::value_ptr(vertex.v_normal));
	            if(this->m_material->isTextureExists())
	                glTexCoord2fv(glm::value_ptr(vertex.v_textureUV));
	            glVertex3fv(glm::value_ptr(vertex.v_coords));
	        }
	    glEnd();
    */

	// Render con VAO e glDrawElements()
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

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
        coorVertices.push_back(vertex);

        // Vertex normal:
        unsigned int normalData;
        memcpy(&normalData, data + position, sizeof(unsigned int));
        glm::vec3 normal = glm::unpackSnorm3x10_1x2(normalData);
        position += sizeof(unsigned int);
        normalVertices.push_back(normal);

        // Texture coordinates:
        unsigned int textureData;
        memcpy(&textureData, data + position, sizeof(unsigned int));
        glm::vec2 uv = glm::unpackHalf2x16(textureData);
        position += sizeof(unsigned int);
        uvVertices.push_back(uv);

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

        //faceIndices.push_back(face[0]);
        //faceIndices.push_back(face[1]);
        //faceIndices.push_back(face[2]);

        this->m_reserved->m_faces.push_back(std::vector<Reserved::Vertex>());
        this->m_reserved->m_faces[c].reserve(3);

        for (unsigned int i = 0; i < 3; i++)
        {
            this->m_reserved->m_faces[c].push_back(m_vertices[face[i]]);
            faceIndices.push_back(face[i]);
        }
    }

    this->setupMesh();

	return children;
}
