// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/terminal.h"
#ifdef GB_TERMINAL

#include <cassert>
#include <sstream>

namespace gb::terminal {

    color_t rgb(color_t const r, color_t const g, color_t const b) noexcept {
        assert(r < 6);
        assert(g < 6);
        assert(b < 6);
        return 16 + (36 * r) + (6 * g) + b;
    }

    color_t grey(color_t const step) noexcept {
        assert(step < 24);
        return 232 + step;
    }

    std::string colorText(std::string_view const& str, color_t const color) noexcept {
        if (str.empty()) {
            return "";
        }
        auto const startColor { gb::strings::replace(CSI_COLOR_FOREGROUND, CODE, std::to_string(color)) };
        std::ostringstream ss;
        ss << startColor << str << CSI_COLOR_RESET;
        return ss.str();
    }
}

#endif // GB_TERMINAL
