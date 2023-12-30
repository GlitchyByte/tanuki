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

public final class BuildCommand extends Command {

    public BuildCommand(final Path projectRoot, final TanukiConfig config) {
        super(projectRoot, config);
    }

    @Override
    public void run() {
        for (final var subproject: config.projects) {
            final Path subprojectDir = GPaths.getFullPath(projectRoot.resolve(subproject.projectDir));
            if (!Files.isDirectory(subprojectDir)) {
                panic("Subproject directory not found: " + subprojectDir);
            }
            // Build.
            GTerminal.println("%s: %s (%s)",
                    GTerminal.text("Building", Colors.heading),
                    GTerminal.text(subproject.name, Colors.highlight),
                    GTerminal.text(subproject.build, Colors.text)
            );
            final GProcessResult buildResult = GOSInterface.instance.executeWithResult(
                    GOSInterface.instance.makeCommand(subproject.build), subprojectDir);
            buildResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, Colors.text)));
            if (!GOSInterface.instance.isSuccessfulExitCode(buildResult.exitCode)) {
                panic(GStrings.format("Build error! (code: %d)", buildResult.exitCode));
            }
            // Copy.
            GTerminal.println("%s: %s (%s)",
                    GTerminal.text("Copying", Colors.heading),
                    GTerminal.text(subproject.name, Colors.highlight),
                    GTerminal.text(subproject.copy, Colors.text)
            );
            final GProcessResult copyResult = GOSInterface.instance.executeWithResult(
                    GOSInterface.instance.makeCommand(subproject.copy), projectRoot);
            copyResult.output.forEach(line -> GTerminal.println(GTerminal.text(line, Colors.text)));
            if (!GOSInterface.instance.isSuccessfulExitCode(copyResult.exitCode)) {
                panic(GStrings.format("Copy error! (code: %d)", copyResult.exitCode));
            }
        }
    }
}
