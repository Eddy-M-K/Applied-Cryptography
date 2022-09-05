/*
 * @brief XOR Template Header File
 * @author Edward Kim
 */
#ifndef SEC_XOR
#define SEC_XOR

#include <fstream>
#include <stdexcept>
#include <set>
#include <type_traits>
#include <tuple>
#include <bitset>
#include <limits>
#include <memory>
#include <queue>

#include "types_bin.hpp"

namespace kim
{
    namespace sec
    {
        /*
         * @brief Performs the XOR operation of two kim::sec security types
         *
         * @param Container1 Template parameter for lhs parameter (kim::sec security type)
         * @param Container2 Template parameter for rhs parameter (kim::sec security type) - defaults to std::byte if no template argument
         *
         * @param lhs Left-hand side of XOR operation (kim::sec security type)
         * @param rhs Right-hand side of XOR operation (kim::sec security type)
         *
         * @return The kim::sec::Binary object result of the XOR operation
         */
        template<class Container1, class Container2 = std::byte>
        Binary XOR(const Container1& lhs, const Container2& rhs)
        {
            Binary      ret{};
            Binary      lhs_Bin{lhs};
            Binary      rhs_Bin{rhs};

            if (rhs_Bin.length() == 1) {
                ret.reserve(lhs_Bin.length());

                for (std::vector<std::byte>::size_type index{}; index < lhs_Bin.length(); index++) {
                    ret.push_back(lhs_Bin[index] ^ rhs_Bin[0]);
                }

                return ret;
            }

            if (lhs_Bin.length() != rhs_Bin.length()) {
                throw std::invalid_argument("XOR operation of two buffers requires that they are equal in length");
            }

            ret.reserve(lhs_Bin.length());

            for (std::vector<std::byte>::size_type index{}; index < lhs_Bin.length(); index++) {
                ret.push_back(lhs_Bin[index] ^ rhs_Bin[index]);
            }

            return ret;
        }

        /*
         * @brief Decrypts a XOR byte encrypted ciphertext string
         *
         * @param Container Template parameter for input ciphertext (kim::sec security type)
         *
         * @param p_Con Ciphertext (kim::sec security type)
         *
         * @return A tuple consisting of { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string }
         */
        template<class Container>
        std::tuple<std::size_t, Container, Binary, std::string> XOR_byte_dec(const Container& p_Con)
        {
            using score_entry = std::tuple<std::size_t, Container, Binary, std::string>;

            /* Custom comparator */
            auto cmp{
                        [](const score_entry& lhs, const score_entry& rhs)
                        {
                            return (std::get<0>(lhs) < std::get<0>(rhs));
                        }
                    };

            /* Priority Queue of score entries */
            std::priority_queue<score_entry, std::vector<score_entry>, decltype(cmp)> ret_pqueue(cmp);

            Binary              p_Con_Bin{p_Con};
            const uint16_t      chr_freq[] = { 609, 105, 284, 292, 1136, 179,
                                               138, 341, 544,  24,   41, 292,
                                               276, 544, 600, 195,   24, 495,
                                               568, 803, 243,  97,  138,  24,
                                               130,   3 };
            const char*         nonprint_ASCII[] = { "(NUL)", "(SOH)", "(STX)", "(ETX)", "(EOT)",
                                                     "(ENQ)", "(ACK)", "(BEL)",  "(BS)",  "(HT)",
                                                      "(LF)",  "(VT)",  "(FF)",  "(CR)",  "(SO)",
                                                      "(SI)", "(DLE)", "(DC1)", "(DC2)", "(DC3)",
                                                     "(DC4)", "(NAK)", "(SYN)", "(ETB)", "(CAN)",
                                                      "(EM)", "(SUB)", "(ESC)",  "(FS)",  "(GS)",
                                                      "(RS)",  "(US)" };

            {
                uint8_t i{};
                do {
                    Binary          XOR_result{XOR<Binary>(p_Con_Bin, std::byte{i})};
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
                        ret_pqueue.push(std::make_tuple(score, p_Con, Binary{std::byte{i}}, ASCII_string));
                    }

                } while (i++ != UINT8_MAX);
            }

            if (!ret_pqueue.empty()) {
                return ret_pqueue.top();
            } else {
                return score_entry{};
            }
        }

        /*
         * @brief Decrypts a file with XOR byte encrypted ciphertext
         *
         * @param Container Template parameter for input ciphertext (kim::sec security type)
         *
         * @param p_File Input file with the ciphertext (std::ifstream)
         *
         * @return A set with tuples in the form of { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string }
         */
        template <class Container>
        auto XOR_byte_dec(std::ifstream p_File)
        {
            using score_entry = std::tuple<std::size_t, Container, Binary, std::string>;

            /* Custom comparator */
            auto cmp{
                        [](const score_entry& lhs, const score_entry& rhs)
                        {
                            return (std::get<0>(lhs) > std::get<0>(rhs));
                        }
                    };

            /* Set comprising of the best candidates in the format { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string } */
            std::set<score_entry, decltype(cmp)> ret(cmp);

            for (std::string line{}; getline(p_File, line);) {
                score_entry best_candidate{XOR_byte_dec<Container>(Container{line})};
                if (std::get<0>(best_candidate)) {
                    ret.insert(best_candidate);
                }
            }

            return ret;
        }

