// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.terminal.GTerminal;

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
}
