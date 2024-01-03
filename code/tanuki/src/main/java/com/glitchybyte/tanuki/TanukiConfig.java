// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.google.gson.TypeAdapter;
import com.google.gson.stream.JsonReader;
import com.google.gson.stream.JsonToken;
import com.google.gson.stream.JsonWriter;

import java.io.IOException;
import java.util.ArrayList;
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

        @Override
        public String toString() {
            return "Project{" +
                    "name='" + name + '\'' +
                    ", projectDir='" + projectDir + '\'' +
                    ", watchDirs=" + watchDirs +
                    ", build='" + build + '\'' +
                    ", outputDir='" + outputDir + '\'' +
                    ", copy='" + copy + '\'' +
                    '}';
        }
    }

    public final List<Project> projects;

    public TanukiConfig(final List<Project> projects) {
        this.projects = projects;
    }

    @Override
    public String toString() {
        return "TanukiConfig{" +
                "projects=" + projects +
                '}';
    }

    public static final class JsonAdapter extends TypeAdapter<TanukiConfig> {

        @Override
        public TanukiConfig read(final JsonReader in) throws IOException {
            if (in.peek() == JsonToken.NULL) {
                in.nextNull();
                return null;
            }
            List<Project> projects = null;
            in.beginObject();
            while (in.hasNext()) {
                final String fieldName = in.nextName();
                switch (fieldName) {
                    case "projects" -> {
                        projects = new ArrayList<>();
                        in.beginArray();
                        while (in.hasNext()) {
                            final Project project = readProject(in);
                            if (project != null) {
                                projects.add(project);
                            }
                        }
                        in.endArray();
                    }
                    default -> in.skipValue();
                }
            }
            in.endObject();
            return new TanukiConfig(projects);
        }

        private Project readProject(final JsonReader in) throws IOException {
            if (in.peek() == JsonToken.NULL) {
                in.nextNull();
                return null;
            }
            String name = null;
            String projectDir = null;
            List<String> watchDirs = null;
            String build = null;
            String outputDir = null;
            String copy = null;
            in.beginObject();
            while (in.hasNext()) {
                final String fieldName = in.nextName();
                switch (fieldName) {
                    case "name" -> name = in.nextString();
                    case "projectDir" -> projectDir = in.nextString();
                    case "watchDirs" -> {
                        watchDirs = new ArrayList<>();
                        in.beginArray();
                        while (in.hasNext()) {
                            final String dir = in.nextString();
                            if (dir != null) {
                                watchDirs.add(dir);
                            }
                        }
                        in.endArray();
                    }
                    case "build" -> build = in.nextString();
                    case "outputDir" -> outputDir = in.nextString();
                    case "copy" -> copy = in.nextString();
                    default -> in.skipValue();
                }
            }
            in.endObject();
            return new Project(name, projectDir, watchDirs, build, outputDir, copy);
        }

        @Override
        public void write(final JsonWriter out, final TanukiConfig value) {
            throw new UnsupportedOperationException();
        }
    }
}
