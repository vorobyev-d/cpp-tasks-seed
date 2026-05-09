#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "base85ed.h"

namespace
{

std::vector<uint8_t> cstr2v(const char *s)
{
    return std::vector<uint8_t>(
               s,
               s + std::string(s).size()
           );
}

std::vector<uint8_t> bytes(std::initializer_list<uint8_t> values)
{
    return std::vector<uint8_t>(values);
}

std::vector<uint8_t> range_bytes(std::size_t size)
{
    std::vector<uint8_t> result;
    result.reserve(size);

    for (std::size_t i = 0; i < size; ++i)
    {
        result.push_back(static_cast<uint8_t>(i & 0xFF));
    }

    return result;
}

std::vector<uint8_t> noisy_bytes(std::size_t size)
{
    std::vector<uint8_t> result;
    result.reserve(size);

    for (std::size_t i = 0; i < size; ++i)
    {
        result.push_back(static_cast<uint8_t>((i * 37 + 11) & 0xFF));
    }

    return result;
}

std::size_t encoded_size(std::size_t raw_size)
{
    const std::size_t full_blocks = raw_size / 4;
    const std::size_t tail = raw_size % 4;

    return full_blocks * 5 + (tail == 0 ? 0 : tail + 1);
}

const std::vector<std::pair<std::vector<uint8_t>, const char * >> cases =
{
    { cstr2v(""), "" },
    { cstr2v("1"), "F#" },
    { cstr2v("12"), "F){" },
    { cstr2v("123"), "F)}j" },
    { cstr2v("1234"), "F)}kW" },
    { cstr2v("hello"), "Xk~0{Zv" },
    { cstr2v("hello world"), "Xk~0{Zy<MXa%^M" },
    { cstr2v("some text some letters ww"), "b8l^BAarGTbRctYZDk;AWps39a&sVecK" },
    { bytes({0x00, 0x00, 0x00, 0x00}), "00000" },
    { bytes({0xFF, 0xFF, 0xFF, 0xFF}), "|NsC0" },
    { bytes({0x00, 0x01, 0x02, 0x03}), "009C6" },
    { bytes({0x04, 0x05, 0x06, 0x07}), "1O)~M" },
    { bytes({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}), "009C61O)~M" },
    { bytes({0x00, 0x0A, 0x0D, 0x1A, 0x7F, 0x80, 0xFE, 0xFF}), "016Ene}Mk~" },
    { range_bytes(16), "009C61O)~M2nh-c3=Iws" },
    {
        bytes({0x0B, 0x30, 0x55, 0x7A, 0x9F, 0xC4, 0xE9, 0x0E, 0x33, 0x58, 0x7D, 0xA2, 0xC7, 0xEC, 0x11,
               0x36, 0x5B, 0x80, 0xA5, 0xCA, 0xEF, 0x14, 0x39, 0x5E, 0x83, 0xA8, 0xCD, 0xF2, 0x17, 0x3C, 0x61}),
        "3ouoBpTy}7Ggy71$LtX{TY#m?""?-V&+gQ(5&7d&A"
    }
};

}

TEST(Base85Cases, EncodesPythonBase85Vectors)
{
    for (const auto &p : cases)
    {
        EXPECT_EQ(base85::encode(p.first), cstr2v(p.second));
    }
}

TEST(Base85Cases, DecodesPythonBase85Vectors)
{
    for (const auto &p : cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(p.second)), p.first);
    }
}

TEST(Base85Encode, ProducesExpectedSizeForEveryTailLength)
{
    for (std::size_t size = 0; size <= 80; ++size)
    {
        EXPECT_EQ(base85::encode(noisy_bytes(size)).size(), encoded_size(size));
    }
}

TEST(Base85RoundTrip, HandlesEveryTailLengthAroundManyBlockBorders)
{
    for (std::size_t size = 0; size <= 257; ++size)
    {
        const std::vector<uint8_t> raw = noisy_bytes(size);
        const std::vector<uint8_t> encoded = base85::encode(raw);

        EXPECT_EQ(base85::decode(encoded), raw);
    }
}

TEST(Base85RoundTrip, HandlesAllPossibleSingleByteValues)
{
    for (int byte = 0; byte <= 0xFF; ++byte)
    {
        const std::vector<uint8_t> raw = bytes({static_cast<uint8_t>(byte)});
        const std::vector<uint8_t> encoded = base85::encode(raw);

        EXPECT_EQ(encoded.size(), 2U);
        EXPECT_EQ(base85::decode(encoded), raw);
    }
}

TEST(Base85RoundTrip, HandlesAllPossibleByteValuesRepeatedSeveralTimes)
{
    const std::vector<uint8_t> raw = range_bytes(1024);
    const std::vector<uint8_t> encoded = base85::encode(raw);

    EXPECT_EQ(encoded.size(), encoded_size(raw.size()));
    EXPECT_EQ(base85::decode(encoded), raw);
}

TEST(Base85RoundTrip, HandlesZeroFilledAndMaxFilledBlocks)
{
    const std::vector<std::vector<uint8_t>> inputs =
    {
        std::vector<uint8_t>(1, 0x00),
        std::vector<uint8_t>(2, 0x00),
        std::vector<uint8_t>(3, 0x00),
        std::vector<uint8_t>(4, 0x00),
        std::vector<uint8_t>(17, 0x00),
        std::vector<uint8_t>(1, 0xFF),
        std::vector<uint8_t>(2, 0xFF),
        std::vector<uint8_t>(3, 0xFF),
        std::vector<uint8_t>(4, 0xFF),
        std::vector<uint8_t>(17, 0xFF)
    };

    for (const auto &raw : inputs)
    {
        const std::vector<uint8_t> encoded = base85::encode(raw);

        EXPECT_EQ(encoded.size(), encoded_size(raw.size()));
        EXPECT_EQ(base85::decode(encoded), raw);
    }
}

TEST(Base85Encode, UsesOnlyTheDeclaredPythonBase85Alphabet)
{
    const std::string alphabet =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!#$%&()*+-;<=>?@^_`{|}~";

    const std::vector<uint8_t> encoded = base85::encode(noisy_bytes(999));

    for (uint8_t ch : encoded)
    {
        EXPECT_NE(alphabet.find(static_cast<char>(ch)), std::string::npos);
    }
}
