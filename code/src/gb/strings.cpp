// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/strings.h"
#ifdef GB_STRINGS

namespace gb::strings {

    std::vector<std::string_view> createVectorStringViewFromCArray(int const argc, char const* argv[]) noexcept {
        size_t const argsSize { static_cast<size_t>(argc) };
        std::vector<std::string_view> args { argsSize };
        for (size_t i = 0; i < argsSize; ++i) {
            args[i] = std::string_view { argv[i] };
        }
        return args;
    }

    std::vector<std::string> createVectorStringFromCArray(int const argc, char const* argv[]) noexcept {
        size_t const argsSize { static_cast<size_t>(argc) };
        std::vector<std::string> args { argsSize };
        for (size_t i = 0; i < argsSize; ++i) {
            args[i] = std::string { argv[i] };
        }
        return args;
    }

    std::string replace(std::string_view const& str, std::string_view const& token, std::string_view const& value) noexcept {
        size_t pos = { str.find(token) };
        if (pos == std::string::npos) {
            return "";
        }
        std::string newStr { str };
        return newStr.replace(pos, token.length(), value);
    }

    std::vector<std::string_view> splitWeak(std::string_view const& str, std::string_view const& delimiter) noexcept {
        size_t const delimiterSize { delimiter.size() };
        size_t pos { str.find(delimiter) };
        if ((delimiterSize == 0) || (pos == std::string::npos)) {
            return std::vector<std::string_view> { std::string { str } };
        }
        size_t start { 0 };
        std::vector<std::string_view> lines;
        while (pos != std::string::npos) {
            lines.emplace_back(str.substr(start, pos - start));
            start = pos + delimiterSize;
            pos = str.find(delimiter, start);
        }
        lines.emplace_back(str.substr(start));
        return lines;
    }

    std::vector<std::string> split(std::string_view const& str, std::string_view const& delimiter) noexcept {
        std::vector<std::string_view> const weak { std::move(splitWeak(str, delimiter)) };
        size_t const weakSize { weak.size() };
        std::vector<std::string> lines { weakSize };
        for (size_t i = 0; i < weakSize; ++i) {
            lines[i] = weak[i];
        }
        return lines;
    }

    std::string unindent(std::string_view const& str) noexcept {
        if (str.empty()) {
            return "";
        }
        auto lines { std::move(splitWeak(str, "\n")) };
        if (lines.size() == 1) {
            return std::string { str };
        }
        if (lines[0].empty()) {
            // We allow a 1st new line so that it looks good in code.
            // We remove that extra first line.
            lines.erase(lines.begin());
        }
        // Find indentation.
        size_t indent { std::string::npos };
        bool foundDifference { false };
        while (!foundDifference) {
            char ch;
            for (size_t index = 0; index < lines.size(); ++index) {
                auto const& line { lines[index] };
                if (line.empty()) {
                    continue;
                }
                if (index == 0) {
                    if ((indent != -1) && (indent >= line.size())) {
                        foundDifference = true;
                        break;
                    }
                    if (indent == std::string::npos) {
                        indent = 0;
                    } else {
                        ++indent;
                    }
                    ch = line[index];
                    if ((ch == ' ') || (ch == '\t')) {
                        continue;
                    }
                    foundDifference = true;
                    break;
                }
                foundDifference = (indent >= line.size()) || (line[indent] != ch);
                if (foundDifference) {
                    break;
                }
            }
        }
        // Reassemble while removing indentation.
        if (indent == std::string::npos) {
            indent = 0;
        }
        std::ostringstream ss;
        bool first { true };
        for (auto const& line: lines) {
            if (first) {
                first = false;
            } else {
                ss << '\n';
            }
            if (line.empty()) {
                continue;
            }
            if (indent == 0) {
                ss << line;
            } else {
                ss << line.substr(indent);
            }
        }
        return ss.str();
    }

    std::string addThousandSeparators(std::string const& str) noexcept {
        std::string number { str };
        size_t const period { number.find('.') };
        size_t const start { period == std::string::npos ? number.length() : period };
        size_t const finish { static_cast<size_t>(number.starts_with('-') ? 4 : 3) };
        size_t index { start };
        while (index > finish) {
            index -= 3;
            number.insert(index, ",");
        }
        return number;
    }

    std::string fromTime(std::time_t const& time, std::string const& format) noexcept {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), format.c_str());
        return ss.str();
    }
}

#endif // GB_STRINGS
