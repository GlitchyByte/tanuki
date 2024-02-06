// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_HASH

#include <functional>

namespace gb::hash {

    class Hashable {
    public:
        virtual size_t hash() const noexcept = 0;

        virtual ~Hashable() = default;
    };
}

namespace std {
    template <typename TClass>
    requires std::is_class<gb::hash::Hashable>::value
    struct hash<TClass> {
        size_t operator()(TClass const& obj) const noexcept {
            return obj.hash();
        }
    };
}

#endif // GB_HASH
