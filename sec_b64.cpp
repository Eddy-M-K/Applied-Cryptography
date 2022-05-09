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

        Base64::Base64(const std::string& p_string)
        {
            for (unsigned i{}; i < p_string.length(); i++) {
                if (!isalnum(p_string[i])) {
                    throw std::invalid_argument("Base64 string contains a non-alphanumeric");
                } else if (p_string[i] != '+' && p_string[i] != '/') {
                    throw std::invalid_argument("Base64 string contains an invalid character");
                }
            }

            for (const char& e : p_string) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Base64 string contains a non-alphanumeric");
                } else if (e != '+' && e != '/' && e != '=') {
                    throw std::invalid_argument("Base64 string contains an invalid character");
                }
            }

            m_b64 = p_string;
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

        Base64& Base64::append(const char& p_chr)
        {
            m_b64 += p_chr;

            return *this;
        }

        Base64& Base64::append(const std::string& p_str)
        {
            m_b64 += p_str;

            return *this;
        }

        Binary Base64::to_bin() const
        {
            Binary ret{};

            for (int i{}; i < m_b64.size(); i++) {
                if (m_b64[i] == '=') {
                    break;
                }


            }

            return ret;
        }

        Hex Base64::to_hex() const
        {
            Hex ret{};

            Binary tmp_bin{};

            return ret;
        }

        Base64& Base64::operator+=(const Base64& rhs)
        {
            this->m_b64 += rhs.m_b64;

            return *this;
        }

        std::ostream& operator<<(std::ostream& os, const Base64& p_B64)
        {
            os << p_B64.m_b64;
        }
    }
}
