/* "Security Types: Base64" Source File */
#include "types_b64.hpp"

#include <stdexcept>

#include <cctype>
#include <cstdint>
#include <cstddef>

#include "types_hex.hpp"
#include "types_bin.hpp"

namespace kim
{
    namespace sec
    {
        Base64::Base64() { }

        Base64::Base64(std::string p_str)
        {
            /* Appends padding, if necessary */
            while (p_str.length() % 4 != 0) {
                p_str.push_back('=');
                m_pad++;
            }

            const std::string::size_type p_str_len{p_str.length()};

            /* Checks if the string is valid Base64 */
            for (std::string::size_type index{}; index < p_str_len; index++) {
                const char curr{p_str[index]};

                if (!isalnum(curr) && curr != '+' && curr != '/' && curr != '=') {
                    throw std::invalid_argument(p_str + std::string(" is not a valid Base64 string"));
                } else if (curr == '=' && index != p_str_len - 1 && index != p_str_len - 2) {
                    throw std::invalid_argument(p_str + std::string(" is not a valid Base64 string "
                                                "or it has improper usage of the padding character (=)"));
                }
            }

            /* Edge case check */
            if (p_str[p_str_len - 2] == '=' && p_str[p_str_len - 1] != '=') {
                throw std::invalid_argument(p_str + std::string(" has improper padding"));
            }

            m_b64 = p_str;
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

        void Base64::reserve(const std::string::size_type p_size)
        {
            m_b64.reserve(p_size);
        }

        Base64& Base64::append(std::string p_str)
        {
            const std::string::size_type p_str_len{p_str.length()};

            /* Checks if the string is valid Base64 */
            for (std::string::size_type index{}; index < p_str.length(); index++) {
                const char curr_chr{p_str[index]};

                if (!isalnum(curr_chr) && curr_chr != '+' && curr_chr != '/' && curr_chr != '=') {
                    throw std::invalid_argument(p_str + std::string(" is not a valid Base64 string"));
                } else if (curr_chr == '=' && index != p_str_len - 1 && index != p_str_len - 2) {
                    throw std::invalid_argument(p_str + std::string(" has improper usage of the padding character (=)"));
                }
            }

            /* Edge case check */
            if (p_str[p_str_len - 2] == '=' && p_str[p_str_len - 1] != '=') {
                throw std::invalid_argument(p_str + std::string(" has improper padding"));
            }

            /* Replace any original padding in the original Base64 string */
            if (m_pad == 2 && !p_str.empty()) {
                m_b64[m_b64.length() - 2] == p_str[0];
                p_str.erase(0, 1);
                m_pad--;

                if (!p_str.empty()) {
                    m_b64[m_b64.length() - 1] == p_str[1];
                    p_str.erase(0, 1);
                    m_pad--;
                }
            } else if (m_pad == 1 && !p_str.empty()) {
                m_b64[p_str_len - 1] == p_str[0];
                p_str.erase(0, 1);
                m_pad--;
            }

            /* Append the remainder of the string to append */
            if (!p_str.empty()) {
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

        Base64& Base64::discard(const std::string::size_type p_size)
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

            for (std::string::size_type index{}; index < m_b64.length(); index += 4) {
                uint8_t sextet[4] = { };

                for (uint8_t sextet_index{}; sextet_index < 4; sextet_index++) {
                    const char curr{m_b64[index + sextet_index]};

                    if (isalpha(curr)) {
                        if (isupper(curr)) {
                            sextet[sextet_index] = curr - 'A';
                        } else {
                            sextet[sextet_index] = curr - 'a' + 26;
                        }
                    } else if (isdigit(curr)) {
                        sextet[sextet_index] = curr - '0' + 52;
                    } else {
                        switch(curr) {
                            case '+':
                                sextet[sextet_index] = 62;
                                break;
                            case '/':
                                sextet[sextet_index] = 63;
                                break;
                            default:
                                sextet[sextet_index] = 64;
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
            const Binary        this_Bin{this->to_Bin()};
            std::string         append_str{};
            const char          hex_table[] = { '0', '1', '2', '3',
                                                '4', '5', '6', '7',
                                                '8', '9', 'A', 'B',
                                                'C', 'D', 'E', 'F' };

            append_str.reserve(this_Bin.length() * 2);

            for (std::string::size_type index{}; index < this_Bin.length(); index++) {
                append_str.push_back(hex_table[std::to_integer<uint8_t>((this_Bin[index] & std::byte{0b11110000}) >> 4)]);
                append_str.push_back(hex_table[std::to_integer<uint8_t>(this_Bin[index] & std::byte{0b00001111})]);
            }

            return Hex(append_str);
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
