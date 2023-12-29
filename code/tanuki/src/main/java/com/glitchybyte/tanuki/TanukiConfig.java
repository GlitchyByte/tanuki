// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import java.util.List;

public final class TanukiConfig {

    public static final class Project {

        public final String name;
        public final String projectDir;
        public final List<String> watchDirs;
        public final String build;
        public final String outputDir;
        public final String copy;

        public Project(final String name, final String projectDir, final List<String> watchDirs, final String build,
                final String outputDir, final String copy) {
            this.name = name;
            this.projectDir = projectDir;
            this.watchDirs = watchDirs;
            this.build = build;
            this.outputDir = outputDir;
            this.copy = copy;
        }
    }

    public final List<Project> projects;

    public TanukiConfig(final List<Project> projects) {
        this.projects = projects;
    }
}
