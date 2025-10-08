#pragma once

#include <data/material.hpp>
#include <data/rect.hpp>

namespace scs::data {

struct MaterialStore {
    Material material;
    Rect     space;
};

}    // namespace scs::data
