#ifndef INCLUDE_CRYPTO
#define INCLUDE_CRYPTO

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include <cctype>
#include <cstdint>

namespace ekim
{
    namespace crypto
    {
        class Hex
        {
        public:
            Hex(const std::string&);
            ~Hex();
            bool empty();
            void append(const std::string&);
            Binary to_bin();
            Base64 to_b64();
            friend std::ostream& operator<<(std::ostream&, const Hex&);
        private:
            std::string m_hex;
        };

        class Binary
        {
        public:
            Binary(const std::vector<uint8_t>&);
            ~Binary();
            bool empty();
            void append(const std::vector<uint8_t>&);
            Hex to_hex();
            Base64 to_b64();
            friend std::ostream& operator<<(std::ostream&, Binary&);
        private:
            std::vector<uint8_t> m_bin;
        };

        class Base64
        {
        public:
            Base64(const std::string&);
            ~Base64();
            bool empty();
            void append(const std::string&);
            Hex to_hex();
            Binary to_bin();
            friend std::ostream& operator<<(std::ostream&, Base64&);
        private:
            std::string m_b64;
        };
    }
}

#endif /* INCLUDE_CRYPTO */
