#include "hex.hpp"
#include "binary.hpp"
#include "base64.hpp"

namespace edkim
{
    namespace crypto
    {
        Binary::Binary() { }

        Binary::Binary(const std::vector<uint8_t>& p_vec)
        {
            m_bin = p_vec;
        }

        Binary::~Binary() { }

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

        std::ostream& operator<<(std::ostream& os, const Binary& p_Bin)
        {
            if (p_Bin.empty()) {
                os << "{ }";
            } else {
                os << "{ " << p_Bin.m_bin[0];

                for (std::vector<uint8_t>::iterator it = ++(p_Bin.m_bin.begin()); it != p_Bin.m_bin.end(); it++) {
                    os << ", " << *it;
                }

                os << " }";
            }
        }
    }
}
