#include "hex.hpp"
#include "binary.hpp"
#include "base64.hpp"

namespace edkim
{
    namespace crypto
    {
        Base64::Base64() { }

        Base64::Base64(const std::string& p_string)
        {
            for (const char& e : p_string) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Base64 string contains a non-alphanumeric");
                } else if (e != '+' || e != '/' || e != '=') {
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

        Hex Base64::to_hex() const
        {

        }

        Binary Base64::to_bin() const
        {

        }

        std::ostream& operator<<(std::ostream& os, const Base64& p_B64)
        {
            os << p_B64.m_b64;
        }
    }
}
