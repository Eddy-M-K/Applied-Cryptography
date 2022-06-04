/* "Security Types: Binary" Source File */
#include "types_bin.hpp"

#include <bitset>
#include <algorithm>

#include "types_hex.hpp"
#include "types_b64.hpp"

namespace kim
{
    namespace sec
    {
        Binary::Binary() { }

        Binary::Binary(std::string p_str)
        {
            if (p_str.empty()) {
                return;
            }

            for (const char& e : p_str) {
                /* Check if the string argument is an ASCII string */
                if (e != '0' && e != '1' && e != ' ') {
                    m_bin.reserve(p_str.length());

                    for (const char &e : p_str) {
                        if (!isascii(e)) {
                            throw std::invalid_argument("String contains a non-ASCII character");
                        } else {
                            m_bin.push_back(static_cast<std::byte>(e));
                        }
                    }

                    return;
                }
            }

            /* Remove any spaces */
            p_str.erase(std::remove(p_str.begin(), p_str.end(), ' '), p_str.end());

            /* String length must be a multiple of 8 */
            if (p_str.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));
            }

            m_bin.reserve(p_str.length() / 8);

            /* Check if the string is a valid binary string */
            for (std::string::size_type byte_index{}; byte_index < p_str.length(); byte_index += 8) {
                for (uint8_t bit_index{}; bit_index < 8; bit_index++) {
                    if ((p_str[byte_index + bit_index] != '0') && (p_str[byte_index + bit_index] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str.substr(byte_index, 8), nullptr, 2)));
            }
        }

        Binary::Binary(const std::vector<std::byte>& p_vec) : m_bin{p_vec} { }

        Binary::Binary(const std::byte& p_byte) : m_bin{p_byte} { }

        Binary::Binary(const Hex& p_Hex)
        {
            *this = p_Hex.to_Bin();
        }

        Binary::Binary(const Base64& p_B64)
        {
            *this = p_B64.to_Bin();
        }

        Binary::Binary(const Binary& p_Bin)
        {
            this->m_bin = p_Bin.m_bin;
        }

        Binary::~Binary() { }

        std::size_t Binary::length() const
        {
            return m_bin.size();
        }

        bool Binary::empty() const
        {
            return m_bin.empty();
        }

        void Binary::push_back(const std::byte& p_byte)
        {
            m_bin.push_back(p_byte);
        }

        void Binary::pop_back(const std::vector<std::byte>::size_type p_size)
        {
            m_bin.reserve(m_bin.size() - p_size);

            for (std::vector<std::byte>::size_type count{p_size}; count > 0; count--) {
                m_bin.pop_back();
            }
        }

        void Binary::reserve(const std::vector<std::byte>::size_type p_size)
        {
            m_bin.reserve(p_size);
        }

        Binary& Binary::append(std::string p_str)
        {
            if (p_str.empty()) {
                return *this;
            }

            /* Remove any spaces */
            p_str.erase(std::remove(p_str.begin(), p_str.end(), ' '), p_str.end());

            m_bin.reserve(m_bin.size() + p_str.length() / 8);

            /* String length must be a multiple of 8 */
            if (p_str.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));
            }

            /* Check if the string is a valid binary string */
            for (std::string::size_type byte_index{}; byte_index < p_str.length(); byte_index += 8) {
                for (uint8_t bit_index{}; bit_index < 8; bit_index++) {
                    if ((p_str[byte_index + bit_index] != '0') && (p_str[byte_index + bit_index] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str.substr(byte_index, 8), nullptr, 2)));
            }

            return *this;
        }

        Binary Binary::subBin(const std::vector<std::byte>::size_type p_index, const std::vector<std::byte>::size_type p_len)
        {
            if (!p_len) {
                throw std::invalid_argument("Length of sub Binary object is invalid");
            }

            return Binary(std::vector<std::byte>(m_bin.begin() + p_index, m_bin.begin() + p_index + p_len));
        }

        Hex Binary::to_Hex() const
        {
            std::string     ret_str{};
            const char      hex_table[] = { '0', '1', '2', '3',
                                            '4', '5', '6', '7',
                                            '8', '9', 'A', 'B',
                                            'C', 'D', 'E', 'F' };

            ret_str.reserve(m_bin.size() * 2);

            for (std::vector<std::byte>::const_iterator it = m_bin.begin(); it != m_bin.end(); it++) {
                ret_str.push_back(hex_table[std::to_integer<uint8_t>((*it & std::byte{0b11110000}) >> 4)]);
                ret_str.push_back(hex_table[std::to_integer<uint8_t>(*it & std::byte{0b00001111})]);
            }

            return Hex(ret_str);
        }

        Base64 Binary::to_B64() const
        {
            Base64 ret{};
            ret.reserve(m_bin.size() * 4 / 3 + (m_bin.size() * 4 % 3));

            std::size_t index{};
            const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+', '/' };

            /* Loop through every 3 bytes of the binary representation of the hexadecimal
               string but only until the set that does not need padding */
            for (; index < (m_bin.size() / 3) * 3; index += 3) {
                std::string tmp_str{};

                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(m_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index + 1] & std::byte{0b00001111}) << 2)
                                             + std::to_integer<uint8_t>(m_bin[index + 2] >> 6)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index + 2] & std::byte{0b00111111})]);

                ret.append(tmp_str);
            }

            /* If there are any remaining bytes, compute those and add padding */
            const unsigned long     remaining{m_bin.size() % 3};
            std::string             tmp_str{};

            if (remaining == 1) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)]);
                tmp_str.append("==");
            } else if (remaining == 2) {
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[index] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(m_bin[index + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[index + 1] & std::byte{0b00001111}) << 2)]);
                tmp_str.push_back('=');
            }

            ret.append(tmp_str);

            return ret;
        }

        std::string Binary::to_ASCII()
        {
            std::string     ret{};
            const char      *nonprint_ASCII[] = { "(NUL)", "(SOH)", "(STX)", "(ETX)", "(EOT)",
                                                  "(ENQ)", "(ACK)", "(BEL)",  "(BS)",  "(HT)",
                                                  "(LF)",  "(VT)",  "(FF)",  "(CR)",  "(SO)",
                                                  "(SI)", "(DLE)", "(DC1)", "(DC2)", "(DC3)",
                                                  "(DC4)", "(NAK)", "(SYN)", "(ETB)", "(CAN)",
                                                  "(EM)", "(SUB)", "(ESC)",  "(FS)",  "(GS)",
                                                  "(RS)",  "(US)" };

            for (const std::byte& e : m_bin) {
                uint8_t byte_int{std::to_integer<uint8_t>(e)};

                if (byte_int > 127U) {
                    return "";
                } else if (byte_int >= 0U && byte_int <= 31U) {
                    if (byte_int == 10U) {
                        ret += "\n";
                    } else {
                        ret += nonprint_ASCII[byte_int];
                    }
                } else if (byte_int == 127U) {
                    ret += "(DEL)";
                } else {
                    ret += (static_cast<char>(byte_int));
                }
            }

            return ret;
        }

        std::byte Binary::operator[](const std::vector<std::byte>::size_type p_index) const
        {
            return m_bin[p_index];
        }

        std::byte& Binary::operator[](const std::vector<std::byte>::size_type p_index)
        {
            return m_bin[p_index];
        }

        Binary& Binary::operator+=(const Binary& rhs)
        {
            this->m_bin.reserve(m_bin.size() + rhs.m_bin.size());

            for (auto& e: rhs.m_bin) {
                this->m_bin.push_back(e);
            }

            return *this;
        }

        Binary Binary::operator+(const Binary& rhs)
        {
            Binary this_copy{*this};
            this_copy.m_bin.reserve(this_copy.m_bin.size() + rhs.m_bin.size());

            for (auto& e : rhs.m_bin) {
                this_copy.push_back(e);
            }

            return this_copy;
        }

        std::ostream& operator<<(std::ostream& os, const Binary& p_Bin)
        {
            std::cout << std::bitset<8>(std::to_integer<uint8_t>(p_Bin.m_bin[0]));

            for (std::vector<std::byte>::const_iterator it = ++p_Bin.m_bin.begin(); it != p_Bin.m_bin.end(); it++) {
                os << " " << std::bitset<8>(std::to_integer<uint8_t>(*it));
            }

            return os;
        }
    }
}
