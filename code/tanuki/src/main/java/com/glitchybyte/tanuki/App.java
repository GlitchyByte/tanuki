// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.GStrings;
import com.glitchybyte.glib.process.GOSInterface;
import com.glitchybyte.glib.process.GProcessResult;
import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Files;
import java.nio.file.Path;

public final class App implements Runnable {

    public static void main(final String[] args) {
        if (args.length == 0) {
            printUsage();
            return;
        }
        final AppParameters parameters = new AppParameters(args);
        try {
            parameters.validate();
        } catch (final IllegalArgumentException e) {
            GTerminal.println(GTerminal.text(e.getMessage(), errorColor));
            System.exit(1);
        }
        final App app = new App(parameters.getCommand(), parameters.getProjectRoot(), parameters.getInput());
        app.run();
    }

    private static void printUsage() {
        final int cmdColor = GTerminal.rgb(3, 5, 3);
        final int argColor = GTerminal.rgb(0, 2, 0);
        final String app = GTerminal.text("tanuki", cmdColor);
        final String build = GTerminal.text("build", argColor);
        final String watch = GTerminal.text("watch", argColor);
        final String config = GTerminal.text("config_file", argColor);
        GTerminal.print("""
                Watch and build multiple projects.
                Usage:
                  %s <%s|%s> [%s]
        
                  %s       Force builds projects.
                  %s       Watches projects for changes and builds them.
                  %s Tanuki config file. If empty, "tanuki.json" in
                              the current directory is used.
                """, app, build, watch, config, build, watch, config);
    }

    private static final int errorColor = GTerminal.rgb(5, 1, 1);
    private static final int headingColor = GTerminal.rgb(1, 5, 3);
    private static final int textColor = GTerminal.rgb(1, 1, 1);
    private static final int highlightColor = GTerminal.rgb(5, 5, 5);

    private final String command;
    private final Path projectRoot;
    private final TanukiInput input;

    public App(final String command, final Path projectRoot, final TanukiInput input) {
        this.command = command;
        this.projectRoot = projectRoot;
        this.input = input;
    }

    @Override
    public void run() {
        if (command.equals("build")) {
            buildFull();
            return;
        }
        GTerminal.println(GTerminal.text("Yo!", highlightColor));
        GTerminal.println("Is this working?");
    }

    private void panic(final String message) {
        GTerminal.println(GTerminal.text(message, errorColor));
        System.exit(2);
    }

    private void buildFull() {
        for (final var subproject: input.projects) {
            final Path subprojectDir = GPaths.getFullPath(projectRoot.resolve(subproject.projectDir));
            if (!Files.isDirectory(subprojectDir)) {
                panic("Subproject directory not found: " + subprojectDir);
            }
            // Build.
            GTerminal.println("%s: %s (%s)",
                    GTerminal.text("Building", headingColor),
                    GTerminal.text(subproject.name, highlightColor),
                    GTerminal.text(subproject.build, textColor)
            );
            final GProcessResult buildResult = GOSInterface.instance.executeWithResult(
                    GOSInterface.instance.makeCommand(subproject.build), subprojectDir);
            buildResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, textColor)));
            if (!GOSInterface.instance.isSuccessfulExitCode(buildResult.exitCode)) {
                panic(GStrings.format("Build error! (code: %d)", buildResult.exitCode));
            }
            // Copy.
            GTerminal.println("%s: %s (%s)",
                    GTerminal.text("Copying", headingColor),
                    GTerminal.text(subproject.name, highlightColor),
                    GTerminal.text(subproject.copy, textColor)
            );
            final GProcessResult copyResult = GOSInterface.instance.executeWithResult(
                    GOSInterface.instance.makeCommand(subproject.copy), projectRoot);
            copyResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, textColor)));
            if (!GOSInterface.instance.isSuccessfulExitCode(copyResult.exitCode)) {
                panic(GStrings.format("Copy error! (code: %d)", copyResult.exitCode));
            }
        }
    }
}
