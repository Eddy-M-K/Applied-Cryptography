#ifndef INCLUDE_CRYPTO
#define INCLUDE_CRYPTO

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include <cctype>
#include <cstdint>
#include <cstddef>

namespace edkim
{
    namespace crypto
    {
        class Hex
        {
        public:
            Hex();
            Hex(const std::string&);
            ~Hex();

            std::size_t         length() const;
            bool                empty() const;
            Hex&                append(const char&);
            Hex&                append(const std::string&);
            Binary              to_bin() const;
            Base64              to_b64() const;
            Hex&                operator+=(const Hex&);

        private:
            std::string m_hex;

        friend std::ostream& operator<<(std::ostream&, const Hex&);
        };

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

        class Base64
        {
        public:
            Base64();
            Base64(const std::string&);
            ~Base64();

            std::size_t         length() const;
            bool                empty() const;
            Base64&             append(const char&);
            Base64&             append(const std::string&);
            Hex                 to_hex() const;
            Binary              to_bin() const;
            Base64&             operator+=(const Base64&);

        private:
            std::string m_b64;

        friend std::ostream& operator<<(std::ostream&, const Base64&);
        };
    }
}

#endif /* INCLUDE_CRYPTO */
