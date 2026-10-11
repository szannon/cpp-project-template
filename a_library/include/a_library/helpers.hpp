#pragma once

#include <fmt/core.h>

#include <boost/stacktrace.hpp>
#include <a_library/essentials.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace a_library {
template <CanConstruct<std::string> Msg>
std::string make_stacktrace_str(Msg&& msg) {
    return fmt::format(
        "[{}] Stacktrace:\n{}",
        std::forward<Msg>(msg),
        boost::stacktrace::to_string(boost::stacktrace::stacktrace())
    );
}

class Exception : public std::runtime_error {
   public:
    template <CanConstruct<std::string> Msg>
    Exception(Msg&& msg)
        : std::runtime_error(make_stacktrace_str(std::forward<Msg>(msg))) {}
};

}  // namespace a_library
