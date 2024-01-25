// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_STRINGS

#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>

namespace gb::strings {

    /**
     * Converts a C string array into a vector of string_views.
     *
     * <p>The contents of the vector will be valid as long as the original array is valid.
     *
     * <p>This function is specifically designed to convert main function args
     * into a string vector.
     *
     * @param argc String array size.
     * @param argv Array of C strings.
     * @return A string vector of the arguments.
     */
    std::vector<std::string_view> createVectorStringViewFromCArray(int const argc, char const* argv[]) noexcept;

    /**
     * Converts a C string array into a vector of strings.
     *
     * <p>This function is specifically designed to convert main function args
     * into a string vector.
     *
     * @param argc String array size.
     * @param argv Array of C strings.
     * @return A string vector of the arguments.
     */
    std::vector<std::string> createVectorStringFromCArray(int const argc, char const* argv[]) noexcept;

    /**
     * Convenience replace of a token in a string.
     *
     * <p>This creates a new string, so it is only a shortcut, not efficient for replacing many tokens.
     *
     * @param str String to search for token.
     * @param token Token to find.
     * @param value Value to replace the token with.
     * @return The replaced string with the token replaced.
     */
    [[nodiscard]]
    std::string replace(std::string_view const& str, std::string_view const& token, std::string_view const& value) noexcept;

    /**
     * Splits a string by the delimiter into a vector of strings.
     *
     * <p>Storage is weak. Meaning, the resulting vector is actually pointing at sections in the original string.
     * This is faster than regular split, but if the original string goes away, the contents of this
     * vector are not valid.
     *
     * @param str String to split.
     * @param delimiter Delimiter to split the string.
     * @return A vector of strings.
     */
    [[nodiscard]]
    std::vector<std::string_view> splitWeak(std::string_view const& str, std::string_view const& delimiter) noexcept;

    /**
     * Splits a string by the delimiter into a vector of strings.
     *
     * @param str String to split.
     * @param delimiter Delimiter to split the string.
     * @return A vector of strings.
     */
    [[nodiscard]]
    std::vector<std::string> split(std::string_view const& str, std::string_view const& delimiter) noexcept;

    /**
     * Unindents a multiline block of text by removing all common spaces or tabs
     * from the beginning of the lines. Empty lines are left alone.
     *
     * <p>This function is specifically designed to use with raw string literals.
     * And it will remove indentation created to align the string with the rest
     * of the surrounding code.
     *
     * @param str String to unindent.
     * @return The unindented string.
     */
    [[nodiscard]]
    std::string unindent(std::string_view const& str) noexcept;

    /**
     * Joins a string representation of all elements into a string.
     * Each item is separate by a comma and a space.
     *
     * @tparam T The type of item.
     * @param vector Vector of items to join.
     * @return A joined string of all items.
     */
    template <typename T>
    [[nodiscard]]
    std::string fromVector(std::vector<T> const& vector, std::string_view const& separator = ", ") noexcept;

    /**
     * Inserts thousand separator in the given string that must be a numeric representation.
     *
     * <p>Insertion happens in place. The return string is the same as the parameter string.
     *
     * @param str String numeric representation.
     * @return The numeric representation with thousand separators.
     */
    std::string& insertThousandSeparatorsInPlace(std::string& str) noexcept;

    /**
     * Inserts thousand separator in the given string that must be a numeric representation.
     *
     * @param str String numeric representation.
     * @return The numeric representation with thousand separators.
     */
    std::string insertThousandSeparators(std::string_view const& str) noexcept;

    /**
     * Default precision for numeric values.
     */
    extern constinit int DefaultPrecision;

    /**
     * Converts an integral value to a human representation.
     *
     * @tparam T Integer type.
     * @param value Value.
     * @return A human representation of value.
     */
    template <std::integral T>
    std::string fromIntegral(T const value) noexcept;

    /**
     * Converts a floating point value to a human representation.
     *
     * @tparam T Floating point type.
     * @param value Value.
     * @param precision Number of decimals to show. Default shows all needed.
     * @return A human representation of value.
     */
    template <std::floating_point T>
    std::string fromFloatingPoint(T const value, int const precision = DefaultPrecision) noexcept;
}

// ----------------================ Templates ================----------------
namespace gb::strings {

    template<typename T>
    std::string fromVector(std::vector<T> const& vector, std::string_view const& separator) noexcept {
        std::ostringstream ss;
        bool first { true };
        for (const T& item: vector) {
            if (first) {
                first = false;
            } else {
                ss << separator;
            }
            ss << item;
        }
        return ss.str();
    }

    template <std::integral T>
    std::string fromIntegral(T const value) noexcept {
        std::string str { std::to_string(value) };
        return insertThousandSeparatorsInPlace(str);
    }

    template <std::floating_point T>
    std::string fromFloatingPoint(T const value, int const precision) noexcept {
        std::string str;
        if (precision == DefaultPrecision) {
            str = std::to_string(value);
        } else {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(precision) << value;
            str = ss.str();
        }
        return insertThousandSeparators(str);
    }
}

#endif // GB_STRINGS