        /*
         * @brief Encrypts a string using repeating key XOR
         *
         * @param Container Template parameter for the type of the ciphertext return (kim::sec security type)
         *
         * @param p_pt Plaintext to encrypt (std::string)
         * @param p_key Key to use for encryption (kim::sec::Binary)
         *
         * @return Ciphertext in the specified kim::sec security type
         */
        template <class Container>
        Container XOR_rep_key_enc(const std::string& p_pt, const Binary& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            Binary      ret{};
            Binary      p_pt_Bin{p_pt};

            ret.reserve(p_pt_Bin.length());

            for (std::size_t pt_index{}, key_index{}; pt_index < p_pt_Bin.length(); pt_index++, key_index++) {
                if (key_index == p_key.length()) {
                    key_index = 0;
                }

                ret.push_back(p_pt_Bin[pt_index] ^ p_key[key_index]);
            }

            return Container{ret};
        }

        /*
         * @brief Encrypts a text file with repeating key XOR
         *
         * @param Container Template parameter for the type of the ciphertext return (kim::sec security type)
         *
         * @param p_in_File File with plaintext to encrypt (std::ifstream)
         * @param p_key Key to use for encryption (kim::sec::Binary)
         *
         * @return Ciphertext in the specified kim::sec security type
         */
        template <class Container>
        Container XOR_rep_key_enc(std::ifstream p_in_File, const Binary& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            Binary      ret{};
            char        curr_chr{};

            for (std::size_t key_index{}; p_in_File.get(curr_chr); key_index++) {
                if (key_index == p_key.length()) {
                    key_index = 0;
                }

                if (!isascii(curr_chr)) {
                    throw std::invalid_argument("Input file contains invalid ASCII");
                }

                ret.push_back(static_cast<std::byte>(curr_chr) ^ p_key[key_index]);
            }

            /* Pop back one byte since files have a trailing LF */
            ret.pop_back();

            return Container{ret};
        }

        /*
         * @brief Calculates the Hamming/edit distance between two kim::sec security types
         *
         * @param Container1 Template parameter for lhs parameter (std::string or kim::sec security type)
         * @param Container2 Template parameter for rhs parameter (std::string or kim::sec security type)
         *
         * @param lhs Left-hand side of Hamming/edit distance calculation (std::string or kim::sec security type)
         * @param rhs Right-hand side of Hamming/edit distance calculation (std::string or kim::sec security type)
         *
         * @return The Hamming/edit distance (std::size_t)
         */
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

        /*
         * @brief Decrypts a file containing XOR repeating key encrypted ciphertext
         *
         * @param Container Template parameter for the type of the ciphertext (kim::sec security type)
         *
         * @param p_in_File The input file containing the ciphertext (std::ifstream)
         * @param p_out_name The output file name (std::string)
         *
         * @return File with plaintext (std::ofstream)
         */
        template <class Container>
        std::ofstream XOR_rep_key_dec(std::ifstream p_in_File, const std::string& p_out_name)
        {
            std::ofstream       ret{p_out_name};
            std::string         full_ct{};

            for (std::string curr_line{}; getline(p_in_File, curr_line); full_ct += curr_line) { }

            Binary                  full_ct_Bin{Container{full_ct}};
            const std::size_t       full_ct_Bin_length{full_ct_Bin.length()};
            uint8_t                 keysize{2};

            for (std::size_t keysize_guess{2}, min_hamming_norm{std::numeric_limits<std::size_t>::max()}; keysize_guess <= 40U; keysize_guess++) {
                if (keysize_guess > full_ct_Bin_length) {
                    break;
                }

                std::size_t     curr_hamming_norm{};
                std::size_t     count{1};
                for (; (count + 1) * keysize_guess - 1 <= full_ct_Bin_length; count++) {
                    curr_hamming_norm += Hamming<Binary, Binary>(full_ct_Bin.subBin(0, keysize_guess),
                                                                 full_ct_Bin.subBin(count * keysize_guess, keysize_guess));
                }
                curr_hamming_norm /= (keysize_guess * count);

                if (curr_hamming_norm < min_hamming_norm) {
                    min_hamming_norm = curr_hamming_norm;
                    keysize = keysize_guess;
                }
            }

            std::unique_ptr<Binary[]>       XOR_byte_blocks = std::make_unique<Binary[]>(keysize);
            Binary                          key{};
            Binary                          pt_Bin{};

            for (std::vector<std::byte>::size_type ct_block_index{}; ct_block_index < full_ct_Bin_length; ct_block_index += keysize) {
                for (std::vector<std::byte>::size_type keysize_index{}; keysize_index < keysize; keysize_index++) {
                    if (ct_block_index + keysize_index > full_ct_Bin_length) {
                        break;
                    }
                    XOR_byte_blocks[keysize_index].push_back(full_ct_Bin[ct_block_index + keysize_index]);
                }
            }

            for (std::size_t index{}; index < keysize; index++) {
                key += std::get<2>(XOR_byte_dec<kim::sec::Binary>(XOR_byte_blocks[index]));
            }

            for (std::size_t ct_index{}, key_index{}; ct_index < full_ct_Bin_length; ct_index++, key_index++) {
                if (key_index == keysize) {
                    key_index = 0;
                }

                pt_Bin.push_back(key[key_index] ^ full_ct_Bin[ct_index]);
            }

            ret << pt_Bin.to_ASCII();

            return ret;
        }
    }
}

#endif /* SEC_XOR */
