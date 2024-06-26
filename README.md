# tanuki

![Version](https://img.shields.io/badge/Version-2.1.0-blue)

Utility for watching directories and running actions when they change.

The utility can be used in immediate mode and directly watch and run an action, or with a json config file that allows you to have modules with directory groups and an action each.

## Use it!

    tanuki <run|watch> [config_file]
    tanuki -i watch_dir [action]

    run         Runs actions in all modules.
    watch       Watches modules for changes and runs actions on them.
    config_file Tanuki config file. If empty, "tanuki.json" in the current
                  directory is used.
    -i          Immediate mode. In this mode tanuki watches a single directory and
                  runs the action when it is modified. if no action is specified
                  it simply exits.
    watch_dir   Directory to watch.
    action      Action to run when directory is modified.

    Tanuki json configuration file schema is as follows:
      {
        "modules": [
          {
            "name": "My module name",
            "rootDir": "path/to/module",
            "watchDirs": [
              "watched1",
              "subdir/watched2"
            ],
            "action": "echo 'This will run in rootDir.'"
          }
        ]
      }

## Get it!

It is highly recommended that you build it yourself on your platform.

If you want a pre-compiled binary for macOS, Windows, or Ubuntu, check the [latest release](https://github.com/GlitchyByte/tanuki/releases).
These are built using GitHub Actions on GitHub Runners.

## Build it!

Clone this repo. Then:

    ./build MinSizeRel clean

### Receive your tasty binary!

After building, `tanuki` executable is in the `code/bin` directory.
