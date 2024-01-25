// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/process.h"
#ifdef GB_PROCESS

#include <cstdio>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WIFEXITED(x) (x != -1)
#define WEXITSTATUS(x) ((x >> 8) && 0xff)
#endif

namespace gb::process {

    bool readLine(FILE* file, std::string& line) {
        #ifdef _WIN32
        size_t const eolSize = 2;
        #else
        size_t const eolSize = 1;
        #endif
        char buffer[1024];
        line.clear();
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            line += buffer;
            if (line.ends_with('\n')) {
                line = line.substr(0, line.length() - eolSize);
                return true;
            }
        }
        return false;
    }

    bool execute(std::string_view const& command, std::optional<std::filesystem::path> const& workDir,
            std::deque<std::string>* lines, int* exitCode,
            std::function<bool(std::string const&)> const& filter) {
        auto const& originalDir { std::filesystem::current_path() };
        if (workDir.has_value()) {
            std::filesystem::current_path(workDir.value());
        }
        std::string redirectedCommand { command };
        redirectedCommand += " 2>&1";
        FILE* file { popen(redirectedCommand.c_str(), "r") };
        if (!file) {
            std::filesystem::current_path(originalDir);
            return false;
        }
        std::string line;
        while (readLine(file, line)) {
            if ((lines == nullptr) && (filter == nullptr)) {
                continue;
            }
            if ((filter == nullptr) || filter(line)) {
                if (lines != nullptr) {
                    lines->emplace_back(std::move(line));
                }
            }
        }
        int const closeCode { pclose(file) };
        std::filesystem::current_path(originalDir);
        if (WIFEXITED(closeCode)) {
            if (exitCode != nullptr) {
                *exitCode = WEXITSTATUS(closeCode);
            }
            return true;
        }
        return false;
    }
}

#endif // GB_PROCESS
