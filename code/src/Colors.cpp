// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "Colors.h"

namespace Colors {
    gb::terminal::color_t const text { gb::terminal::rgb(1, 1, 1) };
    gb::terminal::color_t const highlight { gb::terminal::rgb(5, 5, 5) };
    gb::terminal::color_t const cmd { gb::terminal::rgb(3, 5, 3) };
    gb::terminal::color_t const arg { gb::terminal::rgb(0, 2, 0) };
    gb::terminal::color_t const error { gb::terminal::rgb(5, 0, 0) };
    gb::terminal::color_t const success { gb::terminal::rgb(1, 5, 3) };
    gb::terminal::color_t const heading { gb::terminal::rgb(1, 5, 3) };
}
