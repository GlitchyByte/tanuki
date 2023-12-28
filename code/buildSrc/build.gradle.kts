// Copyright 2022-2023 GlitchyByte
// SPDX-License-Identifier: MIT-0

plugins {
    `kotlin-dsl`
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

repositories {
    gradlePluginPortal()
}
