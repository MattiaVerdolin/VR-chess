#include "textManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "GL/freeglut.h"

ENG_API TextManager::TextManager() {}

ENG_API TextManager& TextManager::getInstance() {
	static TextManager instance;
	return instance;
}

void ENG_API TextManager::displayText(const std::list<std::string>& texts, Camera* camera) const {
	if (dynamic_cast<OrthoCamera*>(camera) == nullptr)
		return;

	camera->render();

	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

	float y = 10.0f;
	float yIncrement = 15.0f;
	float width = this->setDynamicWidth(texts) * 9.0f;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(width, 0.0f);
	glVertex2f(0.0f, yIncrement * texts.size() + y * 1.5f);
	glVertex2f(width, yIncrement * texts.size() + y * 1.5f);
	glEnd();
	glDisable(GL_BLEND);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (const auto& text : texts) {
		glRasterPos2f(5.0f, y);
		glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
		y += yIncrement;
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

ENG_API void TextManager::displayFPS(const float& fps, Camera* camera) const {
	if (camera == nullptr) return;

	camera->render();

	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

	std::ostringstream oss;
	oss << "FPS: " << std::fixed << std::setprecision(1) << fps;
	std::string finalFPSstr = oss.str();

	float y = (float)glutGet(GLUT_WINDOW_HEIGHT) - 20.0f;
	float yIncrement = -15.0f;
	float width = finalFPSstr.length() * 10.0f;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0.0f, y);
	glVertex2f(width, y);
	glVertex2f(0.0f, yIncrement + y * 1.5f);
	glVertex2f(width, yIncrement + y * 1.5f);
	glEnd();
	glDisable(GL_BLEND);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(5.0f, y + 5.0f);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)finalFPSstr.c_str());


	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

ENG_API float TextManager::setDynamicWidth(const std::list<std::string>& texts) const {
	auto maxLength = std::max_element(
		texts.begin(),
		texts.end(),
		[](const std::string& a, const std::string& b) {
			return a.length() < b.length();
		}
	);
	return maxLength != texts.end() ? (float)maxLength->length() : 0.0f;
}