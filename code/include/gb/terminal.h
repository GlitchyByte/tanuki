// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_TERMINAL

#include <string>

namespace gb::terminal {

    typedef uint32_t color_t;

    #define CODE "@"
    #define CSI "\u001b["
    #define CSI_COLOR_RESET CSI "0m"
    #define CSI_COLOR_FOREGROUND CSI "38;5;" CODE "m"

    /**
     * Converts rgb values into a single color code.
     *
     * @param r Red value between [0, 6).
     * @param g Green value between [0, 6).
     * @param b Blue value between [0, 6).
     * @return The color code.
     */
    [[nodiscard]]
    color_t rgb(color_t const r, color_t const g, color_t const b) noexcept;

    /**
     * Converts a grey step into a color code.
     *
     * @param step Grey step between [0, 24).
     * @return The color code.
     */
    [[nodiscard]]
    color_t grey(color_t const step) noexcept;

    /**
     * Returns a string that represents the given string in the given color.
     *
     * @param str String to apply color.
     * @param color Color.
     * @return The string in color.
     */
    [[nodiscard]]
    std::string colorText(std::string_view const& str, color_t const color) noexcept;
}

#endif // GB_TERMINAL
