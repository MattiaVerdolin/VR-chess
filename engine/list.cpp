#include "list.h"
#include "light.h"
#include "directLight.h"
#include "spotLight.h"
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

		if(!dynamic_cast<DirectLight*>(node) && !dynamic_cast<SpotLight*>(node))
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

void ENG_API List::renderElements(const glm::mat4& cameraInverseFinalMatrix) const {
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	for (const auto* r : m_listOfReservedToRender)
		r->r_node->render(cameraInverseFinalMatrix * r->r_nodeFinalMatrix);
	Light::resetLightCounter();
}


void ENG_API List::renderElements(const glm::mat4& cameraInverseFinalMatrix, const glm::vec4 planes[6]) const
{
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

	for (const auto* r : m_listOfReservedToRender)
	{
		Node* node = dynamic_cast<Node*>(r->r_node);
		glm::mat4 modelMat = r->r_nodeFinalMatrix;
		glm::mat4 mvp = cameraInverseFinalMatrix * modelMat;

		// Se è una Mesh, facciamo il test sphere-frustum
		if (auto mesh = dynamic_cast<Mesh*>(node)) {
			// trasforma centro modello -> view space
			glm::vec3 centerVS = glm::vec3(mvp * glm::vec4(mesh->getBoundingCenter(), 1.0f));
			float radius = mesh->getBoundingRadius();
			bool inside = true;
			for (int i = 0; i < 6; ++i) {
				float d = glm::dot(glm::vec3(planes[i]), centerVS) + planes[i].w;
				if (d < -radius) { inside = false; break; }
			}
			if (!inside)
				continue;   // skip rendering
		}

		node->render(mvp);
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

std::vector<RenderItem> ENG_API List::getRenderItems() const {
	std::vector<RenderItem> items;
	items.reserve(m_listOfReservedToRender.size());
	for (auto* r : m_listOfReservedToRender) {
		RenderItem elem;
		elem.node = dynamic_cast<Node*>(r->r_node);
		elem.matrix = r->r_nodeFinalMatrix;
		items.push_back(elem);
	}
	return items;
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