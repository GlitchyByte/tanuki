// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_PROCESS

#include <string>
#include <string_view>
#include <filesystem>
#include <functional>
#include <deque>

namespace gb::process {

    /**
     * Executes a command and optionally captures its output.
     * <p>
     * Output can be filtered before capture, allowing the caller to even process the lines
     * as they come and not capture. It is advised that the filter lambda be as fast as possible
     * so as to not hold the executed process.
     *
     * @param command Full command line as it would be on a terminal.
     * @param workDir Directory to run the command on.
     * @param lines Optional deque to accumulate lines into.
     * @param exitCode Optional exit code from command.
     * @param filter Optional lambda to filter or process each line.
     *              filter must return true to add the line to lines, false to skip.
     * @return True if command execution was successful.
     */
    bool execute(std::string_view const& command, std::filesystem::path const* workDir = nullptr,
            std::deque<std::string>* lines = nullptr, int* exitCode = nullptr,
            std::function<bool(std::string const&)>const& filter = nullptr);
}

#endif // GB_PROCESS
