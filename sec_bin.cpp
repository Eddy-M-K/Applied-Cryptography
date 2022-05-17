/* "Security Types: Binary" Source File */
#include "sec_bin.hpp"

#include "sec_hex.hpp"
#include "sec_b64.hpp"

#include <bitset>
#include <algorithm>

namespace kim
{
    namespace sec
    {
        Binary::Binary() { }

        Binary::Binary(const std::string& p_str)
        {
            std::string p_str_copy{p_str};

            if (p_str_copy.empty()) {
                return;
            }

            /* Remove any spaces */
            p_str_copy.erase(std::remove(p_str_copy.begin(), p_str_copy.end(), ' '), p_str_copy.end());

            /* String length must be a multiple of 8 */
            if (p_str_copy.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));
            }

            m_bin.reserve(p_str.length() / 8);

            /* Check if the string is a valid binary string */
            for (std::size_t i{}; i < p_str_copy.length(); i += 8) {
                for (uint8_t j{}; j < 8; j++) {
                    if ((p_str_copy[i + j] != '0') && (p_str_copy[i + j] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str_copy.substr(i, 8), nullptr, 2)));
            }
        }

        Binary::Binary(const Binary& p_Bin)
        {
            this->m_bin = p_Bin.m_bin;
        }

        Binary::~Binary() { }

        std::size_t Binary::length() const
        {
            return m_bin.size();
        }

        bool Binary::empty() const
        {
            return m_bin.empty();
        }

        void Binary::push_back(const std::byte& p_byte)
        {
            m_bin.push_back(p_byte);
        }

        void Binary::reserve(const std::vector<std::byte>::size_type p_size)
        {
            m_bin.reserve(p_size);
        }

        Binary& Binary::append(const std::string& p_str)
        {
            std::string p_str_copy{p_str};

            if (p_str_copy.empty()) {
                return *this;
            }

            /* Remove any spaces */
            p_str_copy.erase(std::remove(p_str_copy.begin(), p_str_copy.end(), ' '), p_str_copy.end());

            m_bin.reserve(m_bin.size() + p_str.length() / 8);

            /* String length must be a multiple of 8 */
            if (p_str.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));
            }

            /* Check if the string is a valid binary string */
            for (std::size_t i{}; i < p_str_copy.length(); i += 8) {
                for (uint8_t j{}; j < 8; j++) {
                    if ((p_str_copy[i + j] != '0') && (p_str_copy[i + j] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str_copy.substr(i, 8), nullptr, 2)));
            }

            return *this;
        }

        Hex Binary::to_Hex() const
        {
            const char hex_table[] = { '0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'A', 'B',
                                       'C', 'D', 'E', 'F' };

            std::string tmp_str{};
            tmp_str.reserve(m_bin.size() * 2);

            for (std::vector<std::byte>::const_iterator it = m_bin.begin(); it != m_bin.end(); it++) {
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>((*it & std::byte{0b11110000}) >> 4)]);
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>(*it & std::byte{0b00001111})]);
            }

            return Hex(tmp_str);
        }

        Base64 Binary::to_B64() const
        {
            Base64 ret{};
            ret.reserve(m_bin.size() * 4 / 3 + (m_bin.size() * 4 % 3));

            std::size_t index{};
            const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+', '/' };

            /* Loop through every 3 bytes of the binary representation of the hexadecimal
               string but only until the set that does not need padding */
            for (; index < (m_bin.size() / 3) * 3; index += 3) {
                std::string tmp_str{};

                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(m_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index + 1] & std::byte{0b00001111}) << 2)
                                             + std::to_integer<uint8_t>(m_bin[index + 2] >> 6)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index + 2] & std::byte{0b00111111})]);

                ret.append(tmp_str);
            }

            /* If there are any remaining bytes, compute those and add padding */
            const unsigned long remaining{m_bin.size() % 3};
            std::string tmp_str{};

            if (remaining == 1) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)]);
                tmp_str.append("==");
            } else if (remaining == 2) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(m_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index + 1] & std::byte{0b00001111}) << 2)]);
                tmp_str.push_back('=');
            }

            ret.append(tmp_str);

            return ret;
        }

        std::byte Binary::operator[](const std::size_t p_index) const
        {
            return m_bin[p_index];
        }

        std::byte& Binary::operator[](const std::size_t p_index)
        {
            return m_bin[p_index];
        }

        Binary& Binary::operator+=(const Binary& rhs)
        {
            this->m_bin.reserve(m_bin.size() + rhs.m_bin.size());

            for (auto& e: rhs.m_bin) {
                this->m_bin.push_back(e);
            }

            return *this;
        }

        Binary Binary::operator+(const Binary& rhs)
        {
            Binary this_copy{*this};
            this_copy.m_bin.reserve(this_copy.m_bin.size() + rhs.m_bin.size());

            for (auto& e : rhs.m_bin) {
                this_copy.push_back(e);
            }

            return this_copy;
        }

        Binary Binary::to_Bin() const
        {
            return *this;
        }

        Binary Binary::operator^(const Binary& rhs)
        {
            Binary ret{};

            if (this->length() != rhs.length()) {
                throw std::logic_error("For XOR operation the two Binary strings must be equal in length");
            }

            for (std::size_t i{}; i < this->length(); i++) {
                ret.push_back(this->m_bin[i] ^ rhs.m_bin[i]);
            }

            return ret;
        }

        Binary Binary::operator^(const std::byte& rhs)
        {
            Binary ret{};

            for (auto& e : this->m_bin) {
                ret.push_back(e ^ rhs);
            }

            return ret;
        }

        std::ostream& operator<<(std::ostream& os, const Binary& p_Bin)
        {
            std::cout << std::bitset<8>(std::to_integer<uint8_t>(p_Bin.m_bin[0]));

            for (std::vector<std::byte>::const_iterator it = ++p_Bin.m_bin.begin(); it != p_Bin.m_bin.end(); it++) {
                os << " " << std::bitset<8>(std::to_integer<uint8_t>(*it));
            }

            return os;
        }

        void print_to_chr(const Binary& p_Bin)
        {
            for (auto& e : p_Bin.m_bin) {
                printf("%c", std::to_integer<uint8_t>(e));
            }
        }
    }
}
