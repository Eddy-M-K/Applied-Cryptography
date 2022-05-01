#ifndef INCLUDE_CRYPTO
#define INCLUDE_CRYPTO

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include <cctype>
#include <cstdint>
#include <cstddef>

namespace ekim
{
    namespace crypto
    {
        class Hex
        {
        public:
            Hex();
            Hex(const std::string&);
            ~Hex();
            std::size_t length() const;
            bool empty() const;
            void append(const std::string&);
            Binary to_bin() const;
            Base64 to_b64() const;
            friend std::ostream& operator<<(std::ostream&, const Hex&);
        private:
            std::string m_hex;
        };

        class Binary
        {
        public:
            Binary();
            Binary(const std::vector<uint8_t>&);
            ~Binary();
            std::size_t count() const;
            bool empty() const;
            void push_back(const uint8_t&);
            void append(const std::vector<uint8_t>&);
            void reserve(std::vector<uint8_t>::size_type p_size);
            Hex to_hex() const;
            Base64 to_b64() const;
            friend std::ostream& operator<<(std::ostream&, Binary&);
        private:
            std::vector<uint8_t> m_bin;
        };

        class Base64
        {
        public:
            Base64();
            Base64(const std::string&);
            ~Base64();
            std::size_t length() const;
            bool empty() const;
            void append(const std::string&);
            Hex to_hex() const;
            Binary to_bin() const;
            friend std::ostream& operator<<(std::ostream&, Base64&);
        private:
            std::string m_b64;
        };
    }
}

#endif /* INCLUDE_CRYPTO */
