/* "Security Types: Binary" Header File */
#ifndef SEC_BIN
#define SEC_BIN

#include <iostream>
#include <vector>
#include <string>
#include <set>

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

            /* Constructor which takes in a valid Binary string (spaces are optional) */
            Binary(const std::string&);

            /* Copy Constructor */
            Binary(const Binary&);

            /* Destructor */
            ~Binary();


            /*** Public Methods ***/

            /* Returns the number of bytes in the Binary string */
            std::size_t         length() const;

            /* Returns true if the Binary string is empty, else false */
            bool                empty() const;

            /* Pushes back a byte */
            void                push_back(const std::byte&);

            /* Reserves space for the Binary string specified by a size_t argument */
            void                reserve(const std::vector<std::byte>::size_type);

            /* Appends a valid Binary string (spaces are optional) */
            Binary&             append(const std::string&);

            /* Returns the Hexadecimal object equivalent of the Binary string */
            Hex                 to_Hex() const;

            /* Returns the Base64 object equivalent of the Binary string */
            Base64              to_B64() const;

            /* Returns the ASCII string equivalent of the Binary object
             * - If the string contains invalid ASCII, the method will return an empty string
             */
            std::string         to_ASCII();


            /*** Public Member Operators ***/

            /* Constant subscript operator */
            std::byte           operator[](const std::size_t p_index) const;

            /* Subscript operator */
            std::byte&          operator[](const std::size_t p_index);

            /* Appends another Binary object */
            Binary&             operator+=(const Binary&);

            /* Returns the concatenation of two Binary objects */
            Binary              operator+(const Binary&);


        private:
            /*** Private Member Variables ***/

            /* Underlying Data Structure */
            std::vector<std::byte> m_bin;


            /*** Private Member Functions ***/

            /* Used for compatibility with template functions */
            Binary              to_Bin() const;


            /*** Private Member Operators ***/

            /* Returns the XOR result of two equal-length Binary objects */
            Binary              operator^(const Binary&);

            /* Returns the result of XORing with a single-byte */
            Binary              operator^(const std::byte&);

        /*** Friends ***/

        /* std::cout */
        friend std::ostream& operator<<(std::ostream&, const Binary&);

        /* XOR of two containers */
        template <class Container>
        friend Container XOR(const Container& lhs, const Container& rhs);

        /* XOR of container and byte */
        template<class Container>
        friend Container XOR(const Container& p_Con, const std::byte& p_byte);

        template<class Container>
        friend auto XOR_byte_dec(const Container& p_Con);
        };
    }
}

#endif /* SEC_BIN */
