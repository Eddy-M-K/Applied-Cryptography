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
        /* Hex class function definitions */

        Hex::Hex() { }

        Hex::Hex(const std::string& p_string)
        {
            if (p_string.length() % 2 != 0) {
                throw std::invalid_argument("Hex string has an odd number of characters");
            }

            for (const char& e : p_string) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Hex string contains a non-alphanumeric");
                } else if (isalpha(e) && (toupper(e) > 'F' || toupper(e) < 'A')) {
                    throw std::invalid_argument("Hex string contains a letter that is not from A-F");
                } else {
                    m_hex += toupper(e);
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

        Hex& Hex::append(const char& p_chr)
        {
            m_hex += p_chr;

            return *this;
        }

        Hex& Hex::append(const std::string& p_str)
        {
            m_hex += p_str;

            return *this;
        }

        Binary Hex::to_bin() const
        {
            Binary ret{};
            ret.reserve(m_hex.length());

            for (int i{}; i < m_hex.length(); i += 2) {
                ret.push_back(std::stoi(m_hex.substr(i, 2), nullptr, 16));
            }

            return ret;
        }

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
            for (; index < (this_bin.size() / 3) * 3; index += 3) {
                ret.append(base64_table[this_bin[index] >> 2]);
                ret.append(base64_table[((this_bin[index] & 0b00000011) << 4) + (this_bin[index + 1] >> 4)]);
                ret.append(base64_table[((this_bin[index + 1] & 0b00001111) << 2) + (this_bin[index + 2] >> 6)]);
                ret.append(base64_table[this_bin[index + 2] & 0b00111111]);
            }

            uint8_t remaining{this_bin.size() % 3};

            if (remaining == 1) {
                ret.append(base64_table[this_bin[i] >> 2]);
                ret.append(base64_table[((this_bin[i] & 0b00000011) << 4);
                ret.append("==");
            } else if (remaining == 2) {
                ret.append(base64_table[this_bin[i] >> 2]);
                ret.append(base64_table[((this_bin[i] & 0b00000011) << 4);

            }

            for (int i{}; i < 3 - (this_bin.size() % 3); i++) {
                ret.append('=');
            }

            return ret;
        }

        Hex& Hex::operator+=(const Hex& rhs)
        {
            this->m_hex += rhs.m_hex;

            return *this;
        }

        std::ostream& operator<<(std::ostream& os, const Hex& p_Hex)
        {
            os << p_Hex.m_hex;
        }
    }
}
