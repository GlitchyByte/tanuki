// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_REPLACEABLE_VARS

#include <map>

namespace gb {

    /**
     * Class to hold variable names and their values for string replacement.
     *
     * <p>Variables in strings are of the form "${myVar}".
     *
     * <p>Such variable would be made known to this class by calling add("myVar", "5").
     */
    class ReplaceableVars {
    private:
        std::map<std::string, std::string> vars;

    public:
        /**
         * Creates a replaceable var container.
         */
        ReplaceableVars() noexcept = default;

        /**
         * Copy constructor.
         *
         * @param other Other object to copy from.
         */
        ReplaceableVars(ReplaceableVars const& other) noexcept;

        /**
         * Move constructor.
         *
         * @param other Other object to move the contents from.
         */
        ReplaceableVars(ReplaceableVars&& other) noexcept;

        /**
         * Copy assignment.
         *
         * @param other Other object to copy from.
         * @return This object with new contents.
         */
        ReplaceableVars& operator=(ReplaceableVars const& other) noexcept;

        /**
         * Move assignment.
         *
         * @param other Other object to move the contents from.
         * @return This object with new contents.
         */
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
