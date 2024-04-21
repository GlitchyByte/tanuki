// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include "gb.h"
#include "resources.h"
#include "Colors.h"
#include "AppParams.h"
#include "Command.h"
#include "ImmediateCommand.h"
#include "RunCommand.h"
#include "WatchCommand.h"
#include "App.h"

void printUsage() noexcept {
    std::string const text { gb::strings::unindent(R"===(
        ${version}
        Watch directories and run actions when they are modified.
        Usage:
          ${app} <${option1}|${option2}> [${config_file}]
          ${app} -i ${watch_dir} [${action}]

          ${option1}         Runs actions in all modules.
          ${option2}       Watches modules for changes and runs actions on them.
          ${config_file} Tanuki config file. If empty, "tanuki.json" in the current
                        directory is used.
          -i          Immediate mode. In this mode tanuki watches a single directory and
                        runs the action when it is modified. if no action is specified
                        it simply exits.
          ${watch_dir}   Directory to watch.
          ${action}      Action to run when directory is modified.
        )===") };
    std::string version { "v" };
    version += resources::appVersion;
    std::string const usage {
        gb::ReplaceableVars()
            .add("version", gb::terminal::colorText(version, Colors::text))
            .add("app", gb::terminal::colorText("tanuki", Colors::cmd))
            .add("option1", gb::terminal::colorText("run", Colors::arg))
            .add("option2", gb::terminal::colorText("watch", Colors::arg))
            .add("config_file", gb::terminal::colorText("config_file", Colors::arg))
            .add("watch_dir", gb::terminal::colorText("watch_dir", Colors::arg))
            .add("action", gb::terminal::colorText("action", Colors::arg))
            .replace(text)
    };
    std::cout << usage;
}

void printConfigExample() noexcept {
    std::string const text { gb::strings::unindent(R"===(
          Tanuki json configuration file schema is as follows:
            {
              "modules": [
                {
                  "name": "My module name",
                  "rootDir": "path/to/module",
                  "watchDirs": [
                    "watched1",
                    "subdir/watched2"
                  ],
                  "action": "echo 'This will run in rootDir.'"
                }
              ]
            }

        )===") };
    std::cout << text;
}

void printError(std::string_view const& error) noexcept {
    std::cerr << gb::terminal::colorText(error, Colors::error) << std::endl;
}

int App::run(std::vector<std::string_view> const& args) noexcept {
    if (args.size() <= 1) {
        printUsage();
        std::cout << std::endl;
        printConfigExample();
        return 0;
    }
    AppParams const params { args };
    if (!params.isValid()) {
        printError(params.getError());
        printUsage();
        return 1;
    }
    std::unique_ptr<Command> command;
    try {
        if (params.isImmediate()) {
            command = std::make_unique<ImmediateCommand>(params.getWatchDir(), params.getAction());
        } else if (params.getCommand() == "run") {
            command = std::make_unique<RunCommand>(params.getConfigFile());
        } else { //if (params.getCommand() == "watch") {
            command = std::make_unique<WatchCommand>(params.getConfigFile());
        }
    } catch (std::exception const& e) {
        printError(e.what());
        printConfigExample();
        return 2;
    }
    try {
        command->execute();
    } catch (std::exception const& e) {
        printError(e.what());
        return 3;
    }
    std::cout << std::endl << gb::terminal::colorText("Done!", Colors::success) << std::endl;
    return 0;
}
