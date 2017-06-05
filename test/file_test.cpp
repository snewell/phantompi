#include <gtest/gtest.h>

#include <phantompi/file.hpp>

TEST(Io, write)
{
    auto test = phantompi::OutputFile::createFile(
        "test.txt", phantompi::OutputFile::Permissions::owner_read,
        phantompi::OutputFile::Permissions::owner_write);

    std::string data = "Test";
    auto ret = test.write(data.c_str(), data.length());
    ASSERT_EQ(data.length(), ret);
}

TEST(Io, read)
{
    phantompi::InputFile test{"test.txt"};

    char data[5];
    auto ret = test.read(data, 4);
    data[ret] = '\0';
    ASSERT_EQ(4, ret);
    ASSERT_STREQ("Test", data);
}
