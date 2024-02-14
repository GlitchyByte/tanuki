// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_REPLACEABLE_VARS

#include <map>

namespace gb {

    /**
     * Class to hold variable names and their values for string replacement.
     */
    class ReplaceableVars {
    private:
        std::map<std::string, std::string> vars;

    public:
        ReplaceableVars() noexcept = default;

        ReplaceableVars(ReplaceableVars const& other) noexcept;

        ReplaceableVars(ReplaceableVars&& other) noexcept;

        ReplaceableVars& operator=(ReplaceableVars const& other) noexcept;

        ReplaceableVars& operator=(ReplaceableVars&& other) noexcept;

        /**
         * Adds a string value.
         *
         * @param name Variable name.
         * @param value Value.
         * @return This object.
         */
        ReplaceableVars& add(std::string const& name, std::string const& value) noexcept;

        /**
         * Replace inline variables with values.
         *
         * @param str String with variables.
         * @return The replaced string.
         */
        [[nodiscard]]
        std::string replace(std::string_view const& str) const noexcept;

        /**
         * String representation.
         *
         * @return The string representation.
         */
        [[nodiscard]]
        std::string string() const noexcept;
    };
}

#endif // GB_REPLACEABLE_VARS
