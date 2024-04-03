// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include <optional>

class AppParams {
private:
    bool valid;
    std::string error;
    bool immediate;
    std::string command;
    std::filesystem::path configFile;
    std::filesystem::path watchDir;
    std::optional<std::string> action;

public:
    explicit AppParams(std::vector<std::string_view> const& args) noexcept;

    [[nodiscard]]
    bool isValid() const noexcept;

    [[nodiscard]]
    std::string getError() const noexcept;

    [[nodiscard]]
    bool isImmediate() const noexcept;

    [[nodiscard]]
    std::string getCommand() const noexcept;

    [[nodiscard]]
    std::filesystem::path getConfigFile() const noexcept;

    [[nodiscard]]
    std::filesystem::path getWatchDir() const noexcept;

    [[nodiscard]]
    std::optional<std::string> getAction() const noexcept;

    [[nodiscard]]
    std::string string() const noexcept;
};
