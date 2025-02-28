#include "node.h"
#include "camera.h"
#include <iostream>
#include <algorithm>

ENG_API Node::Node(const std::string& name) : Object(name), m_matrix{ glm::mat4(1.0f) }, m_parent{ nullptr }, m_children{ std::vector<Node*>() }, m_material{ nullptr }, m_enableLighting{ true }, m_notificationService{ NotificationService::getInstance() }, m_isMatrixParsed{ false } {}

ENG_API Node::Node(const Node& other) : Object(other), m_matrix{ other.m_matrix }, m_parent{ other.m_parent }, m_children{ std::vector<Node*>() }, m_material{ other.m_material == nullptr ? other.m_material : new Material(*other.m_material) }, m_enableLighting{ other.m_enableLighting }, m_notificationService{ NotificationService::getInstance() }, m_isMatrixParsed{ false } {}

ENG_API Node::~Node() {
	for (auto child : m_children)
		delete child;
	m_children.clear();
}

ENG_API Node* Node::clone() const {
	Node* newNode = new Node(*this);
	this->recursiveClone(newNode);

	return newNode;
}

ENG_API void Node::recursiveClone(Node* newNode) const {
	for (auto* child : this->m_children) {
		Node* clonedChild = child->clone();
		newNode->addChild(clonedChild);
	}
}

void ENG_API Node::setMatrix(const glm::mat4& matrix) {
	this->m_matrix = matrix;
	if (dynamic_cast<Camera*>(this) == nullptr) {
		if (this->m_isMatrixParsed)
			this->recursiveNotifyChangeMatrix();
		else
			this->m_isMatrixParsed = true;
	}	
}

void ENG_API Node::recursiveNotifyChangeMatrix() {
	this->m_notificationService.notifyOnChangeMatrix(this->getId());
	for (auto* child : this->getChildren())
		child->recursiveNotifyChangeMatrix();
}

const ENG_API glm::mat4& Node::getMatrix() const {
	return this->m_matrix;
}

const ENG_API glm::mat4 Node::getFinalMatrix() const {
	if (this->m_parent == nullptr)
		return this->m_matrix;
	return this->m_parent->getFinalMatrix() * this->m_matrix;
}

const ENG_API Node* Node::findNodeByName(const std::string& name) const {
	for (const auto* node : this->m_children)
		if (node->m_name == name)
			return node;

	for (const auto* node : this->m_children) {
		const Node* nodeByName = node->findNodeByName(name);
		if (nodeByName != nullptr)
			return nodeByName;
	}

	return nullptr;
}

const ENG_API unsigned int Node::parse(const char* data, unsigned int& position) {
	// Node name:
	char nodeName[FILENAME_MAX];
	strncpy(nodeName, data + position, sizeof(nodeName) - 1);
	this->setName(nodeName);

	position += (unsigned int)strlen(nodeName) + 1;

	// Node matrix:
	glm::mat4 matrix;
	memcpy(&matrix, data + position, sizeof(glm::mat4));
	this->setMatrix(matrix);
	position += sizeof(glm::mat4);

	// Nr. of children nodes:
	unsigned int children;
	memcpy(&children, data + position, sizeof(unsigned int));
	this->m_children.reserve(children);
	position += sizeof(unsigned int);

	//NON DOVREBBE SERVIRE
	char targetName[FILENAME_MAX];
	strncpy(targetName, data + position, sizeof(targetName) - 1);
	position += (unsigned int)strlen(targetName) + 1;

	return children;
}

ENG_API Node* Node::getMainCamera() const {
	for (auto* element : this->m_children) {
		Camera* camera = dynamic_cast<Camera*>(element);

		if (camera != nullptr) {
			if (camera->isMainCamera()) {
				return camera;
			}
		}
		else {
			if (element != nullptr) {
				Node* childCamera = element->getMainCamera();
				if (childCamera != nullptr)
					return childCamera;
			}
		}
	}
	return nullptr;
}

void ENG_API Node::setParent(Node* parent) {
	this->m_parent = parent;
}

const ENG_API Node* Node::getParent() const {
	return this->m_parent;
}

ENG_API bool Node::addChild(Node* child) {
	if (child != nullptr && std::find(this->m_children.begin(), this->m_children.end(), child) == this->m_children.end()) {
		this->m_children.push_back(child);
		child->setParent(this);
		return true;
	}
	return false;
}

ENG_API bool Node::removeChild(Node* child) {
	if (child == nullptr) return false;

	auto it = std::find(this->m_children.begin(), this->m_children.end(), child);
	if (it != this->m_children.end()) {
		this->m_children.erase(it);
		child->setParent(nullptr);
		return true;
	}

	for (Node* childNode : this->m_children) {
		if (childNode->removeChild(child)) {
			return true;
		}
	}

	return false;
}

ENG_API Node* Node::removeChildByPosition(const unsigned int& position) {
	if (position >= this->m_children.size()) return nullptr;

	Node* removedNode = this->m_children[position];
	return this->removeChild(removedNode) ? removedNode : nullptr;
}

const ENG_API unsigned int Node::getNumberOfChildren() const {
	return (unsigned int) this->m_children.size();
}

const ENG_API std::vector<Node*> Node::getChildren() const {
	std::vector<Node*> children;
	for (auto* node : this->m_children)
		children.push_back(node);
	return children;
}

void ENG_API Node::setMaterial(Material* material) {
	this->m_material = material;
}

ENG_API Material* Node::getMaterial() const{
	return this->m_material;
}

ENG_API void Node::setEnableLighting(const bool& val) {
	this->m_enableLighting = val;
}

ENG_API bool Node::isEnableLighting() const {
	return this->m_enableLighting;
}
