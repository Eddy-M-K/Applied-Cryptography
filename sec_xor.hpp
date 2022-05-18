/* XOR Template Header File */
#ifndef SEC_XOR
#define SEC_XOR

#include <stdexcept>
#include <set>

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
        std::set<Container> XOR_byte_dec(const Container& p_Con)
        {
            std::set<Container,
                    [](const std::string& lhs, const std::string& rhs) {

                    })> ret{};

            Binary this_bin{p_Con.to_Bin()};

            for ()

            return ret;
        }
    }
}

#endif /* SEC_XOR */
