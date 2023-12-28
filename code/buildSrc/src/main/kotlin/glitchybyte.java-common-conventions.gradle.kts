// Copyright 2022-2023 GlitchyByte
// SPDX-License-Identifier: MIT-0

plugins {
    id("java")
}

repositories {
    mavenCentral()
    maven {
        // GitHub repository.
        name = "GitHubPackages"
        url = uri("https://maven.pkg.github.com/GlitchyByte/*")
        credentials {
            username = project.findProperty("gpr.username") as String?
            password = project.findProperty("gpr.token") as String?
        }
        metadataSources {
            gradleMetadata()
        }
    }
}

java {
    toolchain {
        languageVersion = JavaLanguageVersion.of(21)
    }
}

tasks.compileJava {
    // Add version file to resources.
    doLast {
        val versionFile = project.file("${projectDir}/src/main/resources/version")
        versionFile.writeText("${project.version}")
    }
}

dependencies {
    // JUnit for unit tests.
    testImplementation("org.junit.jupiter:junit-jupiter:5.10.0")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

tasks.named<Test>("test") {
    // Use JUnit Platform for unit tests.
    useJUnitPlatform()
}

tasks.withType<Test>().configureEach {
    // Maximize parallel forks.
    maxParallelForks = Runtime.getRuntime().availableProcessors().takeIf { it > 0 } ?: 1
}
