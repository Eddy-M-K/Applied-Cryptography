#ifndef INCLUDE_BIN
#define INCLUDE_BIN

namespace edkim
{
    namespace crypto
    {
        class Hex;
        class Base64;
    }
}

namespace edkim
{
    namespace crypto
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

#endif /* INCLUDE_BIN */
