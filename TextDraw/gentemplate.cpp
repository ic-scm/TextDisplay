#include <iostream>
#include <fstream>
#include <cstring>
#include <png++/png.hpp>

int main(int argc, char** args) {
    if(argc!=4) {
        std::cout << "Usage: " << args[0] << " [width] [height] [character count]\n";
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
    //We are making a template to draw CHARACTER_COUNT characters.
    //Adding more vertical pixels for the visible separation between characters.
    unsigned int imgheight = height*CHARACTER_COUNT+CHARACTER_COUNT;
    //Make the file name for the template file.
    char filename[30];
    unsigned char filenamePos=0;
    for(unsigned char i=0;i<strlen(args[1]);filenamePos++,i++) {filename[filenamePos]=args[1][i];}
    filename[filenamePos++]='x';
    for(unsigned char i=0;i<strlen(args[2]);filenamePos++,i++) {filename[filenamePos]=args[2][i];}
    filename[filenamePos++]='-';
    for(unsigned char i=0;i<strlen(args[3]);filenamePos++,i++) {filename[filenamePos]=args[3][i];}
    const char* fnend = "-template.png";
    for(unsigned char i=0;i<strlen(fnend);filenamePos++,i++) {filename[filenamePos]=fnend[i];}
    filename[filenamePos++]='\0';
    
    //Create image.
    png::image<png::rgb_pixel> image(width,imgheight);
    unsigned int totaly=0;
    for(unsigned int c=0;c<CHARACTER_COUNT;c++) {
        for(unsigned int y=0;y<height+1;y++) {
            for(unsigned int x=0;x<width;x++) {
                if(y<height) {image[totaly][x] = png::rgb_pixel(0,0,0);}
                else {image[totaly][x] = png::rgb_pixel(0,255,0);}
            }
            totaly++;
        }
    }
    image.write(filename);
}
