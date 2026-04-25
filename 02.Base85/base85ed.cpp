#include "base85ed.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>


namespace {

constexpr char ALPHABET[] = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "!#$%&()*+-;<=>?@^_`{|}~";

std::array<int, 256> make_decode_table()
{
    std::array<int, 256> table{};
    table.fill(-1);

    for (int i = 0; i < 85; ++i) {
        table[static_cast<unsigned char>(ALPHABET[i])] = i;
    }

    return table;
}

} 

std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    std::vector<uint8_t> result;
    for (std::size_t i = 0; i < bytes.size(); i += 4) {
        const std::size_t block_size = std::min<std::size_t>(4, bytes.size() - i);

        uint32_t value = 0;

        for (std::size_t j = 0; j < 4; ++j) {
            value <<= 8;

            if (j < block_size) {
                value |= bytes[i + j];
            }
        }

        uint8_t encoded[5];

        for (int j = 4; j >= 0; --j) {
            encoded[j] = static_cast<uint8_t>(ALPHABET[value % 85]);
            value /= 85;
        }

        const std::size_t chars_to_write = block_size + 1;

        for (std::size_t j = 0; j < chars_to_write; ++j) {
            result.push_back(encoded[j]);
        }
    }

    return result;
}

std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    static const std::array<int, 256> decode_table = make_decode_table();

    std::vector<uint8_t> result;

    for (std::size_t i = 0; i < b85str.size(); i += 5) {
        const std::size_t block_size = std::min<std::size_t>(5, b85str.size() - i);

        uint64_t value = 0;

        for (std::size_t j = 0; j < 5; ++j) {
            uint8_t ch = 0;

            if (j < block_size) {
                ch = b85str[i + j];
            } else {
                ch = static_cast<uint8_t>('~');
            }

            const int digit = decode_table[ch];

            value = value * 85 + static_cast<uint64_t>(digit);
        }

        const uint32_t decoded_value = static_cast<uint32_t>(value);

        uint8_t decoded[4];

        decoded[0] = static_cast<uint8_t>((decoded_value >> 24) & 0xFF);
        decoded[1] = static_cast<uint8_t>((decoded_value >> 16) & 0xFF);
        decoded[2] = static_cast<uint8_t>((decoded_value >> 8) & 0xFF);
        decoded[3] = static_cast<uint8_t>(decoded_value & 0xFF);

        const std::size_t bytes_to_write = block_size - 1;

        for (std::size_t j = 0; j < bytes_to_write; ++j) {
            result.push_back(decoded[j]);
        }
    }

    return result;
}

