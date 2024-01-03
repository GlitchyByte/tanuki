// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.GPaths;
import com.glitchybyte.glib.GShutdownMonitor;
import com.glitchybyte.glib.GStrings;
import com.glitchybyte.glib.concurrent.GTaskRunnerService;
import com.glitchybyte.glib.concurrent.event.GEvent;
import com.glitchybyte.glib.concurrent.event.GEventLink;
import com.glitchybyte.glib.concurrent.event.GEventReceiver;
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
    private final GEventLink eventLink = new GEventLink();
    private String watchingSummary;

    public WatchCommand(final Path projectRoot, final TanukiConfig config) {
        super(projectRoot, config);
    }

    @Override
    public void run() {
        collectDirectoriesToWatch();
        try (final GTaskRunnerService runner = new GTaskRunnerService();
            final GEventReceiver eventReceiver = eventLink.createEventReceiver(this::changeHandler)
                    .subscribeTo(CHANGE_EVENT_TYPE)
        ) {
            for (final var entry: subprojects.entrySet()) {
                eventLink.send(CHANGE_EVENT_TYPE, entry.getKey());
                runner.start(new WatchBuild(entry.getKey(), entry.getValue(), eventLink));
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

    private void changeHandler(final GEvent event) {
        final TanukiConfig.Project subproject = event.getDataAs(TanukiConfig.Project.class);
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
        GTerminal.println("-".repeat(64));
        GTerminal.println(watchingSummary);
    }
}
