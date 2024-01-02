// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.process.GOSInterface;
import com.glitchybyte.glib.process.GProcessResult;
import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Files;
import java.nio.file.Path;

public abstract class Command implements Runnable {

    protected final Path projectRoot;
    protected final TanukiConfig config;

    public Command(final Path projectRoot, final TanukiConfig config) {
        this.projectRoot = projectRoot;
        this.config = config;
    }

    protected void panic(final String message) {
        GTerminal.println(GTerminal.text(message, Colors.error));
        System.exit(2);
    }

    protected Integer build(final TanukiConfig.Project subproject) {
        final Path subprojectDir = GPaths.getFullPath(projectRoot.resolve(subproject.projectDir));
        if (!Files.isDirectory(subprojectDir)) {
            panic("Subproject directory not found: " + subprojectDir);
        }
        GTerminal.println("%s: %s (%s)",
                GTerminal.text("Building", Colors.heading),
                GTerminal.text(subproject.name, Colors.highlight),
                GTerminal.text(subproject.build, Colors.text)
        );
        final GProcessResult buildResult = GOSInterface.instance.executeWithResult(
                GOSInterface.instance.makeCommand(subproject.build), subprojectDir);
        buildResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, Colors.text)));
        return buildResult.exitCode;
    }

    protected Integer copy(final TanukiConfig.Project subproject) {
        GTerminal.println("%s: %s (%s)",
                GTerminal.text("Copying", Colors.heading),
                GTerminal.text(subproject.name, Colors.highlight),
                GTerminal.text(subproject.copy, Colors.text)
        );
        final GProcessResult copyResult = GOSInterface.instance.executeWithResult(
                GOSInterface.instance.makeCommand(subproject.copy), projectRoot);
        copyResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, Colors.text)));
        return copyResult.exitCode;
    }
}
