// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.GShutdownMonitor;
import com.glitchybyte.glib.GStrings;
import com.glitchybyte.glib.concurrent.GTaskRunnerService;
import com.glitchybyte.glib.process.GOSInterface;
import com.glitchybyte.glib.terminal.GTerminal;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class WatchCommand extends Command {

    private final Map<TanukiConfig.Project, List<Path>> subprojects = new HashMap<>();
    private String watchingSummary;

    public WatchCommand(final Path projectRoot, final TanukiConfig config) {
        super(projectRoot, config);
    }

    @Override
    public void run() {
        collectDirectoriesToWatch();
        try (final GTaskRunnerService runner = new GTaskRunnerService()) {
            for (final var entry: subprojects.entrySet()) {
                final TanukiConfig.Project subproject = entry.getKey();
                final List<Path> paths = entry.getValue();
                changeHandler(subproject);
                runner.start(new WatchBuild(this::changeHandler, subproject, paths));
            }
            GShutdownMonitor.createShutdownMonitor().awaitShutdown();
        } catch (final InterruptedException e) {
            // We out!
        }
        GTerminal.println("%n%s", GTerminal.text("Done!", Colors.heading));
    }

    private void collectDirectoriesToWatch() {
        // Collect directories to watch.
        final StringBuilder sb = new StringBuilder();
        config.projects.forEach(subproject -> {
            sb.append(GStrings.format("%s:%n", GTerminal.text("Watching", Colors.heading)));
            final Path subprojectDir = GPaths.getFullPath(projectRoot.resolve(subproject.projectDir));
            if (!Files.isDirectory(subprojectDir)) {
                panic("Subproject directory not found: " + subprojectDir);
            }
            sb.append(GStrings.format("%s %s (%s)%n",
                    GTerminal.text("§", Colors.heading),
                    GTerminal.text(subproject.name, Colors.highlight),
                    GTerminal.text(subprojectDir.toString(), Colors.text)
            ));
            subproject.watchDirs.forEach(dir -> {
                final Path watchDir = GPaths.getFullPath(subprojectDir.resolve(dir));
                if (!Files.isDirectory(watchDir)) {
                    panic("Subproject directory not found: " + watchDir);
                }
                subprojects.computeIfAbsent(subproject, p -> new ArrayList<>())
                        .add(watchDir);
                sb.append(GStrings.format("  %s %s%n",
                        GTerminal.text("»", Colors.highlight),
                        GTerminal.text(dir, Colors.text)
                ));
            });
        });
        sb.append(GTerminal.text("%nCtrl + C to exit", Colors.highlight));
        watchingSummary = sb.toString();
    }

    private void changeHandler(final TanukiConfig.Project subproject) {
        // Build.
        final Integer buildExitCode = build(subproject);
        if (GOSInterface.instance.isSuccessfulExitCode(buildExitCode)) {
            // Copy.
            final Integer copyExitCode = copy(subproject);
            if (!GOSInterface.instance.isSuccessfulExitCode(copyExitCode)) {
                GTerminal.println(GTerminal.text(GStrings.format("Copy error! (code: %d)", copyExitCode), Colors.error));
            }
        } else {
            GTerminal.println(GTerminal.text(GStrings.format("Build error! (code: %d)", buildExitCode), Colors.error));
        }
        // Watch summary.
        GTerminal.println("%s< %s >%s", "-".repeat(60), GTerminal.text(getTime(), Colors.highlight), "----");
        GTerminal.println(watchingSummary);
    }

}
