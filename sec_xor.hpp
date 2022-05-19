/* XOR Template Header File */
#ifndef SEC_XOR
#define SEC_XOR

#include <stdexcept>
#include <map>

namespace kim
{
    namespace sec
    {
        template<class Container>
        Container XOR(const Container& lhs, const Container& rhs)
        {
            if (lhs.length() != rhs.length()) {
                throw std::invalid_argument("XOR operation of two buffers requires that they are equal in length");
            }

            return Container(lhs.to_Bin() ^ rhs.to_Bin());
        }

        template<class Container>
        Container XOR(const Container& p_Con, const std::byte& p_byte)
        {
            return Container(p_Con.to_Bin() ^ p_byte);
        }

        template<class Container>
        std::map<std::size_t, Container> XOR_byte_dec(const Container& p_Con)
        {
            std::map<std::size_t, Container> ret{};

            Binary this_bin{p_Con.to_Bin()};
            const uint8_t chr_freq[] = { };

            for (uint8_t i{}; i < UINT8_MAX; i++) {
                std::string tmp{(this_bin ^ std::byte{i}).to_ASCII()};

                if (tmp.empty()) {
                    continue;
                }


            }

            return ret;
        }
    }
}

#endif /* SEC_XOR */
