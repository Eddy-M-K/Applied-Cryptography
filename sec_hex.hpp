/* Security Types: Hexadecimal Header File */

#ifndef SEC_HEX
#define SEC_HEX

#include <iostream>
#include <string>

/* Forward Declarations */
namespace kim
{
    namespace sec
    {
        class Binary;
        class Base64;
    }
}

/* Hexadecimal Class Declaration */
namespace kim
{
    namespace sec
    {
        class Hex
        {
        public:
            /* Constructors/Destructor */
            Hex();
            Hex(const std::string&);
            ~Hex();

            /* Public Methods */
            std::size_t         length() const;
            bool                empty() const;
            Hex&                append(const std::string&);
            Binary              to_bin() const;
            Base64              to_b64() const;

            /* Operator */
            Hex&                operator+=(const Hex&);

        private:
            /* Underlying Data Structure */
            std::string m_hex;

        /* Printer */
        friend std::ostream& operator<<(std::ostream&, const Hex&);
        };
    }
}

#endif /* SEC_HEX */
