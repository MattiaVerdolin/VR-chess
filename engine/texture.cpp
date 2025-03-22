#include <GL/freeglut.h>
#include "FreeImage.h"
#include "texture.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF


bool Texture::isAnisotropicSupported{ false };
int Texture::anisotropicLevel{ 1 };
bool Texture::isAnisotropicAlreadySet{ false };

struct Texture::Reserved {
	unsigned int m_textureId;
	FIBITMAP* m_bitmap;

	Reserved() : m_textureId{ 0 }, m_bitmap{ nullptr } { }

};

ENG_API Texture::Texture(const std::string& name) : Object(name), m_reserved{ std::make_unique<Texture::Reserved>() } {
	//Texture::setAnisotropicFilter();
}

ENG_API void Texture::setAnisotropicFilter() {
	if (!Texture::isAnisotropicAlreadySet) {
		if (strstr((const char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
		{
			Texture::isAnisotropicSupported = true;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Texture::anisotropicLevel);

		}
		Texture::isAnisotropicAlreadySet = true;
	}
}

ENG_API Texture::~Texture() {
	if (this->m_reserved->m_bitmap != nullptr)
		FreeImage_Unload(this->m_reserved->m_bitmap);
	glDeleteTextures(1, &this->m_reserved->m_textureId);
}

ENG_API void Texture::render(const glm::mat4& matrix) {
	if (this->m_reserved->m_bitmap == nullptr) {
		std::cerr << "Texture not loaded or bitmap is nullptr!" << std::endl;
		return;
	}

	if (this->m_reserved->m_textureId == 0) {
		glGenTextures(1, &this->m_reserved->m_textureId);
		glBindTexture(GL_TEXTURE_2D, this->m_reserved->m_textureId);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (Texture::isAnisotropicSupported)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Texture::anisotropicLevel);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
			FreeImage_GetWidth(this->m_reserved->m_bitmap),
			FreeImage_GetHeight(this->m_reserved->m_bitmap),
			GL_BGRA_EXT, GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(this->m_reserved->m_bitmap));


	}

	glBindTexture(GL_TEXTURE_2D, this->m_reserved->m_textureId);

}




const ENG_API unsigned int Texture::parse(const char* data, unsigned int& position) {
	FreeImage_Initialise();
	std::string filename = "textures/" + this->getName();

	this->m_reserved->m_bitmap = FreeImage_Load(FreeImage_GetFileType(filename.c_str(), 0), filename.c_str());
	if (!this->m_reserved->m_bitmap) {
		std::cerr << "Impossibile caricare il file: " << filename << std::endl;
		FreeImage_DeInitialise();
		return 0;
	}

	FreeImage_FlipVertical(this->m_reserved->m_bitmap);

	FreeImage_DeInitialise();
	return 0;
}

