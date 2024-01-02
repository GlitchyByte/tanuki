// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

import com.glitchybyte.glib.concurrent.GTask;
import com.glitchybyte.glib.concurrent.event.GEventSender;
import com.glitchybyte.glib.log.GLog;

import java.io.IOException;
import java.nio.file.*;
import java.util.List;

public final class WatchBuild extends GTask {

    private final TanukiConfig.Project subproject;
    private final List<Path> paths;
    private final GEventSender eventSender;

    public WatchBuild(final TanukiConfig.Project subproject, final List<Path> paths, final GEventSender eventSender) {
        this.subproject = subproject;
        this.paths = paths;
        this.eventSender = eventSender;
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
                key.pollEvents();
                if (!key.reset()) {
                    GLog.severe("Can't reset watch key. Exiting!");
                    return;
                }
                eventSender.send(ChangeEventData.TYPE, new ChangeEventData(subproject));
            }
        } catch (final IOException e) {
            GLog.severe(e);
        } catch (final InterruptedException e) {
            // We out!
        }
    }
}
