// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/ReplaceableVars.h"
#ifdef GB_REPLACEABLE_VARS

#include <sstream>

namespace gb {

    ReplaceableVars::ReplaceableVars(ReplaceableVars const& other) noexcept {
        vars = other.vars;
    }

    ReplaceableVars::ReplaceableVars(ReplaceableVars&& other) noexcept {
        vars = std::move(other.vars);
    }

    ReplaceableVars& ReplaceableVars::operator=(ReplaceableVars const& other) noexcept {
        if (this != &other) {
            vars = other.vars;
        }
        return *this;
    }

    ReplaceableVars& ReplaceableVars::operator=(ReplaceableVars&& other) noexcept {
        if (this != &other) {
            vars = std::move(other.vars);
        }
        return *this;
    }

    ReplaceableVars& ReplaceableVars::add(std::string const& name, std::string const& value) noexcept {
        std::string const decoratedName = "${" + name + "}";
        vars[decoratedName] = value;
        return *this;
    }

    std::string ReplaceableVars::replace(std::string_view const& str) const noexcept {
        std::string replaced { str };
        for (auto const& entries: vars) {
            std::string const& name { entries.first };
            std::string const& value { entries.second };
            size_t pos { replaced.find(name) };
            while (pos != std::string::npos) {
                replaced.replace(pos, name.length(), value);
                pos = replaced.find(name, pos + value.length());
            }
        }
        return replaced;
    }

    std::string ReplaceableVars::string() const noexcept {
        std::ostringstream ss;
        ss << "{ ";
        bool first = true;
        for (auto const& entries: vars) {
            if (first) {
                first = false;
            } else {
                ss << ", ";
            }
            ss << entries.first << ": " << entries.second;
        }
        ss << " }";
        return ss.str();
    }
}

#endif // GB_REPLACEABLE_VARS
