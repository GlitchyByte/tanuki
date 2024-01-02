// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GStrings;
import com.glitchybyte.glib.process.GOSInterface;

import java.nio.file.Path;

public final class BuildCommand extends Command {

    public BuildCommand(final Path projectRoot, final TanukiConfig config) {
        super(projectRoot, config);
    }

    @Override
    public void run() {
        for (final var subproject: config.projects) {
            // Build.
            final Integer buildExitCode = build(subproject);
            if (!GOSInterface.instance.isSuccessfulExitCode(buildExitCode)) {
                panic(GStrings.format("Build error! (code: %d)", buildExitCode));
            }
            // Copy.
            final Integer copyExitCode = copy(subproject);
            if (!GOSInterface.instance.isSuccessfulExitCode(copyExitCode)) {
                panic(GStrings.format("Copy error! (code: %d)", copyExitCode));
            }
        }
    }
}
