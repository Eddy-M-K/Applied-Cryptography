#include "crypto.hpp"

namespace ekim
{
    namespace crypto
    {
        /* Hex class function definitions */

        Hex::Hex(const std::string& p_string)
        {
            for (const char& e : p_string) {
                if (!isalnum(e)) {
                    throw std::invalid_argument("Hex string contains a non-alphanumeric");
                } else if (toupper(e) > 'F') {
                    throw std::invalid_argument("Hex string contains a letter that is not from A-F");
                }
            }

            m_hex = p_string;
        }

        Hex::~Hex() {}

        bool Hex::empty()
        {
            return this->m_hex.empty();
        }

        Binary Hex::to_bin()
        {

        }

        Base64 Hex::to_b64()
        {
            Binary tmp_bin{this->to_bin()};
        }

        std::ostream& operator<<(std::ostream& os, const Hex& p_hex)
        {
            os << p_hex.m_hex;
        }

        /* Binary class function definitions */

        Binary::Binary(const std::vector<uint8_t>& p_vec)
        {
            m_bin = p_vec;
        }

        Binary::~Binary() {}

        void Binary::append(const std::vector<uint8_t>& p_vec)
        {
            m_bin.insert(m_bin.end(), std::begin(p_vec), std::end(p_vec));
        }

        bool Binary::empty()
        {
            return m_bin.empty();
        }

        Hex Binary::to_hex()
        {

        }

        Base64 Binary::to_b64()
        {

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

        bool Base64::empty()
        {
            return m_b64.empty();
        }

        Hex Base64::to_hex()
        {

        }

        Binary Base64::to_bin()
        {

        }

        std::ostream& operator<<(std::ostream& os, Base64& p_b64)
        {
            os << p_b64.m_b64;
        }
    }
}
