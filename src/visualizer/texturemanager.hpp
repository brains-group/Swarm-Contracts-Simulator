#pragma once

#include <filesystem>
#include <unordered_map>
#include <utility>

#include <SFML/Graphics.hpp>
#include <common/logger.hpp>

namespace scs::vis {

class AssetManager {
public:
    explicit AssetManager(std::filesystem::path basePath)
        : m_assetPath(std::move(basePath)) {}

    auto getTexture(const std::filesystem::path& path) -> const sf::Texture& {
        std::filesystem::path fullPath = m_assetPath / path;
        if (auto it = m_loadedTextures.find(fullPath); it != m_loadedTextures.end()) {
            return it->second;
        }
        if (!m_loadedTextures[fullPath].loadFromFile(fullPath)) {
            LOG(ERROR) << "Failed to load texture: " << fullPath;
        } else {
            m_loadedTextures[fullPath].setSmooth(true);
        }
        return m_loadedTextures[fullPath];
    }

private:
    std::filesystem::path m_assetPath;

    std::unordered_map<std::filesystem::path, sf::Texture> m_loadedTextures;
};

}    // namespace scs::vis
