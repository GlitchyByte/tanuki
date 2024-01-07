# tanuki

![Version](https://img.shields.io/badge/Version-1.1.0-blue)
![Java](https://img.shields.io/badge/Java-21-orange)

Project portfolio watcher and builder.

I made this one for myself mostly. But here is how you can use it.

## Build and copy binary

You'll need [Oracle's GraalVM JDK 21](https://www.oracle.com/java/technologies/downloads/#graalvmjava21). You can use an installer script in [my repo here](https://github.com/GlitchyByte/java-install). It's the easiest way to install it.

    ./build

At the end of the build process it will tell you where `tanuki` binary was generated. Copy it to your project, don't commit it! Add it to your `.gitignore`.

## Usage

The command help is self explanatory:

    tanuki <build|watch> [config_file]

    build       Force builds projects.
    watch       Watches projects for changes and builds them.
    config_file Tanuki config file. If empty, "tanuki.json" in
                    the current directory is used.
    -h, --help  Show usage.

The config file is better explained with an example:

```json
{
  "projects": [
    {
      // A display name for this subproject.
      "name": "Backend",
      // Subproject directory relative to config file.
      "projectDir": "code/backend",
      // An array of directories, relative to projectDir, to watch for changes.
      "watchDirs": [
        "src"
      ],
      // Command or script to run on build, when a change is detected, relative to projectDir.
      "build": "npm run build",
      // Command or script to run on copy, on successful build, relative to config file.
      "copy": "./tanuki.backend.copy"
    },
    // You can have as many subprojects as you want.
    {
      "name": "Frontend",
      "projectDir": "code/frontend",
      "watchDirs": [
        "pages",
        "public",
        "src"
      ],
      "build": "npm run build",
      "copy": "./tanuki.frontend.copy"
    }
  ]
}
```

Just pay attention when a directory is either `relative to config file` or `relative to projectDir`.
And remove the comments, json doesn't allow comments, they are just explanatory in this document.
