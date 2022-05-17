/* XOR */
#ifndef SEC_XOR
#define SEC_XOR

#include <stdexcept>

namespace kim
{
    namespace sec
    {
        template<class Container>
        Container XOR(const Container& lhs, const Container& rhs)
        {
            if (lhs.length() != rhs.length()) {
                throw std::invalid_argument("For XOR operation of two buffers, they must be equal in length");
            }

            return Container(lhs.to_Bin() ^ rhs.to_Bin());
        }

        template<class Container>
        Container XOR(const Container& p_Con, const std::byte&)
        {

        }
    }
}

#endif /* SEC_XOR */
