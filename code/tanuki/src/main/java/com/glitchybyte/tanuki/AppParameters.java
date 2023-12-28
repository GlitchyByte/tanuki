// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.json.GJson;

import java.nio.file.Path;

public final class AppParameters {

    private final boolean isValid;
    private final int errorCode;
    private String command;
    private Path projectRoot;
    private TanukiInput input;

    public AppParameters(final String[] args) {
        if (args.length < 1) {
            errorCode = 0;
            isValid = false;
            return;
        }
        command = args[0];
        if (!command.equals("build") && !command.equals("watch")) {
            errorCode = 1;
            isValid = false;
            return;
        }
        final String configFile = args.length < 2 ? "./tanuki.json" : args[1];
        if (args.length > 2) {
            errorCode = 1;
            isValid = false;
            return;
        }
        final Path path = GPaths.getFullPath(configFile);
        projectRoot = path.getParent();
        input = GJson.fromPath(path, TanukiInput.class);
        errorCode = 0;
        isValid = true;
    }

    public boolean isValid() {
        return isValid;
    }

    public int getErrorCode() {
        return errorCode;
    }

    public String getCommand() {
        return command;
    }

    public Path getProjectRoot() {
        return projectRoot;
    }

    public TanukiInput getInput() {
        return input;
    }
}
