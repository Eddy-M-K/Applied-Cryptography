#ifndef SEC_BIN
#define SEC_BIN

#include <iostream>
#include <vector>

#include <cstdint>

namespace kim
{
    namespace sec
    {
        class Hex;
        class Base64;
    }
}

namespace kim
{
    namespace sec
    {
        class Binary
        {
        public:
            Binary();
            Binary(const std::vector<uint8_t>&);
            ~Binary();

            std::size_t         size() const;
            bool                empty() const;
            void                push_back(const uint8_t&);
            void                append(const std::vector<uint8_t>&);
            void                reserve(std::vector<uint8_t>::size_type p_size);
            Hex                 to_hex() const;
            Base64              to_b64() const;

            uint8_t operator[](const std::size_t p_index) const;
            uint8_t& operator[](const std::size_t p_index);

        private:
            std::vector<uint8_t> m_bin;

        friend std::ostream& operator<<(std::ostream&, const Binary&);
        };
    }
}

#endif /* SEC_BIN */
