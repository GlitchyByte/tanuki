// Copyright 2022-2023 GlitchyByte
// SPDX-License-Identifier: MIT-0

plugins {
    id("glitchybyte.java-library-conventions")
    id("maven-publish")
    id("signing")
}

java {
    // Create javadoc and sources publishing artifacts.
    withJavadocJar()
    withSourcesJar()
}
