/* XOR Template Header File */
#ifndef SEC_XOR
#define SEC_XOR

#include <fstream>
#include <stdexcept>
#include <set>
#include <utility>
#include <type_traits>

#include "sec_bin.hpp"

namespace kim
{
    namespace sec
    {
        template<class Container>
        Container XOR(const Container& lhs, const Container& rhs)
        {
            if (lhs.length() != rhs.length()) {
                throw std::invalid_argument("XOR operation of two buffers requires that they are equal in length");
            }

            return Container(lhs.to_Bin() ^ rhs.to_Bin());
        }

        template<class Container>
        Container XOR(const Container& p_Con, const std::byte& p_byte)
        {
            return Container(p_Con.to_Bin() ^ p_byte);
        }

        template<class Container>
        auto XOR_byte_dec(const Container& p_Con)
        {
            auto cmp{ [](std::pair<std::size_t, std::string> lhs, std::pair<std::size_t, std::string> rhs)
            {
                if (lhs.first > rhs.first) {
                    return true;
                } else {
                    return false;
                }
            }};
            std::set<std::pair<std::size_t, std::string>, decltype(cmp)> ret(cmp);

            Binary this_Bin{p_Con.to_Bin()};

            const uint16_t chr_freq[] = { 609, 105, 284, 292, 1136, 179,
                                          138, 341, 544, 24, 41, 292,
                                          276, 544, 600, 195, 24, 495,
                                          568, 803, 243, 97, 138, 24,
                                          130, 3 };

            for (uint8_t i{}; i < UINT8_MAX; i++) {
                std::string ASCII_string{(this_Bin ^ std::byte{i}).to_ASCII()};

                if (ASCII_string.empty()) {
                    continue;
                }

                std::size_t score{};
                for (auto& e : ASCII_string) {
                    if (isalpha(e)) {
                        score += chr_freq[toupper(e) - 'A'];
                    } else {
                        score += 16;
                    }
                }

                ret.insert(std::make_pair(score, ASCII_string));
            }

            return ret;
        }

        auto hex_file_XOR_byte_dec(const std::ifstream& p_File)
        {
            auto cmp{ [](std::pair<std::size_t, std::string> lhs, std::pair<std::size_t, std::string> rhs)
            {
                if (lhs.first > rhs.first) {
                    return true;
                } else {
                    return false;
                }
            }};
            std::set<std::pair<std::size_t, std::string>, decltype(cmp)> ret(cmp);

            std::string line{};

            // while (getline(p_File, line)) {
            //     ret.insert(*(XOR_byte_dec(kim::sec::Hex{line}).begin()));
            // }

            return ret;
        }
    }
}

#endif /* SEC_XOR */
