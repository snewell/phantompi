#include <gtest/gtest.h>

#include <phantompi/file.hpp>

TEST(Io, write)
{
    phantompi::File test{"test.txt", phantompi::File::Mode::output};

    std::string data = "Test";
    auto ret = test.write(data.c_str(), data.length());
    ASSERT_EQ(data.length(), ret);
}

TEST(Io, read)
{
    phantompi::File test{"test.txt", phantompi::File::Mode::input};

    char data[5];
    auto ret = test.read(data, 4);
    data[ret] = '\0';
    ASSERT_EQ(4, ret);
    ASSERT_STREQ("Test", data);
}
