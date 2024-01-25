// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>
#include <string_view>

class App {
public:
    static int run(std::vector<std::string_view> const& args) noexcept;
};
