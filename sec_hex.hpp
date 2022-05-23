/* "Security Types: Hexadecimal" Header File */
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
            /*** Constructors/Destructor ***/

            /* Empty Constructor */
            Hex();

            /* Constructor which takes in a string */
            Hex(const std::string&);

            /* Constructor which takes in a Binary object */
            Hex(const Binary&);

            /* Copy Constructor */
            Hex(const Hex&);

            /* Destructor */
            ~Hex();


            /*** Public Methods ***/

            /* Returns the length of the Hexadecimal string */
            std::size_t         length() const;

            /* Returns true if the Hexadecimal string is empty, else false */
            bool                empty() const;

            /* Reserves space for the Hexadecimal string specified by a size_t argument */
            void                reserve(const std::size_t);

            /* Appends a string with valid Hexadecimal */
            Hex&                append(const std::string&);

            /* Returns the Binary object equivalent of the Hexadecimal string */
            Binary              to_Bin() const;

            /* Returns the Base64 object equivalent of the Hexadecimal string */
            Base64              to_B64() const;


            /*** Operators ***/

            /* Appends another Hexadecimal object */
            Hex&                operator+=(const Hex&);

            /* Returns the concatenation of two Hexadecimal objects */
            Hex                 operator+(const Hex&);


        private:
            /*** Private Member Variables ***/

            /* Underlying Data Structure */
            std::string m_hex;


        /*** Friends ***/

        /* std::cout */
        friend std::ostream& operator<<(std::ostream&, const Hex&);
        };
    }
}

#endif /* SEC_HEX */
