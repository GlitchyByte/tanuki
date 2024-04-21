// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#ifdef __APPLE__
    #define GB_IS_MACOS
#endif
#ifdef __linux__
    #define GB_IS_LINUX
#endif
#ifdef _WIN32
    #define GB_IS_WINDOWS
#endif

#define GB_RESHELPER
//#define GB_HASH
#define GB_STRINGS
    #define GB_TERMINAL
#define GB_FUNCTIONS
#define GB_FILES
#define GB_PROCESS
#define GB_REPLACEABLE_VARS
#define GB_SHUTDOWN_MONITOR
#define GB_CONCURRENT
    #define GB_TASK
#define GB_DIRECTORY_WATCHER

#include "gb/constants.h"
#ifdef GB_RESHELPER
    #include "gb/reshelper.h"
#endif
#ifdef GB_HASH
    #include "gb/hash.h"
#endif
#ifdef GB_STRINGS
    #include "gb/strings.h"
    #ifdef GB_TERMINAL
        #include "gb/terminal.h"
    #endif
#endif
#ifdef GB_FUNCTIONS
    #include "gb/functions.h"
#endif
#ifdef GB_FILES
    #include "gb/files.h"
#endif
#ifdef GB_PROCESS
    #include "gb/process.h"
#endif
#ifdef GB_REPLACEABLE_VARS
    #include "gb/ReplaceableVars.h"
#endif
#ifdef GB_SHUTDOWN_MONITOR
    #include "gb/ShutdownMonitor.h"
#endif
#ifdef GB_CONCURRENT
    #ifdef GB_TASK
        #include "gb/concurrent/Task.h"
        #include "gb/concurrent/TaskRunner.h"
    #endif
#endif
#ifdef GB_DIRECTORY_WATCHER
    #include "gb/DirectoryWatcher.h"
#endif
