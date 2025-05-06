//////////////
// #INCLUDE //
//////////////
#include <filesystem>
#include <iostream>

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

// Textures:
unsigned int cubemapId;
std::string cubemapNames[] =
{
   "textures/cubemap/negx.jpg",
   "textures/cubemap/posx.jpg",
   "textures/cubemap/posy.jpg",
   "textures/cubemap/negy.jpg",
   "textures/cubemap/posz.jpg",
   "textures/cubemap/negz.jpg",
};

// Cube VBO:
unsigned int globalVao = 0;
unsigned int cubeVboVertices = 0;
float cubeVertices[] = // Vertex and tex. coords are the same
{
   -1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
};
unsigned int cubeVboFaces = 0;
unsigned short cubeFaces[] =
{
   0, 1, 2,
   0, 2, 3,
   3, 2, 6,
   3, 6, 7,
   4, 0, 3,
   4, 3, 7,
   6, 5, 4,
   7, 6, 4,
   4, 5, 1,
   4, 1, 0,
   1, 5, 6,
   1, 6, 2,
};

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

//////////////////////////////////////////
const char* vertShaderCube = R"(
   #version 440 core

   uniform mat4 projection;
   uniform mat4 modelview;

   layout(location = 0) in vec3 in_Position;

   out vec3 texCoord;

   void main(void)
   {
      texCoord = in_Position;
      gl_Position = projection * modelview * vec4(in_Position, 1.0f);
   }
)";

//////////////////////////////////////////
const char* fragShaderCube = R"(
   #version 440 core

   in vec3 texCoord;

   // Texture mapping (cubemap):
   layout(binding = 0) uniform samplerCube cubemapSampler;

   out vec4 fragOutput;

   void main(void)
   {
      fragOutput = texture(cubemapSampler, texCoord);
   }
)";

// Leap Motion shaders:
const char* vertShaderLeap = R"(
#version 440 core
uniform mat4 projection;
uniform mat4 modelview;
layout(location = 0) in vec3 in_Position;
void main(void)
{
    gl_Position = projection * modelview * vec4(in_Position, 1.0f);      
}
)";

const char* fragShaderLeap = R"(
#version 440 core
uniform vec3 color;   
out vec4 frag_Output;
void main(void)
{      
    frag_Output = vec4(color, 1.0f);
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

   vsCube = new Shader();
   vsCube->loadFromMemory(Shader::TYPE_VERTEX, vertShaderCube);

   fsCube = new Shader();
   fsCube->loadFromMemory(Shader::TYPE_FRAGMENT, fragShaderCube);

   shaderCube = new Shader();
   shaderCube->build(vsCube, fsCube);
   shaderCube->render();
   shaderCube->bind(0, "in_Position");

   // --- Leap Motion shader setup ---
   vsLeap = new Shader();
   vsLeap->loadFromMemory(Shader::TYPE_VERTEX, vertShaderLeap);

   fsLeap = new Shader();
   fsLeap->loadFromMemory(Shader::TYPE_FRAGMENT, fragShaderLeap);

   shaderLeap = new Shader();
   shaderLeap->build(vsLeap, fsLeap);
   shaderLeap->render();
   shaderLeap->bind(0, "in_Position");

   leapProjLoc = shaderLeap->getParamLocation("projection");
   leapMVLoc = shaderLeap->getParamLocation("modelview");
   leapColorLoc = shaderLeap->getParamLocation("color");

   // Build a sphere procedurally for Leap Motion joints:
   GLfloat x, y, z, alpha, beta;
   GLfloat radius = 5.0f;
   int gradation = 10;

   for (alpha = 0.0; alpha < glm::pi<float>(); alpha += glm::pi<float>() / gradation)
       for (beta = 0.0f; beta < 2.01f * glm::pi<float>(); beta += glm::pi<float>() / gradation)
       {
           x = radius * cos(beta) * sin(alpha);
           y = radius * sin(beta) * sin(alpha);
           z = radius * cos(alpha);
           leapVertices.push_back(glm::vec3(x, y, z));

           x = radius * cos(beta) * sin(alpha + glm::pi<float>() / gradation);
           y = radius * sin(beta) * sin(alpha + glm::pi<float>() / gradation);
           z = radius * cos(alpha + glm::pi<float>() / gradation);
           leapVertices.push_back(glm::vec3(x, y, z));
       }

   // Init VAO + VBO LEAP MOTION:
   glGenVertexArrays(1, &leapVao);
   glBindVertexArray(leapVao);

   glGenBuffers(1, &leapVbo);
   glBindBuffer(GL_ARRAY_BUFFER, leapVbo);
   glBufferData(GL_ARRAY_BUFFER, leapVertices.size() * sizeof(glm::vec3), leapVertices.data(), GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);

   // Init Leap Motion:
   leap = new Leap();
   if (!leap->init()) {
       std::cerr << "[ERROR] Unable to init Leap Motion" << std::endl;
       delete leap;
       leap = nullptr;
   }


   // 1) Costruisco il VAO + VBO/EBO del cubo
   glGenVertexArrays(1, &globalVao);
   glBindVertexArray(globalVao);

   glGenBuffers(1, &cubeVboVertices);
   glBindBuffer(GL_ARRAY_BUFFER, cubeVboVertices);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   glGenBuffers(1, &cubeVboFaces);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVboFaces);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

   glBindVertexArray(0);

   // 2) Associazione del samplerCube a texture unit 0
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
   GLint locSampler = shaderCube->getParamLocation("cubemapSampler");
   shaderCube->setInt(locSampler, 0);

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
   //glEnable(GL_CULL_FACE);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIREFRAME

   FreeImage_Initialise();

   // Done:
   std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
   
   buildCubemap();
   
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

