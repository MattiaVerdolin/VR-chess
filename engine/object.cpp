#include "object.h"

ENG_API unsigned int Object::m_cnt_object{ 0 };

ENG_API Object::Object(const std::string& name)
	: m_id{ createNewId() }, m_name{name} {}

ENG_API Object::Object(const Object& other) : m_id{this->createNewId()}, m_name{other.m_name} {}

ENG_API Object& Object::operator=(const Object& other) {
	this->m_id = other.m_id;
	this->m_name = other.m_name;
	return *this;
}

const ENG_API unsigned int& Object::getId() const {
	return this->m_id;
}

const ENG_API std::string& Object::getName() const {
	return this->m_name;
}

void ENG_API Object::setName(const std::string& name) {
	this->m_name = name;
}

unsigned int ENG_API Object::createNewId() {
	return this->m_cnt_object++;
}