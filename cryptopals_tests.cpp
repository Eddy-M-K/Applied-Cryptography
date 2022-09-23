#include <iostream>
#include <tuple>

#include "kim_sec.hpp"

int main()
{
    // 1.01
    std::cout << kim::sec::Hex{"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"}.to_B64() << std::endl << std::endl;

    // 1.02
    std::cout << kim::sec::XOR<kim::sec::Hex, kim::sec::Hex>(kim::sec::Hex{"1c0111001f010100061a024b53535009181c"},
                                                             kim::sec::Hex{"686974207468652062756c6c277320657965"}).to_Hex() << std::endl << std::endl;

    // 1.03
    std::tuple<std::size_t, kim::sec::Hex, kim::sec::Binary, std::string> ret{kim::sec::XOR_byte_dec<kim::sec::Hex>(kim::sec::Hex{"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"})};
    std::cout << std::get<0>(ret) << "\t" << std::get<1>(ret) << "\t" << std::get<2>(ret) << "\t" << std::get<3>(ret) << std::endl << std::endl;

    // 1.04
    std::cout << "Score\tCiphertext\t\t\t\t\t\t\tKey\t\tPlain text" << std::endl << "---------------------------------------------------------------------------------------------------------------------------" << std::endl;
    for (auto& e : kim::sec::XOR_byte_dec<kim::sec::Hex>(std::ifstream("4.txt"))) {
        std::cout << std::get<0>(e) << "\t" << std::get<1>(e) << "\t" << std::get<2>(e) << "\t" << std::get<3>(e) << std::endl;
    }
    std::cout << std::endl;

    // 1.05
    std::cout << kim::sec::XOR_rep_key_enc<kim::sec::Hex>(std::ifstream("5.txt"), kim::sec::Binary{"ICE"}) << std::endl << std::endl;

    return 0;
}
