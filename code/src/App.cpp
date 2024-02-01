// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include "gb.h"
#include "AppParams.h"
#include "Command.h"
#include "ImmediateCommand.h"
#include "RunCommand.h"
#include "WatchCommand.h"
#include "App.h"

gb::console::color_t const cmdColor { gb::console::rgb(3, 5, 3) };
gb::console::color_t const argColor { gb::console::rgb(0, 2, 0) };
gb::console::color_t const errorColor { gb::console::rgb(5, 1, 1) };
gb::console::color_t const doneColor { gb::console::rgb(1, 5, 3) };

void printUsage() noexcept {
    std::string const text { gb::strings::unindent(R"===(
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
    std::string const usage { gb::ReplaceableVars()
            .add("app", gb::console::colorText("tanuki", cmdColor))
            .add("option1", gb::console::colorText("run", argColor))
            .add("option2", gb::console::colorText("watch", argColor))
            .add("config_file", gb::console::colorText("config_file", argColor))
            .add("watch_dir", gb::console::colorText("watch_dir", argColor))
            .add("action", gb::console::colorText("action", argColor))
            .replace(text) };
    std::cout << usage;
}

void printError(std::string_view const& error) noexcept {
    std::cerr << gb::console::colorText(error, errorColor) << std::endl;
}

int App::run(std::vector<std::string_view> const& args) noexcept {
    if (args.size() <= 1) {
        printUsage();
        return 0;
    }
    AppParams const params { args };
    if (!params.isValid()) {
        printError(params.getError());
        printUsage();
        return 1;
    }
    std::unique_ptr<Command> command;
    if (params.isImmediate()) {
        command = std::make_unique<ImmediateCommand>(params.getWatchDir(), params.getAction());
    } else if (params.getCommand() == "run") {
        command = std::make_unique<RunCommand>();
    } else { //if (params.getCommand() == "watch") {
        command = std::make_unique<WatchCommand>();
    }
    command->run();
    std::cout << std::endl << gb::console::colorText("Done!", doneColor) << std::endl;
    return 0;
}
