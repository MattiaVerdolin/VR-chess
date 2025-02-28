#include "fileOVOReader.h"
#include "mesh.h"
#include <iostream>
#include <filesystem>
#include <cstdio>

ENG_API std::string FileOVOReader::ovoExtension{ ".ovo" };

ENG_API FileOVOReader::FileOVOReader() : m_dat{ nullptr }, m_materialsMap{std::map<std::string, Material*>()} {}

ENG_API FileOVOReader::~FileOVOReader() {
    this->deleteTextures();
    this->deleteMaterials();
}

ENG_API FileOVOReader& FileOVOReader::getInstance() {
    static FileOVOReader instance;
    return instance;
}

ENG_API bool FileOVOReader::hasOVOExtension(const std::string& fileName) {
    return fileName.size() >= ovoExtension.size() &&
        std::equal(ovoExtension.rbegin(), ovoExtension.rend(), fileName.rbegin());
}

const ENG_API bool FileOVOReader::openFile(const std::string& fileName) {
    this->m_dat = fopen(fileName.c_str(), "rb");
    if(this->m_dat == NULL) return false;
    return true;
}

ENG_API char* FileOVOReader::getDataOfChunkFromFile(unsigned int& chunkId, unsigned int& chunkSize) {
    size_t bytesRead = fread(&chunkId, sizeof(unsigned int), 1, this->m_dat);

    if(bytesRead < 1) return nullptr;
    else if (feof(this->m_dat)) return nullptr;
    
    bytesRead = fread(&chunkSize, sizeof(unsigned int), 1, this->m_dat);
    if(bytesRead < 1) return nullptr;

    char* data = new char[chunkSize];
    if (fread(data, sizeof(char), chunkSize, this->m_dat) != chunkSize)
    {
        fclose(this->m_dat);
        delete[] data;
        return nullptr;
    }
    return data;
}

ENG_API void FileOVOReader::undoReadDataFromFile(const unsigned int& chunkSize) {
    long offset = -static_cast<long>(sizeof(unsigned int) * 2 + chunkSize);
    fseek(this->m_dat, offset, SEEK_CUR);
}

ENG_API Node* FileOVOReader::parseFile(const std::string& fileName) {
    std::filesystem::path filePath(fileName);

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "ERROR: file does not exist" << std::endl;
        return nullptr;
    }

    std::string normalizedPath = filePath.string();
    if (!this->hasOVOExtension(normalizedPath)) {
        std::cerr << "ERROR: file hasn't OVO extension" << std::endl;
        return nullptr;
    }

    if (!this->openFile(normalizedPath)) {
        std::cerr << "ERROR: cannot open file" << std::endl;
        return nullptr;
    }

    this->retrieveMaterials();

    Node* rootNode = this->recursiveLoad();

    fclose(this->m_dat);

    return rootNode;
}

ENG_API void FileOVOReader::retrieveMaterials() {
    unsigned int chunkId, chunkSize, position;
    while (true) {
        char* data = this->getDataOfChunkFromFile(chunkId, chunkSize);

        if (data == nullptr) break;

        position = 0;
        Object* objectToParse = OVOObjectFactory::createObjectByChunkID(chunkId, data);
        if (objectToParse == nullptr) continue;

        Material* material = dynamic_cast<Material*>(objectToParse);

        if (material != nullptr) {
            objectToParse->parse(data, position);
            this->m_materialsMap[material->getName()] = material;
        } else {
            this->undoReadDataFromFile(chunkSize);
            delete[] data;
            break;
        }
        delete[] data;
    }
}

ENG_API Node* FileOVOReader::recursiveLoad() {
    unsigned int chunkId, chunkSize, position = 0;

    char* data = this->getDataOfChunkFromFile(chunkId, chunkSize);
    Node* nodeToParse = dynamic_cast<Node*>(OVOObjectFactory::createObjectByChunkID(chunkId, data));

    if (nodeToParse == nullptr) return nullptr;

    unsigned int numberOfChildren = nodeToParse->parse(data, position);

    delete[] data;

    Mesh* possibleMesh = dynamic_cast<Mesh*>(nodeToParse);

    if (possibleMesh != nullptr && possibleMesh->getMaterial() != nullptr) {
        const std::string materialName = possibleMesh->getMaterial()->getName();
        delete possibleMesh->getMaterial();
        possibleMesh->setMaterial(this->m_materialsMap[materialName]);
    }
    

    if(numberOfChildren)
        for (unsigned int i = 0; i < numberOfChildren; i++) {
            Node* childNode = this->recursiveLoad();
            nodeToParse->addChild(childNode);
        }

    return nodeToParse;
}

ENG_API void FileOVOReader::deleteTextures() {
    std::set<Texture*> textures;

    for (const auto& [materialName, materialPtr] : m_materialsMap) {
        if (materialPtr != nullptr) {
            Texture* texture = const_cast<Texture*>(materialPtr->getTexture());
            if (texture != nullptr) {
                textures.insert(texture);
            }
        }
    }

    for (auto* texture : textures)
        delete texture;
}

ENG_API void FileOVOReader::deleteMaterials() {
    for (const auto& [materialName, materialPtr] : m_materialsMap)
        delete materialPtr;
}