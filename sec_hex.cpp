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
        /* Empty Constructor */
        Hex::Hex() { }

        /* Constructor which takes in a string */
        Hex::Hex(const std::string& p_str)
        {
            /* String must have an even number of characters */
            if (p_str.length() % 2 != 0) {
                throw std::invalid_argument("Hex string has an odd number of characters");
            }

            /* Check if the string is an invalid hex string */
            for (const char& e : p_str) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Hex string contains a non-alphanumeric");
                } else if (isalpha(e) && (toupper(e) > 'F' || toupper(e) < 'A')) {
                    throw std::invalid_argument("Hex string contains a letter that is not from A-F");
                } else {
                    m_hex += toupper(e);
                }
            }
        }

        /* Empty destructor */
        Hex::~Hex() { }

        /* Returns the length of the hexadecimal string */
        std::size_t Hex::length() const
        {
            return m_hex.length();
        }

        /* Returns true if the hexadecimal string is empty, else false */
        bool Hex::empty() const
        {
            return m_hex.empty();
        }

        /* Append a string to the end of the hexadecimal string */
        Hex& Hex::append(const std::string& p_str)
        {
            if (p_str.length() % 2 != 0) {
                throw std::invalid_argument("String to append has an odd number of characters");
            }

            for (const char& e : p_str) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("String to append contains a non-alphanumeric");
                } else if (isalpha(e) && (toupper(e) > 'F' || toupper(e) < 'A')) {
                    throw std::invalid_argument("String to append contains a letter that is not from A-F");
                } else {
                    m_hex += toupper(e);
                }
            }

            return *this;
        }

        /* Converts Hexadecimal string to Binary and returns the result */
        Binary Hex::to_bin() const
        {
            Binary ret{};
            ret.reserve(m_hex.length());

            for (int i{}; i < m_hex.length(); i += 2) {
                ret.push_back(std::stoi(m_hex.substr(i, 2), nullptr, 16));
            }

            return ret;
        }

        /* Converts Hexadecimal string to Base64 string and returns the result */
        Base64 Hex::to_b64() const
        {
            Base64 ret{};

            Binary this_bin{this->to_bin()};
            const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+', '/' };
            unsigned index{};
            unsigned long remaining{this_bin.size() % 3};

            for (; index < (this_bin.size() / 3) * 3; index += 3) {
                ret.append(base64_table[this_bin[index] >> 2]);
                ret.append(base64_table[((this_bin[index] & 0b00000011) << 4) + (this_bin[index + 1] >> 4)]);
                ret.append(base64_table[((this_bin[index + 1] & 0b00001111) << 2) + (this_bin[index + 2] >> 6)]);
                ret.append(base64_table[this_bin[index + 2] & 0b00111111]);
            }

            if (remaining == 1) {
                ret.append(base64_table[this_bin[index] >> 2]);
                ret.append(base64_table[((this_bin[index] & 0b00000011) << 4)]);
                ret.append("==");
            } else if (remaining == 2) {
                ret.append(base64_table[this_bin[index] >> 2]);
                ret.append(base64_table[((this_bin[index] & 0b00000011) << 4) + (this_bin[index + 1] >> 4)]);
                ret.append(base64_table[((this_bin[index + 1] & 0b00001111) << 2)]);
                ret.append('=');
            }

            return ret;
        }

        /* Appends a Hexadecimal string to the end of a Hexadecimal string */
        Hex& Hex::operator+=(const Hex& rhs)
        {
            this->m_hex += rhs.m_hex;

            return *this;
        }

        /* Overloaded << operator */
        std::ostream& operator<<(std::ostream& os, const Hex& p_Hex)
        {
            os << p_Hex.m_hex;

            return os;
        }
    }
}