void Eng::Base::drawLeapPart(const glm::mat4& view,
    const glm::mat4& offsetDraw,
    const glm::vec3& pos)
{
    glm::mat4 c = glm::translate(glm::mat4(1.0f), pos);
    shaderLeap->setMatrix(leapMVLoc,
        view * offsetDraw * glm::scale(glm::mat4(1.0f), glm::vec3(0.001f)) * c);
    glBindVertexArray(leapVao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)leapVertices.size());
    glBindVertexArray(0);
}

void Eng::Base::renderLeapHands(const LEAP_TRACKING_EVENT* l,
    const glm::mat4& view,
    const glm::mat4& proj)
{
    if (!l) return;

    shaderLeap->render();
    shaderLeap->setMatrix(leapProjLoc, proj);

    glm::mat4 offsetDraw = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 1.0f, 0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    for (unsigned int h = 0; h < l->nHands; ++h) {
        LEAP_HAND hand = l->pHands[h];
        glm::vec3 thumbTip, indexTip;
        shaderLeap->setVec3(leapColorLoc, glm::vec3((float)h, (float)(1 - h), 0.5f));

        // draw arm, palm, fingers:
        drawLeapPart(view, offsetDraw,
            glm::vec3(hand.arm.next_joint.x, hand.arm.next_joint.y, hand.arm.next_joint.z));
        drawLeapPart(view, offsetDraw,
            glm::vec3(hand.palm.position.x, hand.palm.position.y, hand.palm.position.z));

        for (unsigned int d = 0; d < 5; ++d) {
            LEAP_DIGIT finger = hand.digits[d];
            for (unsigned int b = 0; b < 4; ++b) {
                LEAP_BONE bone = finger.bones[b];
                drawLeapPart(view, offsetDraw,
                    glm::vec3(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z));
            }
        }
    }
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

    // ---------------------------------
    // 1) Modalità VR
    // ---------------------------------
    if (reserved->vrEnabled) {
        ovr->update();
        glm::mat4 headPos = ovr->getModelviewMatrix();

        float eyeHeight = 0.5f;
        glm::mat4 floorOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0, eyeHeight, 0));
        glm::mat4 horizontalOffset = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, 0, 1.3f));
        glm::mat4 rotationOffset = glm::rotate(glm::mat4(1.0f), glm::radians(290.0f), glm::vec3(0, 1, 0));

        for (int eye = 0; eye < EYE_LAST; ++eye) {
            fbo[eye]->render();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            OvVR::OvEye curEye = (OvVR::OvEye)eye;
            glm::mat4 projMat = ovr->getProjMatrix(curEye, nearPlane, farPlane);
            glm::mat4 eye2head = ovr->getEye2HeadMatrix(curEye);
            glm::mat4 ovrProjMat = projMat * glm::inverse(eye2head);
            glm::mat4 ovrModelView = glm::inverse(floorOffset * rotationOffset * horizontalOffset * headPos);

            // SKYBOX
            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);
            shaderCube->render();
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);

            GLint locP = shaderCube->getParamLocation("projection");
            GLint locMV = shaderCube->getParamLocation("modelview");
            shaderCube->setMatrix(locP, ovrProjMat);

            glm::mat4 rotOnly = glm::mat4(glm::mat3(ovrModelView));
            glm::mat4 skyModel = glm::scale(rotOnly, glm::vec3(50.0f));
            shaderCube->setMatrix(locMV, skyModel);

            glBindVertexArray(globalVao);
            glDrawElements(GL_TRIANGLES,
                sizeof(cubeFaces) / sizeof(unsigned short),
                GL_UNSIGNED_SHORT,
                nullptr);
            glBindVertexArray(0);

            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
            glCullFace(GL_BACK);

            // SCENA 3D
            shader->render();
            shader->setMatrix(shader->getParamLocation("projection"), ovrProjMat);
            mainCamera->render();
            reserved->listOfScene.renderElements(ovrModelView, nearPlane, farPlane);

            // Leap Motion rendering (VR)
            if (leap) {
                leap->update();
                const LEAP_TRACKING_EVENT* l = leap->getCurFrame();
                renderLeapHands(l, ovrModelView, ovrProjMat);
            }

            ovr->pass(curEye, fboTexId[eye]);
        }

        ovr->render();
        Fbo::disable();
        glViewport(0, 0, prevViewport[2], prevViewport[3]);

        if (menuCamera && !menu.empty()) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[0]->getHandle());
            glBlitFramebuffer(0, 0, fboWidth, fboHeight, 0, 0, APP_WINDOWSIZEX / 2, APP_WINDOWSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[1]->getHandle());
            glBlitFramebuffer(0, 0, fboWidth, fboHeight, APP_WINDOWSIZEX / 2, 0, APP_WINDOWSIZEX, APP_WINDOWSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
    }
    // ---------------------------------
    // 2) Modalità Standard (non‐VR)
    // ---------------------------------
    else {
        shader->render();
        shader->setMatrix(shader->getParamLocation("projection"), mainCamera->getProjectionMatrix());
        mainCamera->render();
        reserved->listOfScene.renderElements(mainCamera->getInverseCameraFinalMatrix(),
            mainCamera->getNearPlane(),
            mainCamera->getFarPlane());

        if (leap) {
            leap->update();
            const LEAP_TRACKING_EVENT* l = leap->getCurFrame();
            renderLeapHands(l, mainCamera->getInverseCameraFinalMatrix(), mainCamera->getProjectionMatrix());
        }

        // Skybox
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        shaderCube->render();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        GLint locP = shaderCube->getParamLocation("projection");
        GLint locMV = shaderCube->getParamLocation("modelview");
        shaderCube->setMatrix(locP, mainCamera->getProjectionMatrix());

        glm::mat4 view = mainCamera->getInverseCameraFinalMatrix();
        glm::mat4 rotOnly = glm::mat4(glm::mat3(view));
        glm::mat4 skyModel = glm::scale(rotOnly, glm::vec3(50.0f));
        shaderCube->setMatrix(locMV, skyModel);

        glBindVertexArray(globalVao);
        glDrawElements(GL_TRIANGLES,
            sizeof(cubeFaces) / sizeof(unsigned short),
            GL_UNSIGNED_SHORT,
            nullptr);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glCullFace(GL_BACK);
    }

    reserved->textManager.displayText(menu, menuCamera);
    reserved->textManager.displayFPS(this->getFPS(), menuCamera);
}

