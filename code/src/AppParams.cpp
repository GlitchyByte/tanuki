// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "AppParams.h"

AppParams::AppParams(std::vector<std::string_view> const& args) noexcept {
    size_t const size { args.size() };
    command = args[1];
    immediate = command == "-i";
    if (immediate) {
        if (size < 3) {
            error = "watch_dir not provided!";
            valid = false;
            return;
        } else {
            watchDir = gb::files::canonicalPath(args[2]);
            if (!std::filesystem::is_directory(watchDir)) {
                error = "watch_dir doesn't exist!";
                valid = false;
                return;
            }
        }
        if (size < 4) {
            action.reset();
        } else {
            action = args[3];
        }
        if (size > 4) {
            error = "Too many arguments! ";
            valid = false;
            return;
        }
        valid = true;
        return;
    }
    if ((command != "run") && (command != "watch")) {
        error = "Invalid command!";
        valid = false;
        return;
    }
    std::string const filename { size < 3 ? "./tanuki.json" : args[2] };
    configFile = gb::files::canonicalPath(filename);
    if (!std::filesystem::is_regular_file(configFile)) {
        error = "config_file doesn't exist!";
        valid = false;
        return;
    }
    if (size > 3) {
        error = "Too many arguments! ";
        valid = false;
        return;
    }
    valid = true;
}

bool AppParams::isValid() const noexcept {
    return valid;
}

std::string AppParams::getError() const noexcept {
    return error;
}

bool AppParams::isImmediate() const noexcept {
    return immediate;
}

std::string AppParams::getCommand() const noexcept {
    return command;
}

std::filesystem::path AppParams::getConfigFile() const noexcept {
    return configFile;
}

std::filesystem::path AppParams::getWatchDir() const noexcept {
    return watchDir;
}

std::optional<std::string> AppParams::getAction() const noexcept {
    return action;
}

std::string AppParams::string() const noexcept {
    std::ostringstream ss;
    ss << std::boolalpha
        << "{ valid: " << valid
        << ", immediate: " << immediate
        << ", command: " << command
        << ", configFile: " << configFile
        << ", watchDir: " << watchDir
        << ", action: " << action.value_or("<none>")
        << " }";
    return ss.str();
}
