/* Security Types: Hexadecimal Source File */

#include "sec_hex.hpp"
#include "sec_bin.hpp"
#include "sec_b64.hpp"

#include <exception>

#include <cctype>
#include <cstdint>
#include <cstddef>

namespace kim
{
    namespace sec
    {
        Hex::Hex() { }

        Hex::Hex(const std::string& p_str)
        {
            if (p_str.empty()) {
                return;
            }

            /* String must have an even number of characters */
            if (p_str.length() % 2 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not even"));
            }

            m_hex.reserve(p_str.length());

            /* Check if the string is a valid Hexadecimal string */
            for (const char& e : p_str) {
                if (!isalnum(e)) {
                    throw std::invalid_argument(p_str + std::string(" contains a non-alphanumeric"));
                } else if (isalpha(e) && (toupper(e) > 'F' || toupper(e) < 'A')) {
                    throw std::invalid_argument(p_str + std::string(" contains a letter that is not from A-F"));
                } else {
                    m_hex.push_back(toupper(e));
                }
            }
        }

        Hex::~Hex() { }

        std::size_t Hex::length() const
        {
            return m_hex.length();
        }

        bool Hex::empty() const
        {
            return m_hex.empty();
        }

        void Hex::reserve(const std::size_t p_size)
        {
            m_hex.reserve(p_size);
        }

        Hex& Hex::append(const std::string& p_str)
        {
            if (p_str.empty()) {
                return *this;
            }

            /* String must have an even number of characters */
            if (p_str.length() % 2 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not even"));
            }

            /* Check if the string is a valid Hexadecimal string */
            for (const char& e : p_str) {
                if (!isalnum(e)) {
                    throw std::invalid_argument(p_str + std::string(" contains a non-alphanumeric"));
                } else if (isalpha(e) && (toupper(e) > 'F' || toupper(e) < 'A')) {
                    throw std::invalid_argument(p_str + std::string(" contains a letter that is not from A-F"));
                } else {
                    m_hex.push_back(toupper(e));
                }
            }

            return *this;
        }

        Binary Hex::to_Bin() const
        {
            Binary ret{};
            ret.reserve(m_hex.length());

            for (std::size_t i{}; i < m_hex.length(); i += 2) {
                ret.push_back(static_cast<std::byte>(std::stoi(m_hex.substr(i, 2), nullptr, 16)));
            }

            return ret;
        }

        Base64 Hex::to_B64() const
        {
            Base64 ret{};
            ret.reserve(m_hex.length() * 2 / 3 + (m_hex.length() * 2 % 3));

            Binary this_bin{this->to_Bin()};
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
            for (; index < (this_bin.length() / 3) * 3; index += 3) {
                std::string tmp_str{};

                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(this_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((this_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(this_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((this_bin[index + 1] & std::byte{0b00001111}) << 2)
                                             + std::to_integer<uint8_t>(this_bin[index + 2] >> 6)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(this_bin[index + 2] & std::byte{0b00111111})]);

                ret.append(tmp_str);
            }

            /* If there are any remaining bytes, compute those and add padding */
            const unsigned long remaining{this_bin.length() % 3};
            std::string tmp_str{};

            if (remaining == 1) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(this_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((this_bin[index] & std::byte{0b00000011}) << 4)]);
                tmp_str.append("==");
            } else if (remaining == 2) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(this_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((this_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(this_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((this_bin[index + 1] & std::byte{0b00001111}) << 2)]);
                tmp_str.push_back('=');
            }

            ret.append(tmp_str);

            return ret;
        }

        Hex& Hex::operator+=(const Hex& rhs)
        {
            this->m_hex += rhs.m_hex;

            return *this;
        }

        std::ostream& operator<<(std::ostream& os, const Hex& p_Hex)
        {
            return os << p_Hex.m_hex;
        }
    }
}
