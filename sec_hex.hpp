#ifndef SEC_HEX
#define SEC_HEX

#include <iostream>
#include <string>

namespace kim
{
    namespace sec
    {
        class Binary;
        class Base64;
    }
}

namespace kim
{
    namespace sec
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
    }
}

#endif /* SEC_HEX */
