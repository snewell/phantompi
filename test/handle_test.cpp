#include <gtest/gtest.h>

#include <phantompi/handle.hpp>

TEST(Handle, makeHandle)
{
    auto handle = phantompi::makeHandle(10, []() {});
}

TEST(Handle, cleanupFn)
{
    auto closed = false;
    {
        auto handle = phantompi::makeHandle(10, [&closed]() { closed = true; });
    }
    ASSERT_TRUE(closed);
}
