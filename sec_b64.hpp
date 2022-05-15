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
            /*** Constructors/Destructor ***/

            /* Empty Constructor */
            Base64();

            /* Constructor which takes in a string */
            Base64(const std::string&);

            /* Destructor */
            ~Base64();


            /*** Public Methods ***/

            /* Returns the length of the Base64 string */
            std::size_t         length() const;

            /* Returns true if the Base64 string is empty, else false */
            bool                empty() const;

            /* Reserves space for the Base64 string specified by a size_t argument */
            void                reserve(const std::size_t);

            /* Appends a string with valid Base64
             * - Will replace any padding in the original Base64 string
             * - If string argument contains padding, the final Base64 string must
             *   be valid or an exception will be thrown
             * - If string argument does not contain padding, the method will
             *   will automatically append padding, given that str_len % 4 != 1 */
            Base64&             append(const std::string&);

            /* Returns the Binary object equivalent of the Base64 string */
            Binary              to_Bin() const;

            /* Returns the Hexadecimal object equivalent of the Base64 string */
            Hex                 to_Hex() const;


            /*** Operator ***/
            Base64&             operator+=(const Base64&);


        private:
            /*** Underlying Data Structure ***/
            std::string m_b64;


        /*** Printer ***/
        friend std::ostream& operator<<(std::ostream&, const Base64&);
        };
    }
}

#endif /* SEC_B64 */
