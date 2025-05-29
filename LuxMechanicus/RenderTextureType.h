#pragma once

enum class RenderTextureType {
	ColorMap,
	BrightMap,
	None
};


class RenderTextureTypeHelper {
public:
    static inline std::string ToString(RenderTextureType type) {
        switch (type) {
            case RenderTextureType::ColorMap: return "ColorMap";
            case RenderTextureType::BrightMap: return "BrightMap";
            case RenderTextureType::None: return "None";
            default: {
                std::cerr << "Incorrect Texture Type";
                return "";
            }
        }
    }
};