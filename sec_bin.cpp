#include "sec_bin.hpp"
#include "sec_hex.hpp"
#include "sec_b64.hpp"

namespace kim
{
    namespace sec
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
            Hex ret{};

            const char hex_table[] = { '0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'A', 'B',
                                       'C', 'D', 'E', 'F' };

            for (std::vector<uint8_t>::const_iterator it = m_bin.begin(); it != m_bin.end(); it++) {
                ret.append(hex_table[(*it & 0b11110000) >> 4]);
                ret.append(hex_table[*it & 0b00001111]);
            }
        }

        Base64 Binary::to_b64() const
        {
            Base64 ret{};

            const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+', '/' };

            for (int i{}; i < (m_bin.size() / 3) * 3; i += 3) {
                ret.append(base64_table[m_bin[i] >> 2]);
                ret.append(base64_table[((m_bin[i] & 0b00000011) << 4) + (m_bin[i + 1] >> 4)]);
                ret.append(base64_table[((m_bin[i + 1] & 0b00001111) << 2) + (m_bin[i + 2] >> 6)]);
                ret.append(base64_table[m_bin[i + 2] & 0b00111111]);
            }

            return ret;
        }

        uint8_t Binary::operator[](const std::size_t p_index) const
        {
            return m_bin[p_index];
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

                for (std::vector<uint8_t>::const_iterator it = ++(p_Bin.m_bin.begin()); it != p_Bin.m_bin.end(); it++) {
                    os << ", " << *it;
                }

                os << " }";
            }
        }
    }
}
