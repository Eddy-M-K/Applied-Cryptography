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
#include <memory>

#include "types_bin.hpp"

namespace kim
{
    namespace sec
    {
        /* XORs two security types
         *
         * Template argument 1: The kim::sec security type of argument 1
         * Template argument 2: The kim::sec security type of argument 2
         *
         * Argument 1: LHS of XOR operation (kim::sec security type)
         * Argument 2: RHS of XOR operation (kim::sec security type)
         *
         * Return: The Binary object result of the XOR operation
         */
        template<class Container1, class Container2>
        Binary XOR(const Container1& lhs, const Container2& rhs)
        {
            Binary      ret_Bin{};
            Binary      lhs_Bin{lhs};
            Binary      rhs_Bin{rhs};

            if (lhs_Bin.length() != rhs_Bin.length()) {
                throw std::invalid_argument("XOR operation of two buffers requires that they are equal in length");
            }

            ret_Bin.reserve(lhs_Bin.length());

            for (std::vector<std::byte>::size_type index{}; index < lhs_Bin.length(); index++) {
                ret_Bin.push_back(lhs_Bin[index] ^ rhs_Bin[index]);
            }

            return ret_Bin;
        }

        /* XORs a security type and an std::byte
         *
         * Template argument: The input kim::sec security type
         *
         * Argument 1: LHS of XOR operation (kim::sec security type)
         * Argument 2: RHS of XOR operation (std::byte)
         *
         * Return: The Binary object result of the XOR operation
         */
        template<class Container>
        Binary XOR(const Container& p_Con, const std::byte& p_byte)
        {
            Binary      ret_Bin{};
            Binary      p_Con_Bin{p_Con};

            ret_Bin.reserve(p_Con_Bin.length());

            for (std::vector<std::byte>::size_type index{}; index < p_Con_Bin.length(); index++) {
                ret_Bin.push_back(p_Con_Bin[index] ^ p_byte);
            }

            return ret_Bin;
        }

        /* Decrypts a XOR byte encrypted ciphertext string
         *
         * Template argument: The input kim::sec security type
         *
         * Argument: Ciphertext (kim::sec security type)
         *
         * Return: A tuple in the form of { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string }
         */
        template<class Container>
        std::tuple<const std::size_t,
                   const Container,
                   const Binary,
                   const std::string>
        XOR_byte_dec(const Container& p_Con)
        {
            /* Custom comparator */
            auto cmp{ [](std::tuple<const std::size_t, const Container, const Binary, const std::string> lhs,
                         std::tuple<const std::size_t, const Container, const Binary, const std::string> rhs)
            {
                return (std::get<0>(lhs) > std::get<0>(rhs));
            }};
            /* Set with each entry comprising of { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string } */
            std::set<std::tuple<const std::size_t, const Container, const Binary, const std::string>, decltype(cmp)> ret_set(cmp);

            Binary              p_Con_Bin{p_Con};
            const uint16_t      chr_freq[] = { 609, 105, 284, 292, 1136, 179,
                                               138, 341, 544,  24,   41, 292,
                                               276, 544, 600, 195,   24, 495,
                                               568, 803, 243,  97,  138,  24,
                                               130,   3 };
            const char          *nonprint_ASCII[] = { "(NUL)", "(SOH)", "(STX)", "(ETX)", "(EOT)",
                                                      "(ENQ)", "(ACK)", "(BEL)",  "(BS)",  "(HT)",
                                                      "(LF)",  "(VT)",  "(FF)",  "(CR)",  "(SO)",
                                                      "(SI)", "(DLE)", "(DC1)", "(DC2)", "(DC3)",
                                                      "(DC4)", "(NAK)", "(SYN)", "(ETB)", "(CAN)",
                                                      "(EM)", "(SUB)", "(ESC)",  "(FS)",  "(GS)",
                                                      "(RS)",  "(US)" };

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
                    ret_set.insert(std::make_tuple(score, p_Con, Binary{std::byte{i}}, ASCII_string));
                }

            } while (i++ != UINT8_MAX);

            if (!ret_set.empty()) {
                return *ret_set.begin();
            } else {
                return std::tuple<const std::size_t, const Container, const Binary, const std::string>{};
            }
        }

        /* Decrypts a file with XOR byte encrypted ciphertext
         *
         * Template argument: The kim::sec security type of the input ciphertext
         *
         * Argument: Input file with the ciphertext (std::ifstream)
         *
         * Return: A set with tuples in the form of { Score: std::size_t | Ciphertext: Container | Byte: Binary | Plaintext: std::string }
         */
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
                std::tuple<const std::size_t, const Container, const Binary, const std::string> best_candidate{XOR_byte_dec<Container>(Container{line})};
                if (std::get<0>(best_candidate)) {
                    ret.insert(best_candidate);
                }
            }

            return ret;
        }

        /* Encrypts a string using repeating key XOR
         *
         * Template argument: The kim::sec security type of the ciphertext return
         *
         * Argument 1: Plaintext to encrypt (std::string)
         * Argument 2: Key (kim::sec::Binary)
         *
         * Return: Ciphertext in the specified kim::sec security type
         */
        template <class Container>
        Container XOR_rep_key_enc(const std::string& p_pt, const Binary& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            Binary      ret_Bin{};
            Binary      p_pt_Bin{p_pt};

            ret_Bin.reserve(p_pt_Bin.length());

            for (std::size_t pt_index{}, key_index{}; pt_index < p_pt_Bin.length(); pt_index++, key_index++) {
                if (key_index == p_key.length()) {
                    key_index = 0;
                }

                ret_Bin.push_back(p_pt_Bin[pt_index] ^ p_key[key_index]);
            }

            return Container{ret_Bin};
        }

        /* Encrypts a text file with repeating key XOR
         *
         * Template argument: The kim::sec security type of the ciphertext return
         *
         * Argument 1: File with ciphertext to encrypt (std::ifstream)
         * Argument 2: Key (kim::sec::Binary)
         *
         * Return: Ciphertext in the specified kim::sec security type
         */
        template <class Container>
        Container XOR_rep_key_enc(std::ifstream& p_in_File, const Binary& p_key)
        {
            if (p_key.empty()) {
                throw std::invalid_argument("Key cannot be empty");
            }

            Binary      ret_Bin{};
            char        curr_chr{};

            for (std::size_t key_index{}; p_in_File.get(curr_chr); key_index++) {
                if (key_index == p_key.length()) {
                    key_index = 0;
                }

                if (!isascii(curr_chr)) {
                    throw std::invalid_argument("Input file contains invalid ASCII");
                }

                ret_Bin.push_back(static_cast<std::byte>(curr_chr) ^ p_key[key_index]);
            }

            ret_Bin.pop_back();

            return Container{ret_Bin};
        }

        /* Calculates the Hamming/edit distance between two kim::sec security types
         *
         * Template argument 1: std::string or the kim::sec security type of argument 1
         * Template argument 2: std::string or the kim::sec security type of argument 2
         *
         * Argument 1: std::string or the first kim::sec security type object
         * Argument 2: std::string or the second kim::sec security type object
         *
         * Return: The Hamming/edit distance (std::size_t)
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

        /* Decrypts a file with XOR repeating key encrypted ciphertext
         *
         * Template argument: The kim::sec security type format of the ciphertext
         *
         * Argument 1: The input file with the ciphertext (std::ifstream)
         * Argument 2: The output file name (std::string)
         *
         * Return: The output file (std::ofstream)
         */
        template <class Container>
        std::ofstream XOR_rep_key_dec(std::ifstream& p_in_File, const std::string& p_out_name)
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
