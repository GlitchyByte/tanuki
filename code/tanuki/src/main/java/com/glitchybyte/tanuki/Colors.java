// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.terminal.GTerminal;

public final class Colors {

    public static final int cmd = GTerminal.rgb(3, 5, 3);
    public static final int arg = GTerminal.rgb(0, 2, 0);

    public static final int error = GTerminal.rgb(5, 1, 1);
    public static final int heading = GTerminal.rgb(1, 5, 3);
    public static final int text = GTerminal.rgb(1, 1, 1);
    public static final int highlight = GTerminal.rgb(5, 5, 5);

    private Colors() {
        // Hiding constructor.
    }
}
