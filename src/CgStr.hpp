#ifndef SYNTH_LIBCLANG_HPP_INCLUDED
#define SYNTH_LIBCLANG_HPP_INCLUDED

#include "clang-c/CXString.h"
#include <string>
#include <cassert>

// Bla
class CgStr {
public:
    CgStr(CXString&& s)
        : m_data(s)
    { }

    CgStr(CgStr&& other)
        : m_data(std::move(other.m_data))
    {
        other.m_data.data = nullptr;
    }

    CgStr& operator=(CgStr&& other) {
        destroy();
        m_data = std::move(other.m_data);
        other.m_data.data = nullptr; // HACK Undocumented behavior.
        assert(!other.valid());
    }

    ~CgStr() {
        destroy();
    }

    char const* get() const { return clang_getCString(m_data); }

    operator char const* () const { return get(); }

    std::string copy() const
    {
        auto s = get();
        return s ? s : std::string();
    }

    bool valid() const { return m_data.data != nullptr; } // HACK
    bool empty() const
    {
        if (!valid())
            return true;
        auto s = get();
        return !s || !*s;
    }

private:
    void destroy() { if (valid()) clang_disposeString(m_data); }

    CXString m_data;
};

#endif
