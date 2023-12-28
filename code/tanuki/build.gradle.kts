// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

plugins {
    id("glitchybyte.java-application-conventions")
//    id("org.graalvm.buildtools.native") version "0.9.28"
}

dependencies {
    implementation("com.glitchybyte.glib:glib:1.9.1-SNAPSHOT")
}

// Setup build info.
group = "com.glitchybyte.tanuki"
version = File("../version").readLines().first().trim()

application {
    mainClass.set("com.glitchybyte.tanuki.App")
}

//graalvmNative {
//    binaries {
//        named("main") {
//            buildArgs.add("-O3")
////            buildArgs.add("--compress")
////            buildArgs.add("--strip")
//        }
//    }
//}
