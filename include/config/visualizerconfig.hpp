#pragma once

#include <filesystem>

#include <common/macros.hpp>
#include <data/rect.hpp>

namespace scs::config {

class VisualizerConfig {
public:
    INTERFACE_CTOR_DTORS(VisualizerConfig);

    [[nodiscard]] virtual auto assetBasePath() const -> const std::filesystem::path&      = 0;
    [[nodiscard]] virtual auto agentTexturePath() const -> const std::filesystem::path&   = 0;
    [[nodiscard]] virtual auto stationTexturePath() const -> const std::filesystem::path& = 0;

    [[nodiscard]] virtual auto windowWidth() const -> unsigned int    = 0;
    [[nodiscard]] virtual auto windowHeight() const -> unsigned int   = 0;
    [[nodiscard]] virtual auto windowName() const -> std::string      = 0;
    [[nodiscard]] virtual auto framerateLimit() const -> unsigned int = 0;

    [[nodiscard]] virtual auto getOrderQueueArea() const -> const data::Rect& = 0;
};

}    // namespace scs::config
