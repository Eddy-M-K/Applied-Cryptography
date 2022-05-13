/* Security Types: Binary Header File */

#ifndef SEC_BIN
#define SEC_BIN

#include <iostream>
#include <vector>

#include <cstdint>
#include <cstddef>

/* Forward Declarations */
namespace kim
{
    namespace sec
    {
        class Hex;
        class Base64;
    }
}

/* Binary Class Declaration */
namespace kim
{
    namespace sec
    {
        class Binary
        {
        public:
            /*** Constructors/Destructor ***/

            /* Empty Constructor */
            Binary();
            Binary(const std::string&);
            ~Binary();

            /* Public Methods */
            std::size_t         size() const;
            bool                empty() const;
            void                push_back(const std::string&);
            void                append(const std::string&);
            void                reserve(std::vector<std::byte>::size_type);
            Hex                 to_hex() const;
            Base64              to_b64() const;

            /* Operators */
            std::byte operator[](const std::size_t p_index) const;
            std::byte& operator[](const std::size_t p_index);

        private:
            /* Underlying Data Structure */
            std::vector<std::byte> m_bin;

        /* Printer */
        friend std::ostream& operator<<(std::ostream&, const Binary&);
        };
    }
}

#endif /* SEC_BIN */
