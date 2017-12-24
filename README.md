# Jpeg
Windows application which reads a jpeg image file, decodes it, encodes it back into jpeg and writes it to a new file.

Compiled with g++: g++ src/main.cpp -o bin/jpeg.exe -L libjpeg/lib -l jpeg

To use the application run jpeg.exe with parameters: jpeg.exe [inputfile] [outputfile]
- inputfile is the name of the input jpeg image file
- outputfile is the name of the output jpeg image file

for example: jpeg.exe in.jpeg out.jpeg