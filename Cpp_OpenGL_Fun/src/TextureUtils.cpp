#include "TextureUtils.h"
#include "ResourceManager.h"

namespace OpenGLFun {
	namespace TextureUtils {
		ImTextureID GetImGuiTexId(std::string const& textureName) {
			return reinterpret_cast<ImTextureID>(static_cast<intptr_t>(RESOURCE_MANAGER->GetTexture(textureName)->mGLTextureId));
		}
	}
}