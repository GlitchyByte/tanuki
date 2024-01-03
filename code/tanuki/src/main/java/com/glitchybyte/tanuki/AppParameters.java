// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.json.GJson;
import com.google.gson.GsonBuilder;

import java.nio.file.Files;
import java.nio.file.Path;

public final class AppParameters {

    private final GJson json = new GJson(new GsonBuilder()
            .registerTypeAdapter(TanukiConfig.class, new TanukiConfig.JsonAdapter())
            .create());
    private final String[] args;
    private String command;
    private Path projectRoot;
    private TanukiConfig config;

    public AppParameters(final String[] args) {
        if (args.length == 0) {
            throw new IllegalArgumentException("Must have arguments!");
        }
        this.args = args;
    }

    public void validate() {
        command = args[0];
        if (command.equals("-h") || command.equals("--help")) {
            return;
        }
        if (!command.equals("build") && !command.equals("watch")) {
            throw new IllegalArgumentException("Command must be 'build' or 'watch'!");
        }
        final String configFile = args.length < 2 ? "./tanuki.json" : args[1];
        if (args.length > 2) {
            throw new IllegalArgumentException("Too many arguments!");
        }
        final Path path = GPaths.getFullPath(configFile);
        if (!Files.isRegularFile(path)) {
            throw new IllegalArgumentException("Tanuki config file not found!");
        }
        projectRoot = path.getParent();
        config = json.fromPath(path, TanukiConfig.class);
    }

    public String getCommand() {
        return command;
    }

    public Path getProjectRoot() {
        return projectRoot;
    }

    public TanukiConfig getConfig() {
        return config;
    }
}
