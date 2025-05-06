/**
 * @file		fileOVOReader.h
 * @brief	OVO File Reader header file
 *
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Riccardo Cristallo (C) SUPSI [riccardo.cristallo@student.supsi.ch]
 * @author	Mattia Verdolin (C) SUPSI [mattia.verdolin@student.supsi.ch]
 *
 * This file defines the FileOVOReader class, which is responsible for reading and parsing `.ovo` files,
 * a custom binary format. It handles the file opening, chunk reading, material retrieval, and node parsing.
 * This class uses a singleton pattern to ensure only one instance is used for file processing.
 */

#pragma once

//////////////
// #INCLUDE //
//////////////

// Project-specific includes
#include "ovoObjectFactory.h"
#include "node.h"
#include "material.h"

// Standard libraries
#include <map>
#include <set>

//////////////////////////////
// MAIN FILEOVOREADER CLASS //
//////////////////////////////

/**
 * @brief Concrete FileOVOReader class.
 *
 * The FileOVOReader class is responsible for parsing `.ovo` files. It reads the file, processes the chunks of data,
 * retrieves materials, and constructs a hierarchical scene graph of nodes from the parsed data.
 */
class ENG_API FileOVOReader {
public:
    /**
     * @brief Copy constructor is deleted to ensure singleton usage.
     */
    FileOVOReader(const FileOVOReader&) = delete;

    /**
     * @brief Destructor for the FileOVOReader class.
     *
     * This destructor ensures proper cleanup of resources used by the FileOVOReader instance.
     * It deletes all textures and materials stored in the instance to prevent memory leaks.
     */
    ~FileOVOReader();

    /**
     * @brief Assignment operator is deleted to ensure singleton usage.
     */
    FileOVOReader& operator=(const FileOVOReader&) = delete;

    /**
     * @brief Returns the singleton instance of the FileOVOReader.
     *
     * This method returns the unique instance of the FileOVOReader.
     *
     * @return The singleton instance of FileOVOReader.
     */
    static FileOVOReader& getInstance();

    /**
     * @brief Checks if the provided file name has the `.ovo` extension.
     *
     * This method checks if the given file name has the `.ovo` extension.
     *
     * @param fileName The file name to check.
     * @return `true` if the file has `.ovo` extension, `false` otherwise.
     */
    bool hasOVOExtension(const std::string& fileName);

    /**
     * @brief Parses the `.ovo` file and constructs the scene graph.
     *
     * This method opens the specified `.ovo` file, reads its chunks, and recursively constructs the scene graph
     * from the parsed data, returning the root node of the scene.
     *
     * @param fileName The file name of the `.ovo` file to parse.
     * @return A pointer to the root node of the parsed scene graph.
     */
    Node* parseFile(const std::string& fileName);

private:
    /**
     * @brief Constructor that initializes the FileOVOReader.
     */
    FileOVOReader();

    /**
     * @brief Opens the specified file.
     *
     * This method attempts to open the file in binary read mode.
     *
     * @param fileName The name of the file to open.
     * @return `true` if the file was successfully opened, `false` otherwise.
     */
    const bool openFile(const std::string& fileName);

    /**
     * @brief Reads a chunk of data from the opened file.
     *
     * This method reads the chunk ID and size from the file, followed by the chunk data itself.
     *
     * @param chunkId A reference to store the chunk ID.
     * @param chunkSize A reference to store the chunk size.
     * @return A pointer to the chunk data, or `nullptr` if an error occurs.
     */
    char* getDataOfChunkFromFile(unsigned int& chunkId, unsigned int& chunkSize);

    /**
     * @brief Rolls back the file pointer after reading a chunk.
     *
     * This method undoes the reading of a chunk by moving the file pointer back to the previous position.
     *
     * @param chunkSize The size of the chunk to roll back.
     */
    void undoReadDataFromFile(const unsigned int& chunkSize);

    /**
     * @brief Retrieves materials from the file.
     *
     * This method reads the material chunks from the file and stores them in the `m_materialsMap` map.
     */
    void retrieveMaterials();

    /**
     * @brief Recursively loads the scene graph from the file.
     *
     * This method recursively reads the nodes from the file, constructing the scene graph of `Node` objects,
     * and assigns materials to meshes.
     *
     * @return A pointer to the root node of the scene graph.
     */
    Node* recursiveLoad();

    /**
     * @brief Deletes all textures associated with the materials in the `.ovo` file.
     *
     * This method iterates through the materials map and collects unique texture pointers
     * associated with the materials. It then deletes the textures to free up memory.
     */
    void deleteTextures();

    /**
     * @brief Deletes all materials stored in the materials map.
     *
     * This method iterates through the materials map and deletes each material object,
     * ensuring proper cleanup of dynamically allocated resources.
     */
    void deleteMaterials();

    /**
     * @brief The `.ovo` file extension.
     */
    static std::string ovoExtension;

    /**
     * @brief File pointer for reading the `.ovo` file.
     */
    FILE* m_dat;

    /**
     * @brief Map of materials indexed by their names.
     */
    std::map<std::string, Material*> m_materialsMap;
};
