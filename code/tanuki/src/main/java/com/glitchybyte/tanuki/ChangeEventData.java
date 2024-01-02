// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

package com.glitchybyte.tanuki;

public record ChangeEventData(TanukiConfig.Project subproject) {

    public static final String TYPE = "Change";
}
