#include "crypto.hpp"

namespace ekim
{
    namespace crypto
    {
        /* Hex class function definitions */

        Hex::Hex() { }

        Hex::Hex(const std::string& p_string)
        {
            if (p_string.length() % 2 != 0) {
                throw std::invalid_argument("Hex string has an odd number of digits");
            }

            for (const char& e : p_string) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Hex string contains a non-alphanumeric");
                } else if (toupper(e) > 'F') {
                    throw std::invalid_argument("Hex string contains a letter that is not from A-F");
                }
            }

            m_hex = p_string;
        }

        Hex::~Hex() { }

        std::size_t Hex::length() const
        {
            return m_hex.length();
        }

        bool Hex::empty() const
        {
            return this->m_hex.empty();
        }

        std::string& Hex::append(const std::string& p_str)
        {
            m_hex += p_str;

            return m_hex;
        }

        Hex& Hex::append(const Hex&)
        {

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

        }

        std::ostream& operator<<(std::ostream& os, const Hex& p_hex)
        {
            os << p_hex.m_hex;
        }

        /* Binary class function definitions */

        Binary::Binary() { }

        Binary::Binary(const std::vector<uint8_t>& p_vec)
        {
            m_bin = p_vec;
        }

        Binary::~Binary() {}

        void Binary::push_back(const uint8_t& p_uint8)
        {
            m_bin.push_back(p_uint8);
        }

        std::size_t Binary::size() const
        {
            return m_bin.size();
        }

        void Binary::append(const std::vector<uint8_t>& p_vec)
        {
            m_bin.insert(m_bin.end(), std::begin(p_vec), std::end(p_vec));
        }

        void Binary::reserve(std::vector<uint8_t>::size_type p_size)
        {
            m_bin.reserve(p_size);
        }

        bool Binary::empty() const
        {
            return m_bin.empty();
        }

        Hex Binary::to_hex() const
        {

        }

        Base64 Binary::to_b64() const
        {

        }

        uint8_t& Binary::operator[](const std::size_t p_index)
        {
            return m_bin[p_index];
        }

        std::ostream& operator<<(std::ostream& os, Binary& p_bin)
        {
            if (p_bin.empty()) {
                os << "{ }";
            } else {
                os << "{ " << p_bin.m_bin[0];

                for (std::vector<uint8_t>::iterator it = ++p_bin.m_bin.begin(); it != p_bin.m_bin.end(); it++) {
                    os << ", " << *it;
                }

                os << " }";
            }
        }

        /* Base64 class function definitions */

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

        Base64::~Base64() {}

        std::size_t Base64::length() const
        {
            return m_b64.length();
        }

        bool Base64::empty() const
        {
            return m_b64.empty();
        }

        std::string& Base64::append(const std::string& p_str)
        {
            m_b64 += p_str;

            return m_b64;
        }

        Hex Base64::to_hex() const
        {

        }

        Binary Base64::to_bin() const
        {

        }

        std::ostream& operator<<(std::ostream& os, Base64& p_b64)
        {
            os << p_b64.m_b64;
        }
    }
}
