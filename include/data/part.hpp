#pragma once

#include <optional>
#include <vector>

#include <data/material.hpp>

namespace scs::data {

using Part = std::vector<std::optional<data::Material>>;

}
