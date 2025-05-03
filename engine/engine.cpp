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
    #include "mesh.h"
    #include "FreeImage.h"

   // C/C++:
    #include <iostream>
    #include <source_location>
    #include <chrono>
    #include <fstream>
    #include <string>



	#include "shader.h"


/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

/**
 * @brief Base class reserved structure (using PIMPL/Bridge design pattern https://en.wikipedia.org/wiki/Opaque_pointer).
 */

 // Window size:
#define APP_WINDOWSIZEX   1024
#define APP_WINDOWSIZEY   512
//#define APP_FBOSIZEX      APP_WINDOWSIZEX / 2
//#define APP_FBOSIZEY      APP_WINDOWSIZEY / 1

struct Eng::Base::Reserved
{
    int windowId;
   // Flags:
   bool initFlag;

   FileOVOReader& fileOVOReader;

   List listOfScene;

   NotificationService& notificationService;

   TextManager& textManager;

   bool vrEnabled;

   /**
    * Constructor.
    */
   Reserved() : windowId{ -1 }, initFlag{ false }, vrEnabled{ false }, fileOVOReader{ FileOVOReader::getInstance() }, listOfScene{List()}, notificationService{NotificationService::getInstance()},
       textManager{TextManager::getInstance()}
   {}
};

int frameCount = 0;
float fps = 0.0f;
int previousTime = 0;
const float nearPlane = 0.01f;
const float farPlane = 100.0f;

Eng::Base Eng::Base::instance;

void Eng::Base::handleReshape(int width, int height) {

    this->reserved->notificationService.notifyOnReshapeWindow(width, height, shader);
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


bool ENG_API Eng::Base::loadVRModeFromConfig() {
    std::ifstream configFile("../config.txt");
    if (!configFile.is_open()) {
        std::cerr << "[WARN] Impossibile aprire config.txt. Modalità VR disattivata." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.find("mode=vr") != std::string::npos)
            return true;
        else if (line.find("mode=standard") != std::string::npos)
            return false;
    }

    std::cerr << "[WARN] Modalità non specificata correttamente. Default: standard." << std::endl;
    return false;
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

/////////////
// SHADERS //
/////////////

////////////////////////////
const char* vertShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;
   uniform mat3 normalMatrix;

   // Attributes:
   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;
   layout(location = 2) in vec2 in_TexCoord;


   // Varying:
   out vec4 fragPosition;
   out vec3 normal;   
   out vec2 texCoord;


   void main(void)
   {
      fragPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragPosition;      
      normal = normalMatrix * in_Normal;
      texCoord = in_TexCoord;
   }
)";

////////////////////////////
const char* fragShader = R"(
   #version 440 core

in vec4 fragPosition;
in vec3 normal;   
in vec2 texCoord;

out vec4 fragOutput;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

// Light properties:
uniform vec3 lightPosition[8]; 
uniform vec3 lightAmbient[8]; 
uniform vec3 lightDiffuse[8]; 
uniform vec3 lightSpecular[8];

layout(binding = 0) uniform sampler2D texSampler;


