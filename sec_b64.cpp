/* Security Types: Base64 Source File */

#include "sec_b64.hpp"
#include "sec_hex.hpp"
#include "sec_bin.hpp"

#include <exception>

#include <cctype>
#include <cstdint>
#include <cstddef>

namespace kim
{
    namespace sec
    {
        Base64::Base64() { }

        Base64::Base64(const std::string& p_str)
        {
            std::string p_str_copy{p_str};
            const std::size_t p_str_len{p_str.length()};

            while (p_str_len % 4 != 0) {
                p_str_copy.push_back('=');
            }

            /* Checks if the string is valid Base64 */
            for (unsigned i{}; i < p_str_len; i++) {
                const char curr{p_str_copy[i]};

                if (!isalnum(curr) && curr != '+' && curr != '/' && curr != '=') {
                    throw std::invalid_argument(p_str_copy + std::string(" is not a valid Base64 string"));
                } else if (curr == '=' && i != p_str_len - 1 && i != p_str_len - 2) {
                    throw std::invalid_argument(p_str_copy + std::string(" has improper usage of the padding character (=)"));
                }
            }

            m_b64 = p_str_copy;
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

            // /* Check if string to append is properly padded (if padding was used) */
            // if (p_str.find('=') != std::string::npos && p_str_len % 4 != 0) {
            //     throw std::invalid_argument(p_str + std::string(" has improper usage of the padding character (=)"));
            // }

            /* Edge case check */
            if (p_str_copy[p_str_len - 2] == '=' && p_str_copy[p_str_len - 1] != '=') {
                throw std::invalid_argument(p_str + std::string(" has improper padding"));
            }

            /* Checks if the string is valid Base64 */
            for (std::size_t i{}; i < p_str_len; i++) {
                const char curr{p_str_copy[i]};

                if (!isalnum(curr) && curr != '+' && curr != '/' && curr != '=') {
                    throw std::invalid_argument(p_str + std::string(" is not a valid Base64 string"));
                } else if (curr == '=' && i != p_str_len - 1 && i != p_str_len - 2) {
                    throw std::invalid_argument(p_str + std::string(" has improper usage of the padding character (=)"));
                }
            }

            /* Replace any original padding in the original Base64 string */
            if (m_b64[p_str_len - 2] == '=' && !p_str_copy.empty()) {
                m_b64[p_str_len - 2] == p_str_copy[0];
                p_str_copy.erase(0, 1);

                if (m_b64[p_str_copy.length() - 1] == '=' && !p_str_copy.empty()) {
                    m_b64[p_str_copy.length() - 1] == p_str_copy[1];
                    p_str_copy.erase(0, 1);
                }
            } else if (m_b64[p_str_len - 1] == '=' && !p_str_copy.empty()) {
                m_b64[p_str_len - 1] == p_str_copy[0];
                p_str_copy.erase(0, 1);
            }

            /* Append the remainder of the string to append */
            if (!p_str_copy.empty()) {
                m_b64 += p_str;
            }

            /* Padding edge case check (if p_str contains padding) */
            if (m_b64[m_b64.length() - 1] == '=') {
                if (m_b64.length() % 4 == 0) { }
                else if (m_b64.length() % 4 == 1) {
                    if (m_b64[m_b64.length() - 2] == '=') {
                        throw std::invalid_argument(p_str + std::string(" has improper padding"));
                    } else {
                        m_b64.push_back('=');
                    }
                } else {

                }
            } else if (p_str.length() % 4 == 1) {
                throw std::logic_error(std::string("Appending the string ")
                                       + p_str + std::string(" will lead to an invalid Base64 string"));
            }

            /* String length divided by 4 cannot have a remainder of 1 */
            if (p_str.length() % 4 == 1) {
                throw std::logic_error(std::string("Appending the string ")
                                       + p_str + std::string(" will lead to an invalid Base64 string"));
            }



            while (p_str_len % 4 != 0) {
                p_str_copy.push_back('=');
            }

            return *this;
        }

        Binary Base64::to_Bin() const
        {
            Binary ret{};
            ret.reserve(m_b64.size() / 4 * 3);

            for (std::size_t i{}; i < m_b64.length() / 4; i += 4) {
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

                ret.push_back(static_cast<std::byte>(((sextet[0] & 0b00111111) << 2) + ((sextet[1] & 0b00110000)) >> 4));

                if (sextet[2] == 64) {
                    break;
                }
                ret.push_back(static_cast<std::byte>(((sextet[1] & 0b00001111) << 4) + ((sextet[2] & 0b11110000)) >> 4));

                if (sextet[3] == 64) {
                    break;
                }
                ret.push_back(static_cast<std::byte>(((sextet[2] & 0b00000011) << 6) + (sextet[3] & 0b00111111)));
            }

            return ret;
        }

        Hex Base64::to_Hex() const
        {
            Hex ret{};

            Binary this_bin{this->to_Bin()};



            return ret;
        }

        Base64& Base64::operator+=(const Base64& rhs)
        {
            this->m_b64 += rhs.m_b64;

            return *this;
        }

        std::ostream& operator<<(std::ostream& os, const Base64& p_B64)
        {
            return os << p_B64.m_b64;
        }
    }
}
