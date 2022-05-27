/* XOR Template Header File */
#ifndef SEC_XOR
#define SEC_XOR

#include <fstream>
#include <stdexcept>
#include <set>
#include <utility>
#include <type_traits>
#include <tuple>

#include "sec_hex.hpp"
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
        auto XOR_byte_dec(const std::string& p_str)
        {
            /* Custom comparator */
            auto cmp{ [](std::tuple<const std::size_t, const Container, const Binary, const std::string> lhs,
                         std::tuple<const std::size_t, const Container, const Binary, const std::string> rhs)
            {
                return (std::get<0>(lhs) > std::get<0>(rhs));
            }};
            /* Set with each entry comprising of { Score | Ciphertext | Byte | Plaintext } */
            std::set<std::tuple<const std::size_t, const Container, const Binary, const std::string>, decltype(cmp)> ret(cmp);

            Container p_Con{p_str};
            Binary this_Bin{p_Con.to_Bin()};

            const uint16_t    chr_freq[] = { 609, 105, 284, 292, 1136, 179,
                                             138, 341, 544,  24,   41, 292,
                                             276, 544, 600, 195,   24, 495,
                                             568, 803, 243,  97,  138,  24,
                                             130,   3 };
            const char *nonprint_ASCII[] = { "(NUL)", "(SOH)", "(STX)", "(ETX)", "(EOT)",
                                             "(ENQ)", "(ACK)", "(BEL)",  "(BS)",  "(HT)",
                                              "(LF)",  "(VT)",  "(FF)",  "(CR)",  "(SO)",
                                              "(SI)", "(DLE)", "(DC1)", "(DC2)", "(DC3)",
                                             "(DC4)", "(NAK)", "(SYN)", "(ETB)", "(CAN)",
                                              "(EM)", "(SUB)", "(ESC)",  "(FS)",  "(GS)",
                                              "(RS)",  "(US)" };

            uint8_t i{};
            do {
                Binary XOR_result{this_Bin ^ std::byte{i}};
                std::string ASCII_string{};

                std::size_t score{};
                for (std::size_t k{}; k < XOR_result.length(); k++) {
                    uint8_t byte_int{std::to_integer<uint8_t>(XOR_result[k])};

                    if (byte_int > 127U) {
                        ASCII_string = "";
                        break;
                    } else if (byte_int >= 0U && byte_int <= 31U) {
                        ASCII_string += nonprint_ASCII[byte_int];
                    } else if (byte_int == 127U) {
                        ASCII_string += "(DEL)";
                    } else if (byte_int == 32) {
                        ASCII_string += ' ';
                        score += 1217;
                    } else {
                        const char tmp_chr{static_cast<char>(byte_int)};

                        if (isalpha(tmp_chr)) {
                            score += chr_freq[toupper(tmp_chr) - 'A'];
                        } else {
                            score += 16;
                        }

                        ASCII_string += tmp_chr;
                    }
                }

                if (!ASCII_string.empty()) {
                    ret.insert(std::make_tuple(score, p_Con, Binary{std::byte{i}}, ASCII_string));
                }

            } while (i++ != UINT8_MAX);

            return ret;
        }

        template <class Container>
        auto XOR_byte_dec(std::ifstream& p_File)
        {
            /* Custom comparator */
            auto cmp{ [](std::tuple<const std::size_t, const Container, const Binary, const std::string> lhs,
                         std::tuple<const std::size_t, const Container, const Binary, const std::string> rhs)
            {
                return (std::get<0>(lhs) > std::get<0>(rhs));
            }};
            /* Set comprising of the best candidates in the format { Score | Ciphertext | Byte | Plaintext } */
            std::set<std::tuple<const std::size_t, const Container, const Binary, const std::string>, decltype(cmp)> ret(cmp);

            std::string line{};
            while (getline(p_File, line)) {
                auto tmp{XOR_byte_dec<Container>(line)};
                if (!tmp.empty()) {
                    ret.insert(*tmp.begin());
                }
            }

            return ret;
        }

        template <class Container>
        void XOR_rep_key_enc(std::ifstream& p_File)
        {

        }
    }
}

#endif /* SEC_XOR */
