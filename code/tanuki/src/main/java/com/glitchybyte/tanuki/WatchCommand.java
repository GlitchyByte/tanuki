// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Path;

public final class WatchCommand extends Command {

    public WatchCommand(final Path projectRoot, final TanukiConfig config) {
        super(projectRoot, config);
    }

    @Override
    public void run() {
        GTerminal.println("yo!");
    }
}
