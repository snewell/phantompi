#include <gtest/gtest.h>

#include <phantompi/sysfs/io.hpp>

TEST(Io, write)
{
    phantompi::sysfs::File test{"test.txt"};

    std::string data = "Test";
    auto ret = test.write(data.c_str(), data.length());
    ASSERT_EQ(data.length(), ret);
}

TEST(Io, read)
{
    phantompi::sysfs::File test{"test.txt"};

    char data[5];
    auto ret = test.read(data, 4);
    data[ret] = '\0';
    ASSERT_EQ(4, ret);
    ASSERT_STREQ("Test", data);
}
