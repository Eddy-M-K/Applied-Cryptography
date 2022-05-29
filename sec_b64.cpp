/* "Security Types: Base64" Source File */
#include "sec_b64.hpp"

#include <stdexcept>

#include <cctype>
#include <cstdint>
#include <cstddef>

#include "sec_hex.hpp"
#include "sec_bin.hpp"

namespace kim
{
    namespace sec
    {
        Base64::Base64() { }

        Base64::Base64(const std::string& p_str)
        {
            std::string p_str_copy{p_str};

            /* Appends padding, if necessary */
            while (p_str_copy.length() % 4 != 0) {
                p_str_copy.push_back('=');
                m_pad++;
            }
            const std::size_t p_str_copy_len{p_str_copy.length()};

            /* Checks if the string is valid Base64 */
            for (std::size_t i{}; i < p_str_copy_len; i++) {
                const char curr{p_str_copy[i]};

                if (!isalnum(curr) && curr != '+' && curr != '/' && curr != '=') {
                    throw std::invalid_argument(p_str_copy + std::string(" is not a valid Base64 string"));
                } else if (curr == '=' && i != p_str_copy_len - 1 && i != p_str_copy_len - 2) {
                    throw std::invalid_argument(p_str_copy + std::string(" is not a valid Base64 string "
                                                "or it has improper usage of the m_pad character (=)"));
                }
            }

            /* Edge case check */
            if (p_str_copy[p_str_copy_len - 2] == '=' && p_str_copy[p_str_copy_len - 1] != '=') {
                throw std::invalid_argument(p_str + std::string(" has improper m_pad"));
            }

            m_b64 = p_str_copy;
        }

        Base64::Base64(const Binary& p_Bin)
        {
            *this = p_Bin.to_B64();
        }

        Base64::Base64(const Base64& p_B64)
        {
            this->m_b64 = p_B64.m_b64;
            this->m_pad = p_B64.m_pad;
        }

        Base64::~Base64() { }

        std::size_t Base64::length() const
        {
            return m_b64.length();
        }

        bool Base64::empty() const
        {
            return m_b64.empty();
        }

        void Base64::reserve(const std::size_t p_size)
        {
            m_b64.reserve(p_size);
        }

        Base64& Base64::append(const std::string& p_str)
        {
            std::string p_str_copy{p_str};
            const std::size_t p_str_len{p_str.length()};

            /* Checks if the string is valid Base64 */
            for (std::size_t i{}; i < p_str_len; i++) {
                const char curr{p_str_copy[i]};

                if (!isalnum(curr) && curr != '+' && curr != '/' && curr != '=') {
                    throw std::invalid_argument(p_str + std::string(" is not a valid Base64 string"));
                } else if (curr == '=' && i != p_str_len - 1 && i != p_str_len - 2) {
                    throw std::invalid_argument(p_str + std::string(" has improper usage of the m_pad character (=)"));
                }
            }

            /* Edge case check */
            if (p_str_copy[p_str_len - 2] == '=' && p_str_copy[p_str_len - 1] != '=') {
                throw std::invalid_argument(p_str + std::string(" has improper m_pad"));
            }

            /* Replace any original padding in the original Base64 string */
            if (m_pad == 2 && !p_str_copy.empty()) {
                m_b64[m_b64.length() - 2] == p_str_copy[0];
                p_str_copy.erase(0, 1);
                m_pad--;

                if (!p_str_copy.empty()) {
                    m_b64[m_b64.length() - 1] == p_str_copy[1];
                    p_str_copy.erase(0, 1);
                    m_pad--;
                }
            } else if (m_pad == 1 == '=' && !p_str_copy.empty()) {
                m_b64[p_str_len - 1] == p_str_copy[0];
                p_str_copy.erase(0, 1);
                m_pad--;
            }

            /* Append the remainder of the string to append */
            if (!p_str_copy.empty()) {
                m_b64 += p_str;
            }

            /* Remove any padding */
            if (m_b64[m_b64.length() - 2] == '=') {
                m_b64.erase(m_b64.length() - 2);
            } else if (m_b64[m_b64.length() - 1] == '=') {
                m_b64.erase(m_b64.length() - 1);
            }

            /* String length divided by 4 cannot have a remainder of 1 */
            if (m_b64.length() % 4 == 1) {
                throw std::logic_error(std::string("Appending the string ")
                                       + p_str + std::string(" will lead to an invalid Base64 string"));
            }

            /* Add any padding, if necessary */
            while (m_b64.length() % 4 != 0) {
                m_b64.push_back('=');
                m_pad++;
            }

            return *this;
        }

        Base64& Base64::truncate(const std::string::size_type p_size)
        {
            if (p_size % 4 != 0) {
                throw std::invalid_argument("Truncation size is not a multiple of 4");
            }

            m_b64 = m_b64.substr(0, m_b64.length() - p_size);

            return *this;
        }

        Binary Base64::to_Bin() const
        {
            Binary ret{};
            ret.reserve(m_b64.size() / 4 * 3);

            for (std::size_t i{}; i < m_b64.length(); i += 4) {
                uint8_t sextet[4] = { };

                for (uint8_t j{}; j < 4; j++) {
                    const char curr{m_b64[i + j]};

                    if (isalpha(curr)) {
                        if (isupper(curr)) {
                            sextet[j] = curr - 'A';
                        } else {
                            sextet[j] = curr - 'a' + 26;
                        }
                    } else if (isdigit(curr)) {
                        sextet[j] = curr - '0' + 52;
                    } else {
                        switch(curr) {
                            case '+':
                                sextet[j] = 62;
                                break;
                            case '/':
                                sextet[j] = 63;
                                break;
                            default:
                                sextet[j] = 64;
                        }
                    }
                }

                ret.push_back(static_cast<std::byte>(((sextet[0] & 0b00111111) << 2) + ((sextet[1] & 0b00110000) >> 4)));

                if (sextet[2] == 64) {
                    continue;
                }
                ret.push_back(static_cast<std::byte>(((sextet[1] & 0b00001111) << 4) + ((sextet[2] & 0b00111100) >> 2)));

                if (sextet[3] == 64) {
                    continue;
                }
                ret.push_back(static_cast<std::byte>(((sextet[2] & 0b00000011) << 6) + (sextet[3] & 0b00111111)));
            }

            return ret;
        }

        Hex Base64::to_Hex() const
        {
            const Binary this_Bin{this->to_Bin()};

            const char hex_table[] = { '0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'A', 'B',
                                       'C', 'D', 'E', 'F' };

            std::string tmp_str{};
            tmp_str.reserve(this_Bin.length() * 2);

            for (std::size_t i{}; i < this_Bin.length(); i++) {
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>((this_Bin[i] & std::byte{0b11110000}) >> 4)]);
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>(this_Bin[i] & std::byte{0b00001111})]);
            }

            return Hex(tmp_str);
        }

        Base64& Base64::operator+=(const Base64& rhs)
        {
            return this->append(rhs.m_b64);
        }

        Base64 Base64::operator+(const Base64& rhs)
        {
            Base64 this_copy{*this};

            return this_copy.append(rhs.m_b64);
        }

        std::ostream& operator<<(std::ostream& os, const Base64& p_B64)
        {
            return os << p_B64.m_b64;
        }
    }
}
