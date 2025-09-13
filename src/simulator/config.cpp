#include <simulator/config.hpp>

namespace {
class ConfigImpl : public sim::Config {};
}    // namespace

auto sim::Config::create() -> std::unique_ptr<Config> { return std::make_unique<ConfigImpl>(); }
