// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_FUNCTIONS

namespace gb::functions {

    /**
     * Template function to wrap a method in a function whose 1st argument is a context void pointer.
     * <p>
     * The function will cast that 1st void pointer to an instance of the class and call the method
     * with the rest of the arguments.
     *
     * @tparam TClass Class defining the method.
     * @tparam TReturn Return type of the function.
     * @tparam TArgs Function arguments, not counting the 1st void pointer.
     * @param method Class method to call.
     * @return A function that returns the given type, and takes a void pointer and other defined parameters.
     */
    template<typename TClass, typename TReturn, typename ...TArgs>
    std::function<TReturn(void*, TArgs...)> methodWrapper(TReturn(TClass::*method)(TArgs... args)) {
        return [method](void* context, TArgs... args) -> TReturn {
            auto instance { static_cast<TClass*>(context) };
            return (instance->*method)(args...);
        };
    }
}

#endif // GB_FUNCTIONS
