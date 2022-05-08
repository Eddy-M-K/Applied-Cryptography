#ifndef INCLUDE_B64
#define INCLUDE_B64

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
        class Hex;
        class Binary;
    }
}

namespace edkim
{
    namespace crypto
    {
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

#endif /* INCLUDE_B64 */
