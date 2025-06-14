#pragma once

enum class RenderingType {
    FORWARD_RENDERING,
    DEFERRED_RENDERING
};

class RenderingTypeHelper {
public:
    static inline std::string ToString(RenderingType type) {
        switch (type) {
            case RenderingType::FORWARD_RENDERING: return "Forward Rendering";
            case RenderingType::DEFERRED_RENDERING: return "Deferred Rendering";
            default: {
                std::cerr << "Incorrect Rendering Type";
                return "";
            }
        }
    }
};