void main(void)
{      
    vec4 texel = texture(texSampler, texCoord);

    vec3 _normal = normalize(normal);
    vec3 viewDirection = normalize(-fragPosition.xyz);
    vec3 fragColor = matEmission;

    for (int i = 0; i < 8; i++)
    {
        fragColor += matAmbient * lightAmbient[i];
        vec3 lightDirection = normalize(lightPosition[i] - fragPosition.xyz);
        float nDotL = max(dot(_normal, lightDirection), 0.0f);
        if (nDotL > 0.0f)
        {
            fragColor += matDiffuse * nDotL * lightDiffuse[i];

            vec3 halfVector = normalize(lightDirection + viewDirection);
            float nDotHV = max(dot(_normal, halfVector), 0.0f);
            fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular[i];
        }
    }

    fragOutput = texel * vec4(fragColor, 1.0f);
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

   reserved->vrEnabled = loadVRModeFromConfig();

   //PER ORA
   int argc = 1;
   char* argv[1] = { const_cast<char*>("Engine") };
   glutInit(&argc, argv);


   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitContextVersion(4, 4);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(APP_WINDOWSIZEX, APP_WINDOWSIZEY);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   reserved->windowId = glutCreateWindow("3D Chessboard Game");

   std::cout << glGetString(GL_VENDOR) << std::endl;
   std::cout << glGetString(GL_RENDERER) << std::endl;

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

   // INIT OPENVR
   if (reserved->vrEnabled) {
       ovr = new OvVR();
       if (!ovr->init()) {
           std::cerr << "[ERROR] OpenVR initialization failed." << std::endl;
           return false;
       }
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

   ///////////////////////////////////////////////////////Fbo init()
   if (reserved->vrEnabled) {
       GLint prevViewport[4];
       glGetIntegerv(GL_VIEWPORT, prevViewport);

       fboWidth = ovr->getHmdIdealHorizRes();
       fboHeight = ovr->getHmdIdealVertRes();

       for (int c = 0; c < EYE_LAST; c++)
       {
           glGenTextures(1, &fboTexId[c]);
           glBindTexture(GL_TEXTURE_2D, fboTexId[c]);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
           glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
           glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

           fbo[c] = new Fbo();
           fbo[c]->bindTexture(0, Fbo::BIND_COLORTEXTURE, fboTexId[c]);
           fbo[c]->bindRenderBuffer(1, Fbo::BIND_DEPTHBUFFER, fboWidth, fboHeight);
           if (!fbo[c]->isOk())
               std::cout << "[ERROR] Invalid FBO" << std::endl;
       }
       Fbo::disable();
       glViewport(0, 0, prevViewport[2], prevViewport[3]);
   }
   ////////////////////////////////////////////////////////



   glutDisplayFunc([]() {});
   glutReshapeFunc([](int width, int height) {Eng::Base::instance.handleReshape(width, height); });
   if (closeCallBack != nullptr) glutCloseFunc(closeCallBack);

   glm::vec4 gAmbient(0.2f, 0.2f, 0.2f, 1.0f);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIREFRAME

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

////////////////////////////
// Eng::Base::begin3D()   //
////////////////////////////
void ENG_API Eng::Base::begin3D(Camera* mainCamera, Camera* menuCamera, const std::list<std::string>& menu)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);

    if (mainCamera == nullptr)
        return;

    if (reserved->vrEnabled) {
        ovr->update();
        glm::mat4 headPos = ovr->getModelviewMatrix();

        // Offset
        float eyeHeight = 0.5f;
        glm::mat4 floorOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0, eyeHeight, 0));

        // 2) Offset orizzontale
        float offsetX = -0.2f;
        float offsetZ = 1.3f;
        glm::mat4 horizontalOffset = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(offsetX, 0.0f, offsetZ)
        );

        float yawDegrees = 290.0f;
        glm::mat4 rotationOffset = glm::rotate(glm::mat4(1.0f), glm::radians(yawDegrees), glm::vec3(0, 1, 0));

        for (int c = 0; c < EYE_LAST; c++)
        {
            // OpenVR matrices
            OvVR::OvEye curEye = (OvVR::OvEye)c;
            glm::mat4 projMat = ovr->getProjMatrix(curEye, nearPlane, farPlane);
            glm::mat4 eye2Head = ovr->getEye2HeadMatrix(curEye);

            // Projection
            glm::mat4 ovrProjMat = projMat * glm::inverse(eye2Head);

            glm::mat4 ovrModelViewMat = glm::inverse(floorOffset * rotationOffset * horizontalOffset * headPos);

            // render nel FBO
            fbo[c]->render();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader->render();
            shader->setMatrix(Shader::getCurrent()->getParamLocation("projection"), ovrProjMat);

            // disegna la scena con la view modificata
            mainCamera->render();
            this->reserved->listOfScene.renderElements(ovrModelViewMat, nearPlane, farPlane);

            // invia la texture all'occhio OpenVR
            ovr->pass(curEye, fboTexId[c]);
        }

        // Update internal OpenVR settings:
        ovr->render();

        Fbo::disable();
        glViewport(0, 0, prevViewport[2], prevViewport[3]);


        if (menuCamera == nullptr || menu.empty())
            return;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[0]->getHandle());
        glBlitFramebuffer(0, 0, fboWidth, fboHeight, 0, 0, APP_WINDOWSIZEX / 2, APP_WINDOWSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[1]->getHandle());
        glBlitFramebuffer(0, 0, fboWidth, fboHeight, APP_WINDOWSIZEX / 2, 0, APP_WINDOWSIZEX, APP_WINDOWSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    else {
        mainCamera->render();
        this->reserved->listOfScene.renderElements(mainCamera->getInverseCameraFinalMatrix(), mainCamera->getNearPlane(), mainCamera->getFarPlane());
    }

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

   if (ovr) {
       ovr->free();
       delete ovr;
       ovr = nullptr;
   }

   return true;
}

ENG_API Fbo* Eng::Base::getCurrent(int numEye) {
    return fbo[numEye];
}