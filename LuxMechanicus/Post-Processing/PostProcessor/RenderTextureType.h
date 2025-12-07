#pragma once

enum class RenderTextureType {
	COLOR,
	BRIGHT,
    G_POSITION,
    G_NORMAL,
    G_ALBEDO_SPEC,
	NONE
};


class RenderTextureTypeHelper {
public:
    static inline std::string ToString(RenderTextureType type) {
        switch (type) {
            case RenderTextureType::COLOR: return "ColorMap";
            case RenderTextureType::BRIGHT: return "BrightMap";
            case RenderTextureType::NONE: return "None";
            case RenderTextureType::G_POSITION: return "gPosition";
            case RenderTextureType::G_NORMAL: return "gNormal";
            case RenderTextureType::G_ALBEDO_SPEC: return "gAlbedoSpec";
            default: {
                std::cerr << "Incorrect Texture Type";
                return "";
            }
        }
    }
};