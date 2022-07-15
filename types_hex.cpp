/*
 * @brief kim::sec::Hex Source File
 * @author Edward Kim
 */
#include "types_hex.hpp"

#include <stdexcept>

#include <cctype>
#include <cstdint>
#include <cstddef>

#include "types_bin.hpp"
#include "types_b64.hpp"

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

        Hex::Hex(const Binary& p_Bin)
        {
            *this = p_Bin.to_Hex();
        }

        Hex::Hex(const Hex& p_Hex)
        {
            this->m_hex = p_Hex.m_hex;
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

        void Hex::reserve(const std::string::size_type p_size)
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

        Hex& Hex::discard(const std::string::size_type p_size)
        {
            if (p_size % 2 != 0) {
                throw std::invalid_argument("Truncation size is not a multiple of 2");
            }

            m_hex = m_hex.substr(0, m_hex.length() - p_size);

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
            Base64              ret{};
            const Binary        this_Bin{this->to_Bin()};
            std::size_t         index{};
            const char          base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                                   'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                                   'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                                   'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                                   'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                                   'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                                   'q', 'r', 's', 't', 'u', 'v', 'w',
                                                   'x', 'y', 'z', '0', '1', '2', '3',
                                                   '4', '5', '6', '7', '8', '9', '+', '/' };

            ret.reserve(m_hex.length() * 2 / 3 + (m_hex.length() * 2 % 3));

            /* Loop through every 3 bytes of the binary representation of the hexadecimal
               string but only until the set that does not need padding */
            for (; index < (this_Bin.length() / 3) * 3; index += 3) {
                std::string append_str{};

                append_str.push_back(base64_table[std::to_integer<uint8_t>(this_Bin[index] >> 2)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>((this_Bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(this_Bin[index + 1] >> 4)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>((this_Bin[index + 1] & std::byte{0b00001111}) << 2)
                                             + std::to_integer<uint8_t>(this_Bin[index + 2] >> 6)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>(this_Bin[index + 2] & std::byte{0b00111111})]);

                ret.append(append_str);
            }

            /* If there are any remaining bytes, compute those and add padding */
            const std::size_t       remaining{this_Bin.length() % 3};
            std::string             append_str{};

            if (remaining == 1) {
                append_str.push_back(base64_table[std::to_integer<uint8_t>(this_Bin[index] >> 2)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>((this_Bin[index] & std::byte{0b00000011}) << 4)]);
                append_str.append("==");
            } else if (remaining == 2) {
                append_str.push_back(base64_table[std::to_integer<uint8_t>(this_Bin[index] >> 2)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>((this_Bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(this_Bin[index + 1] >> 4)]);
                append_str.push_back(base64_table[std::to_integer<uint8_t>((this_Bin[index + 1] & std::byte{0b00001111}) << 2)]);
                append_str.push_back('=');
            }

            ret.append(append_str);

            return ret;
        }

        Hex& Hex::operator+=(const Hex& rhs)
        {
            return this->append(rhs.m_hex);
        }

        Hex Hex::operator+(const Hex& rhs)
        {
            Hex this_copy{*this};

            return this_copy.append(rhs.m_hex);
        }

        std::ostream& operator<<(std::ostream& os, const Hex& p_Hex)
        {
            return os << p_Hex.m_hex;
        }
    }
}
