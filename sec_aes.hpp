/* AES Template Header File */
#ifndef SEC_AES
#define SEC_AES

#include <fstream>

#include "sec_types.hpp"

namespace kim
{
    namespace sec
    {
        template <class Container>
        std::ofstream aes_ecb_dec(std::ifstream p_in_File, Binary p_key)
        {
            if (p_key.length() != 16) {
                throw std::invalid_argument("AES ECB decryption key is not 16 bytes long");
            }

            std::string full_ct{};

            if (p_in_File.is_open()) {
                std::string curr_line{};
                while (getline(p_in_File, curr_line)) {
                    full_ct += curr_line;
                }
            } else {
                throw std::invalid_argument("Unable to open input file");
            }

            return std::ofstream{};
        }

    }
}

#endif /* SEC_AES */