// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_RESHELPER

#include <string_view>

namespace gb::reshelper {

    /**
     * Returns the first line of an embedded text resource, without any line ending.
     *
     * @param str Raw embedded string.
     * @return The first line of an embedded text resource.
     */
    consteval std::string_view getFirstLine(std::string_view const str) {
        auto const count = str.find('\n');
        return count == std::string_view::npos ?
            str.substr(0, str.length()) :
            str.substr(0, count);
    }
}

#endif // GB_RESHELPER
