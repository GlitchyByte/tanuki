// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.log.GLog;
import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Path;

public final class App implements Runnable {

    public static void main(final String[] args) {
        GLog.setupDefaultRootConsoleHandler(true);
        if (args.length == 0) {
            printUsage();
            return;
        }
        final AppParameters parameters = new AppParameters(args);
        try {
            parameters.validate();
        } catch (final IllegalArgumentException e) {
            GTerminal.println(GTerminal.text(e.getMessage(), Colors.error));
            System.exit(1);
        }
        final App app = new App(parameters.getCommand(), parameters.getProjectRoot(), parameters.getConfig());
        app.run();
    }

    private static void printUsage() {
        final String app = GTerminal.text("tanuki", Colors.cmd);
        final String build = GTerminal.text("build", Colors.arg);
        final String watch = GTerminal.text("watch", Colors.arg);
        final String config = GTerminal.text("config_file", Colors.arg);
        final String help = GTerminal.text("-h, --help", Colors.arg);
        GTerminal.print("""
                Watch and build a project portfolio.
                Usage:
                  %s <%s|%s> [%s]

                  %s       Force builds projects.
                  %s       Watches projects for changes and builds them.
                  %s Tanuki config file. If empty, "tanuki.json" in
                                the current directory is used.
                  %s  Show usage.
                """, app, build, watch, config, build, watch, config, help);
    }

    private final String command;
    private final Path projectRoot;
    private final TanukiConfig input;

    public App(final String command, final Path projectRoot, final TanukiConfig input) {
        this.command = command;
        this.projectRoot = projectRoot;
        this.input = input;
    }

    @Override
    public void run() {
        switch (command) {
            case "-h", "--help" -> printUsage();
            case "build" -> new BuildCommand(projectRoot, input).run();
            case "watch" -> new WatchCommand(projectRoot, input).run();
        }
    }
}
