#include<iostream>
#include<fstream>

int main() {
    std::ifstream is ("test.gbc", std::ifstream::binary);

    //get length of ROM
    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    //read into buffer
    char * buffer = new char [length];
    std::cout << "Reading " << length << " bytes\n";
    is.read(buffer, length);
    if (is)
      std::cout << "Read ROM\n";
    else
      std::cout << "Error: only " << is.gcount() << " could be read";
    is.close();

    //Get Cartridge Title 
    int i = 0x0134;
    std::cout << "Cartridge Title: ";
    while (i < 0x0142) {
      std::cout << buffer[i];
      i++;
    }
    std::cout << "\n";

    delete[] buffer;
    return 0;
}