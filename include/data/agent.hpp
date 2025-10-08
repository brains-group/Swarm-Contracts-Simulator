#pragma once

#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include <common/macros.hpp>
#include <data/controller.hpp>
#include <data/part.hpp>
#include <data/transform.hpp>

#define ADD_COMPONENT_HELPER(TYPE, ...)                                           \
    [[nodiscard]] auto get##TYPE() __VA_OPT__(const) -> __VA_OPT__(const) TYPE& { \
        return *m_##TYPE;                                                         \
    }

#define ADD_COMPONENT(TYPE)                                                      \
                                                                                 \
public:                                                                          \
    [[nodiscard]] auto has##TYPE() const -> bool { return m_##TYPE != nullptr; } \
    auto               set##TYPE(std::shared_ptr<TYPE> TYPE##_) -> TYPE& {       \
        m_##TYPE = std::move(TYPE##_);                             \
        return get##TYPE();                                        \
    }                                                                            \
    ADD_COMPONENT_HELPER(TYPE)                                                   \
    ADD_COMPONENT_HELPER(TYPE, const)                                            \
                                                                                 \
private:                                                                         \
    std::shared_ptr<TYPE> m_##TYPE;

#define COMPONENT_VIEW_HELPER(TYPE, ...)                                         \
    inline constexpr auto __VA_OPT__(Const)##TYPE##Adapter =                     \
        std::views::filter(&Agent::has##TYPE)                                    \
        | std::views::transform(                                                 \
            static_cast<__VA_OPT__(const) TYPE& (Agent::*)() __VA_OPT__(const)>( \
                &Agent::get##TYPE));                                             \
    using __VA_OPT__(Const)##TYPE##View =                                        \
        decltype(std::declval<__VA_OPT__(const) std::vector<Agent>&>()           \
                 | __VA_OPT__(Const)##TYPE##Adapter)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COMPONENT_VIEW(TYPE)            \
    COMPONENT_VIEW_HELPER(TYPE, const); \
    COMPONENT_VIEW_HELPER(TYPE);

#define ADD_MOVE_PTR_WRAPPER(TYPE) std::shared_ptr<TYPE> TYPE##_
#define ASSIGN_MEMBER(TYPE)        m_##TYPE(std::move(TYPE##_))
#define MAKE_COMPONENT(TYPE)                                                            \
    template <class... Args> auto Make##TYPE(Args&&... args) -> std::shared_ptr<TYPE> { \
        return std::make_unique<TYPE>(std::forward<Args>(args)...);                     \
    }

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define AGENT_CLASS(...)                                                 \
    class Agent {                                                        \
    public:                                                              \
        DEFAULT_CTOR_DTOR(Agent);                                        \
        DEFAULT_COPY_MOVE(Agent);                                        \
        explicit Agent(FOR_EACH_LIST(ADD_MOVE_PTR_WRAPPER, __VA_ARGS__)) \
            : FOR_EACH_LIST(ASSIGN_MEMBER, __VA_ARGS__) {}               \
        FOR_EACH(ADD_COMPONENT, __VA_ARGS__)                             \
    };                                                                   \
    FOR_EACH(COMPONENT_VIEW, __VA_ARGS__)                                \
    FOR_EACH(MAKE_COMPONENT, __VA_ARGS__)

namespace scs::data {

using Balance = uint64_t;

// NOLINTBEGIN
AGENT_CLASS(Controller, Transform, Part, Balance)
// NOLINTEND

}    // namespace scs::data

#undef ADD_COMPONENT
#undef ADD_COMPONENT_HELPER

#undef COMPONENT_VIEW
#undef COMPONENT_VIEW_HELPER

#undef ADD_MOVE_PTR_WRAPPER
#undef ASSIGN_MEMBER
#undef MAKE_COMPONENT

#undef AGENT_CLASS
