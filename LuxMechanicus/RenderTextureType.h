#pragma once

enum class RenderTextureType {
	COLOR,
	BRIGHT,
	NONE
};


class RenderTextureTypeHelper {
public:
    static inline std::string ToString(RenderTextureType type) {
        switch (type) {
            case RenderTextureType::COLOR: return "ColorMap";
            case RenderTextureType::BRIGHT: return "BrightMap";
            case RenderTextureType::NONE: return "None";
            default: {
                std::cerr << "Incorrect Texture Type";
                return "";
            }
        }
    }
};