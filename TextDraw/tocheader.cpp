#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <png++/png.hpp>

int main(int argc, char** args) {
    if(argc!=5) {
        std::cout << "Usage: " << args[0] << " [width] [height] [character count] [input png file]\n";
        exit(0);
    }
    unsigned int width  = atoi(args[1]);
    unsigned int height = atoi(args[2]);
    unsigned int CHARACTER_COUNT = atoi(args[3]);
    if(CHARACTER_COUNT > 224) {
        std::cout << "Character count is too high.\n";
        return 2;
    }
    if(height < 1) {
        std::cout << "Height is too low.\n";
        return 2;
    }
    //-1 because last line is always empty.
    height--;
    //Make the variable name for the C header file and the output file name.
    char varname[30];
    char filename[30];
    unsigned char filenamePos=0;
    const char* fnst = "font_";
    for(unsigned char i=0;i<strlen(fnst);filenamePos++,i++) {filename[filenamePos]=fnst[i]; varname[filenamePos]=fnst[i];}
    for(unsigned char i=0;i<strlen(args[1]);filenamePos++,i++) {filename[filenamePos]=args[1][i]; varname[filenamePos]=args[1][i];}
    filename[filenamePos]='x'; varname[filenamePos++]='x';
    for(unsigned char i=0;i<strlen(args[2]);filenamePos++,i++) {filename[filenamePos]=args[2][i]; varname[filenamePos]=args[2][i];}
    varname[filenamePos]='\0';
    const char* fnend = ".h";
    for(unsigned char i=0;i<strlen(fnend);filenamePos++,i++) {filename[filenamePos]=fnend[i];}
    filename[filenamePos++]='\0';
    
    //Read image and convert to C array data.
    png::image<png::rgb_pixel> image(args[4]);
    //Check if the image size is valid for this font size.
    if(image.get_width()!=width || image.get_height()!=height*CHARACTER_COUNT+CHARACTER_COUNT) {
        std::cout << "Invalid image size.\n";
        return 2;
    }
    std::ofstream ofile(filename,std::ios::out|std::ios::trunc);
    if(ofile.is_open()) {
        
        ofile << "const unsigned char " << varname << " [] = {\n";
        uint16_t totaly=0;
        const uint8_t bLookupTable[8] = {128,64,32,16,8,4,2,1};
        for(unsigned char c=0;c<CHARACTER_COUNT;c++) {
            char tmpchar=c+32;
            //Add comment with current character number in the ASCII table.
            ofile << "// 0x" << std::hex << (int)tmpchar << std::dec << "\n";
            for(uint16_t y=0;y<height;y++) {
                bool bits[width];
                uint8_t nbytes = ceil((double)width/8);
                uint8_t bytes[nbytes];
                for(uint8_t b=0; b<nbytes; b++) bytes[b] = 0;
                for(unsigned char x=0;x<width;x++) {
                    if(image[totaly][x].red>0) {
                        bits[x] = 1;
                        //Add to calculated byte
                        bytes[x/8] += bLookupTable[x%8];
                    }
                    else {bits[x] = 0;}
                }
                //Write converted hex numbers
                for(uint8_t b=0; b<nbytes; b++) {
                    ofile << "0x" << std::hex << (int)bytes[b] << (bytes[b] < 0x10 ? " " : "") << ", " << std::dec;
                }
                //Write binary bits as comment
                ofile << "// ";
                for(uint8_t b=0; b<width; b++) {
                    ofile << (int)bits[b];
                }
                
                ofile << "\n";
                totaly++;
            }
            totaly++;
            ofile << "";
        }
        ofile << "};\n";
        
        ofile << "TextDisplay_CharSet td_" << varname << " = (TextDisplay_CharSet){\n"
        << "" << width << ",\n"
        << "" << height+1 << ",\n"
        //First character
        << 32 << ",\n"
        //Character count
        << CHARACTER_COUNT << ",\n"
        //Font name string
        << "\"" << varname << "\",\n"
        //Pointer to actual font data
        << varname
        << "};\n";
        
        ofile.close();
    } else {std::cout << "Unable to open output file.\n"; exit(255);}
    return 0;
}
