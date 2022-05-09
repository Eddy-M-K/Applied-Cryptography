#ifndef SEC_B64
#define SEC_B64

#include <iostream>
#include <string>

namespace kim
{
    namespace sec
    {
        class Hex;
        class Binary;
    }
}

namespace kim
{
    namespace sec
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
            Binary              to_bin() const;
            Hex                 to_hex() const;
            Base64&             operator+=(const Base64&);

        private:
            std::string m_b64;

        friend std::ostream& operator<<(std::ostream&, const Base64&);
        };
    }
}

#endif /* SEC_B64 */
