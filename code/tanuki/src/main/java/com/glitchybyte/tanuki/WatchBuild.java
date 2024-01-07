// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.concurrent.GTask;
import com.glitchybyte.glib.log.GLog;

import java.io.IOException;
import java.nio.file.*;
import java.util.List;
import java.util.function.Consumer;

public final class WatchBuild extends GTask {

    private final Consumer<TanukiConfig.Project> changeHandler;
    private final TanukiConfig.Project subproject;
    private final List<Path> paths;

    public WatchBuild(final Consumer<TanukiConfig.Project> changeHandler, final TanukiConfig.Project subproject,
            final List<Path> paths) {
        this.changeHandler = changeHandler;
        this.subproject = subproject;
        this.paths = paths;
    }

    @Override
    public void run() {
        // Register watcher.
        try (final WatchService watchService = FileSystems.getDefault().newWatchService()) {
            for (final Path path: paths) {
                path.register(watchService,
                        StandardWatchEventKinds.ENTRY_CREATE,
                        StandardWatchEventKinds.ENTRY_DELETE,
                        StandardWatchEventKinds.ENTRY_MODIFY
                );
            }
            started();
            // Watch loop.
            while (true) {
                final WatchKey key = watchService.take();
                final var events = key.pollEvents();
                if ((events != null) && !events.isEmpty()) {
                    changeHandler.accept(subproject);
                }
                if (!key.reset()) {
                    GLog.severe("Can't reset watch key. Exiting!");
                    return;
                }
            }
        } catch (final IOException e) {
            GLog.severe(e);
        } catch (final InterruptedException e) {
            // We out!
        }
    }
}
