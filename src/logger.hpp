#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include "macros.hpp"
#include "result.hpp"
#include "singleton.hpp"

#define LOG(LEVEL)                                              \
    if (LogManager::instance().getLogLevel() > LogLevel::LEVEL) \
        ;                                                       \
    else                                                        \
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
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& handler : m_handlers) { handler.handler(msg); }
    }

    auto addHandler(auto&& handler) -> HandlerID {
        std::lock_guard<std::mutex> lock(m_mutex);
        const HandlerID             newID = m_nextID++;
        m_handlers.emplace_back(
            HandlerWithID{.handler = std::forward<decltype(handler)>(handler), .id = newID});
        return newID;
    }
    auto removeHandler(HandlerID handlerID) -> void {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::erase_if(m_handlers,
                      [handlerID](const auto& handler) -> bool { return handler.id == handlerID; });
    }

    [[nodiscard]] auto getLogLevel() const -> LogLevel {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_level;
    }

    auto setLogLevel(LogLevel level) -> void {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_level = level;
    }

private:
    mutable std::mutex         m_mutex;
    HandlerID                  m_nextID = 0;
    std::vector<HandlerWithID> m_handlers;

    LogLevel m_level = LogLevel::INFO;
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
        if (m_msg.level == LogLevel::FATAL) { std::quick_exit(1); }
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
    std::size_t      end = msg.file.find_last_of('/');
    std::string_view shortPath =
        end != std::string_view::npos ? msg.file.substr(end + 1) : msg.file;

    std::cout << to_string(msg.level).value_or("UNKNOWN") << " - " << shortPath << ":" << msg.line
              << " " << msg.func << ": " << msg.msg << "\n";
}
