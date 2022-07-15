/*
 * @brief kim::sec::Base64 Header File
 * @author Edward Kim
 */
#ifndef TYPES_B64
#define TYPES_B64

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
            /*** Constructors/Destructor ***/

            /* Empty Constructor */
            Base64();

            /* Constructor which takes in a valid Base64 string with or without padding */
            Base64(std::string);

            /* Constructor which takes in a Binary object */
            Base64(const Binary&);

            /* Copy Constructor */
            Base64(const Base64&);

            /* Destructor */
            ~Base64();


            /*** Public Methods ***/

            /* Returns the length of the Base64 string */
            std::size_t         length() const;

            /* Returns true if the Base64 string is empty, else false */
            bool                empty() const;

            /* Reserves space for the Base64 string specified by a size_t argument */
            void                reserve(const std::string::size_type);

            /* Appends a string with valid Base64
             * - Will replace any padding in the original Base64 string
             * - Padding in the string argument is optional but will throw an exception if
             *   resultant string is invalid Base64
             */
            Base64&             append(std::string);

            /* Removes the specified number of Base64 digits from the back (must be a multiple of 4) */
            Base64&             discard(const std::string::size_type = 4);

            /* Returns the Binary object equivalent of the Base64 string */
            Binary              to_Bin() const;

            /* Returns the Hexadecimal object equivalent of the Base64 string */
            Hex                 to_Hex() const;


            /*** Operators ***/

            /* Appends another Base64 object */
            Base64&             operator+=(const Base64&);

            /* Returns the concatenation of two Base64 objects */
            Base64              operator+(const Base64&);


        private:
            /*** Private Member Variables ***/

            /* Underlying Data Structure */
            std::string m_b64;

            /* Padding count */
            uint8_t m_pad;


        /*** Friends ***/

        /* std::cout */
        friend std::ostream& operator<<(std::ostream&, const Base64&);
        };
    }
}

#endif /* TYPES_B64 */
