#include "TextureUtils.h"
#include "ResourceManager.h"

namespace OpenGLFun {
	namespace TextureUtils {
		ImTextureID GetImGuiTexId(std::string const& textureName) {
			return GetImGuiTexId(RESOURCE_MANAGER->GetTexture(textureName)->mGLTextureId);
		}
		ImTextureID GetImGuiTexId(unsigned int& textureId) {
			return reinterpret_cast<ImTextureID>(static_cast<intptr_t>(textureId));
		}
	}
}