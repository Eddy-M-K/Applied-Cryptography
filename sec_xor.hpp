/* XOR Template Header File */
#ifndef SEC_XOR
#define SEC_XOR

#include <fstream>
#include <stdexcept>
#include <set>
#include <type_traits>
#include <tuple>
#include <bitset>
#include <limits>

#include "types_bin.hpp"

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

            Binary      ret_Bin{};
            Binary      lhs_Bin{lhs};
            Binary      rhs_Bin{rhs};

            ret_Bin.reserve(lhs_Bin.length());

            for (std::vector<std::byte>::size_type index{}; index < lhs_Bin.length(); index++) {
                ret_Bin.push_back(lhs_Bin[index] ^ rhs_Bin[index]);
            }

            return Container(ret_Bin);
        }

        template<class Container>
        Container XOR(const Container& p_Con, const std::byte& p_byte)
        {
            Binary      ret_Bin{};
            Binary      p_Con_Bin{p_Con};

            ret_Bin.reserve(p_Con_Bin.length());

            for (std::vector<std::byte>::size_type index{}; index < p_Con_Bin.length(); index++) {
                ret_Bin.push_back(p_Con_Bin[index] ^ p_byte);
            }

            return Container(ret_Bin);
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

            Container       p_str_Con{p_str};
            Binary          this_Bin{p_str_Con.to_Bin()};

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
                Binary          XOR_result{XOR<Binary>(this_Bin, std::byte{i})};
                std::string     ASCII_string{};
                std::size_t     score{};

                for (std::size_t k{}; k < XOR_result.length(); k++) {
                    uint8_t byte_int{std::to_integer<uint8_t>(XOR_result[k])};

                    /* Invalid ASCII */
                    if (!isascii(byte_int)) {
                        ASCII_string = "";
                        break;
                    /* Unprintable ASCII */
                    } else if (byte_int >= 0U && byte_int <= 31U) {
                        ASCII_string += nonprint_ASCII[byte_int];
                    /* DEL character */
                    } else if (byte_int == 127U) {
                        ASCII_string += "(DEL)";
                    /* Space */
                    } else if (byte_int == 32) {
                        ASCII_string += ' ';
                        score += 1217;
                    /* Characters */
                    } else {
                        const char tmp_chr{static_cast<char>(byte_int)};

                        /* Alphabet */
                        if (isalpha(tmp_chr)) {
                            score += chr_freq[toupper(tmp_chr) - 'A'];
                        /* Numbers/Symbols */
                        } else {
                            score += 16;
                        }

                        ASCII_string += tmp_chr;
                    }
                }

                if (!ASCII_string.empty()) {
                    ret.insert(std::make_tuple(score, p_str_Con, Binary{std::byte{i}}, ASCII_string));
                }

            } while (i++ != UINT8_MAX);

            return ret;
        }

        template <class Container>
        auto XOR_byte_dec(std::ifstream p_File)
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
                auto list{XOR_byte_dec<Container>(line)};
                if (!list.empty()) {
                    auto best_candidate{*list.begin()};
                    ret.insert(best_candidate);
                }
            }

            return ret;
        }

        /* Encrypting a string using repeating key XOR (key is a valid ASCII string) */
        template <class Container>
        Container XOR_rep_key_enc(const std::string& p_pt, const std::string& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            for (const char& e : p_key) {
                if (!isascii(e)) {
                    throw std::invalid_argument("Key contains invalid ASCII");
                }
            }

            Binary      ret_Bin{};
            Binary      p_pt_Bin{p_pt};
            Binary      p_key_Bin{p_key};

            ret_Bin.reserve(p_pt_Bin.length());

            for (std::size_t pt_index{}, key_index{}; pt_index < p_pt_Bin.length(); pt_index++, key_index++) {
                if (key_index == p_key_Bin.length()) {
                    key_index = 0;
                }

                ret_Bin.push_back(p_pt_Bin[pt_index] ^ p_key_Bin[key_index]);
            }

            return Container(ret_Bin);
        }

        /* Encrypting a text file with repeating key XOR (key is a valid ASCII string) */
        template <class Container>
        Container XOR_rep_key_enc(std::ifstream p_in_File, const std::string& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            for (const char& e : p_key) {
                if (!isascii(e)) {
                    throw std::invalid_argument("Key contains invalid ASCII");
                }
            }

            Binary      ret_Bin{};
            Binary      p_key_Bin{p_key};

            if (p_in_File.is_open()) {
                char curr_chr{};
                for (std::size_t key_index{}; p_in_File.get(curr_chr); key_index++) {
                    if (key_index == p_key.length()) {
                        key_index = 0;
                    }

                    if (!isascii(curr_chr)) {
                        throw std::invalid_argument("Input file contains invalid ASCII");
                    }

                    ret_Bin.push_back(static_cast<std::byte>(curr_chr) ^ p_key_Bin[key_index]);
                }
            } else {
                throw std::invalid_argument("Unable to open input file");
            }

            ret_Bin.pop_back();

            return Container(ret_Bin);
        }

        /* Calculates the Hamming/edit distance between two Security Types (could be Binary strings or std::byte) */
        template <class Container1, class Container2>
        std::size_t Hamming(const Container1& lhs, const Container2& rhs)
        {
            std::size_t     ret{};
            Binary          lhs_Bin{lhs};
            Binary          rhs_Bin{rhs};

            if (lhs_Bin.length() != rhs_Bin.length()) {
                throw std::invalid_argument("Inputs must be equal in length to compute the Hamming distance");
            }

            for (std::vector<std::byte>::size_type index{}; index < lhs.length(); index++) {
                ret += std::bitset<8>(static_cast<uint8_t>(lhs_Bin[index] ^ rhs_Bin[index])).count();
            }

            return ret;
        }

        template <class Container>
        std::fstream XOR_rep_key_dec(std::ifstream p_in_File)
        {
            std::fstream        ret("out.txt");
            std::string         full_ct{};

            if (p_in_File.is_open()) {
                std::string curr_line{};
                while (getline(p_in_File, curr_line)) {
                    full_ct += curr_line;
                }
            } else {
                throw std::invalid_argument("Unable to open input file");
            }

            Container       full_ct_Con{full_ct};
            Binary          full_ct_Bin{full_ct_Con};
            std::size_t     min_hamming_norm{std::numeric_limits<std::size_t>::max()};
            uint8_t         keysize{2};

            for (uint8_t keysize_guess{2}; keysize_guess <= 40U; keysize_guess++) {
                // printf("%d\n", keysize_guess);
                if (keysize_guess > full_ct_Bin.length()) {
                    break;
                }

                std::size_t curr_hamming_norm{Hamming<Binary, Binary>(full_ct_Bin.subBin(0, keysize_guess),
                                                                      full_ct_Bin.subBin(keysize_guess, keysize_guess))};


                std::size_t count{1};
                for (; (count + 1) * keysize - 1 <= full_ct_Bin.length(); count++) {
                    curr_hamming_norm += Hamming<Binary, Binary>(full_ct_Bin.subBin(0, keysize_guess),
                                                                 full_ct_Bin.subBin((count + 1) * keysize_guess, keysize_guess));
                }

                curr_hamming_norm /= (keysize_guess * count);

                if (curr_hamming_norm < min_hamming_norm) {
                    // printf("%d %ld\n", keysize_guess, curr_hamming_norm);
                    min_hamming_norm = curr_hamming_norm;
                    keysize = keysize_guess;
                    printf("%ld, %d\n", curr_hamming_norm, keysize);
                }
            }

            // std::vector<Binary> blocks{keysize};

            // for (std::vector<std::byte>::size_type ct_index{}; ct_index < full_ct_Bin.length(); ct_index += blocks) {
            //     for (std::size_t block_index{}; block_index < blocks; block_index++) {
            //         blocks[block_index].push_back(full_ct_Bin[ct_index + block_index]);
            //     }
            // }

            // XOR_byte_dec()

            return std::fstream{};
        }
    }
}

#endif /* SEC_XOR */
