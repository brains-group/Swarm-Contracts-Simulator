#include <simulator/config.hpp>

namespace {
class ConfigImpl : public sim::Config {};
}    // namespace

auto sim::Config::create() -> std::shared_ptr<Config> { return std::make_unique<ConfigImpl>(); }
