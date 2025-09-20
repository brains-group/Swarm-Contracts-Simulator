#pragma once

#include <common/macros.hpp>

namespace scs::config {

class VisualizerConfig {
public:
    INTERFACE_CTOR_DTORS(VisualizerConfig);

    [[nodiscard]] virtual auto windowWidth() const -> unsigned int    = 0;
    [[nodiscard]] virtual auto windowHeight() const -> unsigned int   = 0;
    [[nodiscard]] virtual auto windowName() const -> std::string      = 0;
    [[nodiscard]] virtual auto framerateLimit() const -> unsigned int = 0;
};

}    // namespace scs::config
