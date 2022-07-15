/* "Security Types: Binary" Header File */
#ifndef TYPES_BIN
#define TYPES_BIN

#include <iostream>
#include <vector>
#include <string>

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

            /* Constructor which takes in a
             * - valid Binary string consisting of 1s and 0s (spaces are optional)
             * - OR a valid ASCII string
             */
            Binary(std::string);

            /* Constructor which takes in a vector of bytes */
            Binary(const std::vector<std::byte>&);

            /* Constructor which takes in a single byte */
            Binary(const std::byte&);

            /* Constructor which takes in a Hexadecimal object */
            Binary(const Hex&);

            /* Constructor which takes in a Base64 object */
            Binary(const Base64&);

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

            /* Removes the specified number of bytes from the back */
            void                pop_back(const std::vector<std::byte>::size_type = 1);

            /* Reserves space for the Binary string specified by a size_t argument */
            void                reserve(const std::vector<std::byte>::size_type);

            /* Appends a valid Binary string (spaces are optional) */
            Binary&             append(std::string);

            /* Returns a part of the Binary object
             * - First argument is the index
             * - Second argument is the length
             */
            Binary              subBin(const std::vector<std::byte>::size_type, const std::vector<std::byte>::size_type);

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
            std::byte           operator[](const std::vector<std::byte>::size_type p_index) const;

            /* Subscript operator */
            std::byte&          operator[](const std::vector<std::byte>::size_type p_index);

            /* Appends another Binary object */
            Binary&             operator+=(const Binary&);

            /* Returns the concatenation of two Binary objects */
            Binary              operator+(const Binary&);


        private:
            /*** Private Member Variables ***/

            /* Underlying Data Structure */
            std::vector<std::byte> m_bin;

        /*** Friends ***/

        /* std::cout */
        friend std::ostream& operator<<(std::ostream&, const Binary&);
        };
    }
}

#endif /* TYPES_BIN */
