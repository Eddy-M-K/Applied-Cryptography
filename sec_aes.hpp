/*
 * @brief AES Template Header File
 * @author Edward Kim
 */
#ifndef SEC_AES
#define SEC_AES

#include <fstream>
#include <array>

#include <cstddef>

#include "sec_xor.hpp"
#include "sec_types.hpp"

namespace kim
{
    namespace sec
    {
        static inline void rev_sub_bytes(std::array<std::array<std::byte, 4>, 4>& p_state_array)
        {
            return;
        }

        static inline void rev_shift_rows(std::array<std::array<std::byte, 4>, 4>& p_state_array)
        {
            return;
        }

        static inline void rev_mix_columns(std::array<std::array<std::byte, 4>, 4>& p_state_array)
        {
            return;
        }

        static inline void add_round_key(std::array<std::array<std::byte, 4>, 4>& p_state_array)
        {
            return;
        }

        template <class Container>
        std::ofstream aes_ecb_dec(std::ifstream& p_in_File, const Binary& p_key, const std::string& p_out_name)
        {
            if (p_key.length() != 16) {
                throw std::invalid_argument("AES ECB decryption key is not 16 bytes long");
            }

            std::ofstream       ret{p_out_name};
            std::string         full_ct{};

            for (std::string curr_line{}; getline(p_in_File, curr_line);) {
                full_ct += curr_line;
            }

            Binary      full_ct_Bin{Container{full_ct}};

            for (std::vector<std::byte>::size_type index{}; index < full_ct_Bin.length(); index += 16) {

            }

            return std::ofstream{};
        }

    }
}

#endif /* SEC_AES */