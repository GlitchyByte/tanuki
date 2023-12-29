// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Path;

public abstract class Command implements Runnable {

    protected static final int errorColor = GTerminal.rgb(5, 1, 1);
    protected static final int headingColor = GTerminal.rgb(1, 5, 3);
    protected static final int textColor = GTerminal.rgb(1, 1, 1);
    protected static final int highlightColor = GTerminal.rgb(5, 5, 5);

    protected final Path projectRoot;
    protected final TanukiConfig config;

    public Command(final Path projectRoot, final TanukiConfig config) {
        this.projectRoot = projectRoot;
        this.config = config;
    }

    protected void panic(final String message) {
        GTerminal.println(GTerminal.text(message, errorColor));
        System.exit(2);
    }
}
