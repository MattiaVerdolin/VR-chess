/**
 * @file		engine.cpp
 * @brief	Graphics engine main file
 *
 * @author	Luca Fantò (C) SUPSI [luca.fanto@student.supsi.ch]
 * @author	Mattia Cainarca (C) SUPSI [mattia.cainarca@student.supsi.ch]
 * @author	Antonio Marroffino (C) SUPSI [antonio.marroffino@student.supsi.ch]
 */



//////////////
// #INCLUDE //
//////////////

   // Main include:
    #include "engine.h"
    #include "notificationService.h"
    #include "textManager.h"
    #include "glm/glm.hpp"
    #include "glm/gtc/type_ptr.hpp"
    #include <GL/glew.h>
    #include "GL/freeglut.h"
    #include "fileOVOReader.h"
    #include "list.h"

    #include "FreeImage.h"

   // C/C++:
    #include <iostream>
    #include <source_location>
    #include <chrono>

#include "shader.h"


/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

/**
 * @brief Base class reserved structure (using PIMPL/Bridge design pattern https://en.wikipedia.org/wiki/Opaque_pointer).
 */
struct Eng::Base::Reserved
{
    int windowId;
   // Flags:
   bool initFlag;

   FileOVOReader& fileOVOReader;

   List listOfScene;

   NotificationService& notificationService;

   TextManager& textManager;

   /**
    * Constructor.
    */
   Reserved() : windowId{ -1 }, initFlag{ false }, fileOVOReader{ FileOVOReader::getInstance() }, listOfScene{List()}, notificationService{NotificationService::getInstance()},
       textManager{TextManager::getInstance()}
   {}
};

int frameCount = 0;
float fps = 0.0f;
int previousTime = 0;

Eng::Base Eng::Base::instance;

void Eng::Base::handleReshape(int width, int height) {

    this->reserved->notificationService.notifyOnReshapeWindow(width, height, shader, projLoc);
}

////////////////////////
// BODY OF CLASS Base //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor.
 */
ENG_API Eng::Base::Base() : reserved(std::make_unique<Eng::Base::Reserved>())
{
#ifdef _DEBUG
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Destructor.
 */
ENG_API Eng::Base::~Base()
{
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Gets a reference to the (unique) singleton instance.
 * @return reference to singleton instance
 */
Eng::Base ENG_API &Eng::Base::getInstance()
{
   return Eng::Base::instance;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ENG_API Eng::Base::setKeyboardCallback(void (*keyboardCallback)(unsigned char, int, int)) 
{
    if (keyboardCallback != nullptr)
        glutKeyboardFunc(keyboardCallback);
}

void ENG_API Eng::Base::setSpecialCallback(void (*specialCallback)(int, int, int)) 
{
    if (specialCallback != nullptr)
        glutSpecialFunc(specialCallback);
}

const char* vertShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;

   // Attributes:
   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;

   // Varying:
   out vec4 fragPosition;
   out vec3 normal;   

   void main(void)
   {
      fragPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragPosition;      
   }
)";

////////////////////////////
const char* fragShader = R"(
   #version 440 core

    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

)";

/**
 * Init internal components.
 * @return TF
 */
bool ENG_API Eng::Base::init(void (*closeCallBack)())
{
   // Already initialized?
   if (reserved->initFlag)
   {
      std::cout << "ERROR: engine already initialized" << std::endl;
      return false;
   }

   //PER ORA
   int argc = 1;
   char* argv[1] = { const_cast<char*>("Engine") };
   glutInit(&argc, argv);


   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitContextVersion(4, 4);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   
   glutInitWindowPosition(100, 100);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   reserved->windowId = glutCreateWindow("3D Chessboard Game");

   //GLEW INIT
   GLenum err = glewInit();
   if (err != GLEW_OK)
   {
       std::cout << "Generic Glew Error" << std::endl;
   }

   if (!glewIsSupported("GL_VERSION_4_4"))
   {
       std::cout << "Version error" << std::endl;
   }

   // Compile vertex shader:
   vs = new Shader();
   vs->loadFromMemory(Shader::TYPE_VERTEX, vertShader);

   // Compile fragment shader:
   fs = new Shader();
   fs->loadFromMemory(Shader::TYPE_FRAGMENT, fragShader);

   // Setup shader program:
   shader = new Shader();
   shader->build(vs, fs);
   shader->render();
   shader->bind(0, "in_Position");
   shader->bind(1, "in_Normal");

   // Get shader variable locations:
   projLoc = shader->getParamLocation("projection");
   mvLoc = shader->getParamLocation("modelview");

   glutDisplayFunc([]() {});
   glutReshapeFunc([](int width, int height) {Eng::Base::instance.handleReshape(width, height); });
   if (closeCallBack != nullptr) glutCloseFunc(closeCallBack);

   glm::vec4 gAmbient(0.2f, 0.2f, 0.2f, 1.0f);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(gAmbient));

   FreeImage_Initialise();

   // Done:
   std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
   reserved->initFlag = true;
   return true;
}

const ENG_API float& Eng::Base::getFPS() {
    frameCount++;

    // Ottieni il tempo attuale
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - previousTime;

    if (elapsedTime > 1000) { // Un secondo è passato
        fps = frameCount / (elapsedTime / 1000.0f); // Calcola FPS
        previousTime = currentTime; // Resetta il tempo
        frameCount = 0; // Resetta il contatore
    }
    return fps;
}


ENG_API Node* Eng::Base::load(const std::string& fileName) {
    if (!this->reserved->fileOVOReader.hasOVOExtension(fileName)) {
        std::cerr << "ERROR: Files given is not supported" << std::endl;
        return nullptr;
    }
    Node* rootNode = this->reserved->fileOVOReader.parseFile(fileName);
    return rootNode;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * passing all node of the scene and save them into a list
 */
void ENG_API Eng::Base::passScene(Node* rootNode) {
    this->reserved->listOfScene.pass(rootNode);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  clear all list of the scene
 */
void ENG_API Eng::Base::clearScene() {
    this->reserved->listOfScene.clearList();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * passing all node of the scene and save them into a list
 */
void ENG_API Eng::Base::begin3D(Camera* mainCamera, Camera* menuCamera, const std::list<std::string>& menu) {
    if (mainCamera == nullptr) 
        return;
    mainCamera->render();
    this->reserved->listOfScene.renderElements(mainCamera->getInverseCameraFinalMatrix(), shader, mvLoc);

    if (menuCamera == nullptr || menu.empty())
        return;
    this->reserved->textManager.displayText(menu, menuCamera);
    this->reserved->textManager.displayFPS(this->getFPS(), menuCamera);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Swap buffers.
 */
void ENG_API Eng::Base::swap() {
    if(glutGetWindow())
        glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Clear the screen.
 */
void ENG_API Eng::Base::clear() {
    glutMainLoopEvent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Free internal components.
 * @return TF
 */
bool ENG_API Eng::Base::free()
{
   // Not initialized?
   if (!reserved->initFlag)
   {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   FreeImage_DeInitialise();
   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved->initFlag = false;
   delete shader;
   delete fs;
   delete vs;
   return true;
}
