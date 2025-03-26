#include "list.h"
#include "light.h"
#include "camera.h"
#include <GL/freeglut.h>

#include "mesh.h"

struct ENG_API List::Reserved {
	Object* r_node;
	glm::mat4 r_nodeFinalMatrix;

	~Reserved() = default;
	Reserved(Object* node, const glm::mat4& finalMatrix) : r_node{node}, r_nodeFinalMatrix{finalMatrix} 
	{}
};

ENG_API List::List() : m_reserved{ std::unique_ptr<List::Reserved>() }, m_listOfReservedToRender{ std::list<List::Reserved*>() }, m_notificationService{ NotificationService::getInstance() } {
	this->m_notificationService.subscribeListener(this);
}

ENG_API List::~List() {
	this->resetListAndFreeMemory();
	this->m_notificationService.unsubscribeListener(this);
}

void ENG_API List::pass(const Node* rootNode) {
	if (rootNode == nullptr) return;
	for (auto* node : rootNode->getChildren()) {
		this->addRowToListOfNodeToRender(node);
		this->pass(node);
	}
}

void ENG_API List::addRowToListOfNodeToRender(Node* node) {
	if (node == nullptr || dynamic_cast<Camera*>(node) != nullptr) return;

	List::Reserved* reservedRow = new List::Reserved(node, node->getFinalMatrix());
	Light* optionalLight = dynamic_cast<Light*>(node);
	if (optionalLight != nullptr)
		this->m_listOfReservedToRender.push_front(reservedRow);
	else
		this->m_listOfReservedToRender.push_back(reservedRow);
}

void ENG_API List::clearList() {
	this->resetListAndFreeMemory();
}

void ENG_API List::renderElements(const glm::mat4& cameraInverseFinalMatrix, Shader* shader, int mvLoc, int normalMatLoc, int matEmissionLoc, int matAmbientLoc, int matDiffuseLoc, int matSpecularLoc, int matShininessLoc) const {

	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	for (const auto* reservedRow : this->m_listOfReservedToRender) {

		glm::mat4 f = cameraInverseFinalMatrix * reservedRow->r_nodeFinalMatrix;

		if (dynamic_cast<Mesh*>(reservedRow->r_node) != nullptr){
			shader->setMatrix(mvLoc, f);
			shader->setMatrix3(normalMatLoc, glm::transpose(glm::inverse(f)));
			dynamic_cast<Mesh*>(reservedRow->r_node)->setupParameters(shader, matEmissionLoc, matAmbientLoc, matDiffuseLoc, matSpecularLoc, matShininessLoc);
		} else if (dynamic_cast<Light*>(reservedRow->r_node) != nullptr)
		{
			// stessa cosa sopra
		}

		reservedRow->r_node->render(f);
	}
		
	Light::resetLightCounter();
}

const ENG_API unsigned int List::getNumberOfElementsInList() const {
	return (unsigned int) this->m_listOfReservedToRender.size();
}

void ENG_API List::resetListAndFreeMemory() {
	for (auto* element : this->m_listOfReservedToRender)
		delete element;
	this->m_listOfReservedToRender.clear();
}

void ENG_API List::onMatrixChange(const unsigned int& nodeChangedID) {
	List::Reserved* changedReserved = this->getReservedById(nodeChangedID);
	if (changedReserved == nullptr)
		return;
	
	changedReserved->r_nodeFinalMatrix = dynamic_cast<Node*>(changedReserved->r_node)->getFinalMatrix();
}

ENG_API List::Reserved* List::getReservedById(const unsigned int& id) {
	for (auto* reserved : this->m_listOfReservedToRender)
		if (reserved->r_node->getId() == id)
			return reserved;
	return nullptr;
}