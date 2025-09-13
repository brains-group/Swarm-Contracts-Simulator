#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <common/macros.hpp>
#include <common/result.hpp>
#include <common/singleton.hpp>

#define LOG(LEVEL) \
    if (false)     \
        ;          \
    else           \
        StreamLogger(LogLevel::LEVEL, __FILE__, __LINE__, __func__)

ENUM(LogLevel, uint8_t, FINE, DEBUG, INFO, WARN, ERROR, FATAL);

struct LogMessage {
    LogLevel                              level;
    std::string                           msg;
    std::string_view                      file;
    int                                   line;
    std::string_view                      func;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id                       thread;
};

class LogManager : public Singleton<LogManager> {
public:
    DEFAULT_CTOR_DTOR(LogManager);
    DELETE_COPY_MOVE(LogManager);

    using HandlerID = uint64_t;
    using Handler   = std::function<void(LogMessage)>;

    struct HandlerWithID {
        Handler   handler;
        HandlerID id;
    };

    // TODO: Make this lockless / parallel?
    auto submit(LogMessage& msg) -> void {
        std::lock_guard<std::mutex> lock(m_handlerMutex);
        for (auto& handler : m_handlers) { handler.handler(msg); }
    }

    auto addHandler(auto&& handler) -> HandlerID {
        std::lock_guard<std::mutex> lock(m_handlerMutex);
        const HandlerID             newID = m_nextID++;
        m_handlers.emplace_back(
            HandlerWithID{.handler = std::forward<decltype(handler)>(handler), .id = newID});
        return newID;
    }
    auto removeHandler(HandlerID handlerID) -> void {
        std::lock_guard<std::mutex> lock(m_handlerMutex);
        std::erase_if(m_handlers,
                      [handlerID](const auto& handler) -> bool { return handler.id == handlerID; });
    }

private:
    std::mutex                 m_handlerMutex;
    HandlerID                  m_nextID = 0;
    std::vector<HandlerWithID> m_handlers;
};

class StreamLogger {
public:
    DELETE_CTOR_COPY_MOVE(StreamLogger);
    StreamLogger(LogLevel level, std::string_view file, int line, std::string_view func)
        : m_msg{.level     = level,
                .msg       = {},
                .file      = file,
                .line      = line,
                .func      = func,
                .timestamp = std::chrono::system_clock::now(),
                .thread    = std::this_thread::get_id()} {}

    ~StreamLogger() {
        m_msg.msg = m_stream.str();
        LogManager::instance().submit(m_msg);
    }

    auto operator<<(const auto& value) -> StreamLogger& {
        m_stream << value;
        return *this;
    }

private:
    std::ostringstream m_stream;
    LogMessage         m_msg;
};

class ScopedHandlerWrapper {
public:
    DELETE_CTOR_COPY_MOVE(ScopedHandlerWrapper);

    explicit ScopedHandlerWrapper(auto&& handler)
        : m_handlerID(LogManager::instance().addHandler(std::forward<decltype(handler)>(handler))) {
    }
    ~ScopedHandlerWrapper() { LogManager::instance().removeHandler(m_handlerID); }

private:
    LogManager::HandlerID m_handlerID;
};

inline auto simpleConsoleLogger(const LogMessage& msg) -> void {
    std::cout << to_string(msg.level).value_or("UNKNOWN") << " - " << msg.file << ":" << msg.line
              << " " << msg.func << ": " << msg.msg << "\n";
}