/**
 * Load cubemap into a texture.
 */
void ENG_API Eng::Base::buildCubemap()
{
    // Create and bind cubemap:
    glGenTextures(1, &cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

    // Set params:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Set filters:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Load sides:
    for (int curSide = 0; curSide < 6; curSide++)
    {
        // Load texture:
        FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(cubemapNames[curSide].c_str(), 0), cubemapNames[curSide].c_str());
        if (fBitmap == nullptr)
            std::cout << "[ERROR] loading file '" << cubemapNames[curSide] << "'" << std::endl;
        int intFormat = GL_RGB;
        GLenum extFormat = GL_BGR;
        if (FreeImage_GetBPP(fBitmap) == 32)
        {
            intFormat = GL_RGBA;
            extFormat = GL_BGRA;
        }

        // Fix mirroring:
        FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
        FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

        // Send texture to OpenGL:
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, 0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap));

        // Free resources:
        FreeImage_Unload(fBitmap);
    }
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

   delete shaderCube;
   delete fsCube;
   delete vsCube;

   glDeleteBuffers(1, &cubeVboVertices);
   glDeleteBuffers(1, &cubeVboFaces);
   glDeleteVertexArrays(1, &globalVao);
   glDeleteTextures(1, &cubemapId);

   if (ovr) {
       ovr->free();
       delete ovr;
       ovr = nullptr;
   }

   // Free Leap Motion:
   glDeleteBuffers(1, &leapVbo);
   glDeleteVertexArrays(1, &leapVao);
   delete shaderLeap;
   delete fsLeap;
   delete vsLeap;

   if (leap) {
       leap->free();
       delete leap;
       leap = nullptr;
   }


   return true;
}

ENG_API Fbo* Eng::Base::getCurrent(int numEye) {
    return fbo[numEye];
}

ENG_API Eng::Base::HandLeapData* Eng::Base::getHandsData() {
    return this->hands;
}
