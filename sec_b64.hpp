/* Security Types: Base64 Header File */

#ifndef SEC_B64
#define SEC_B64

#include <iostream>
#include <string>

/* Forward Declarations */
namespace kim
{
    namespace sec
    {
        class Hex;
        class Binary;
    }
}

/* Base64 Class Declaration */
namespace kim
{
    namespace sec
    {
        class Base64
        {
        public:
            /* Constructors/Destructor */
            Base64();
            Base64(const std::string&);
            ~Base64();

            /* Public Methods */
            std::size_t         length() const;
            bool                empty() const;
            void                reserve(const std::size_t);
            Base64&             append(const std::string&);
            Binary              to_bin() const;
            Hex                 to_hex() const;

            /* Operator */
            Base64&             operator+=(const Base64&);

        private:
            /* Underlying Data Structure */
            std::string m_b64;

        /* Printer */
        friend std::ostream& operator<<(std::ostream&, const Base64&);
        };
    }
}

#endif /* SEC_B64 */
