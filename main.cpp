

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;



class Image {
    struct Header {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
    };

    struct Pixels {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
    };
public:
    Header head;
    vector<Pixels> pixels;
    Pixels pix;

    Image() {

    }

    Image(string fileName) {
        ifstream inFile;
        inFile.open(fileName, ios::binary);

        if (!inFile.is_open()) {
            cout << "File not opened correctly" << endl;
        }

        inFile.read(reinterpret_cast<char*>(&head.idLength), 1);
        inFile.read(reinterpret_cast<char*>(&head.colorMapType), 1);
        inFile.read(reinterpret_cast<char*>(&head.dataTypeCode), 1);
        inFile.read(reinterpret_cast<char*>(&head.colorMapOrigin), 2);
        inFile.read(reinterpret_cast<char*>(&head.colorMapLength), 2);
        inFile.read(reinterpret_cast<char*>(&head.colorMapDepth), 1);
        inFile.read(reinterpret_cast<char*>(&head.xOrigin), 2);
        inFile.read(reinterpret_cast<char*>(&head.yOrigin), 2);
        inFile.read(reinterpret_cast<char*>(&head.width), 2);
        inFile.read(reinterpret_cast<char*>(&head.height), 2);
        inFile.read(reinterpret_cast<char*>(&head.bitsPerPixel), 1);
        inFile.read(reinterpret_cast<char*>(&head.imageDescriptor), 1);

        for (int i = 0; i < (head.width * head.height); i++) {
            Pixels p;
            inFile.read((char*)&p, 3);
            pixels.push_back(p);
        }

        inFile.close();
    }

    void Write(Image& obj, string outputFileName) {
        ofstream outFile(outputFileName, ios::binary);

        outFile.write((char*)(&head.idLength), 1);
        outFile.write((char*)(&head.colorMapType), 1);
        outFile.write((char*)(&head.dataTypeCode), 1);
        outFile.write((char*)(&head.colorMapOrigin), 2);
        outFile.write((char*)(&head.colorMapLength), 2);
        outFile.write((char*)(&head.colorMapDepth), 1);
        outFile.write((char*)(&head.xOrigin), 2);
        outFile.write((char*)(&head.yOrigin), 2);
        outFile.write((char*)(&head.width), 2);
        outFile.write((char*)(&head.height), 2);
        outFile.write((char*)(&head.bitsPerPixel), 1);
        outFile.write((char*)(&head.imageDescriptor), 1);

        for (int i = 0; i < (head.width * head.height); i++) {
            outFile.write(((char*)&pixels.at(i)), 3);
        }
        outFile.close();
    }

};

Image Multiply(Image& obj1, Image& obj2) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        float blue = (float)obj1.pixels[i].blue * (float)obj2.pixels[i].blue;
        float green = (float)obj1.pixels[i].green * (float)obj2.pixels[i].green;
        float red = (float)obj1.pixels[i].red * (float)obj2.pixels[i].red;

        blue = (blue / 255.0f) + 0.5f;
        green = (green / 255.0f) + 0.5f;
        red = (red / 255.0f) + 0.5f;

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image Subtract(Image& obj1, Image& obj2) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        float blue = (float)obj2.pixels[i].blue - (float)obj1.pixels[i].blue;
        float green = (float)obj2.pixels[i].green - (float)obj1.pixels[i].green;
        float red = (float)obj2.pixels[i].red - (float)obj1.pixels[i].red;

        if (blue < 0) {
            blue = 0;
        }
        if (green < 0) {
            green = 0;
        }
        if (red < 0) {
            red = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image Screen(Image& obj1, Image& obj2) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        float blue1 = (float)obj1.pixels[i].blue;
        float blue2 = (float)obj2.pixels[i].blue;
        float green1 = (float)obj1.pixels[i].green;
        float green2 = (float)obj2.pixels[i].green;
        float red1 = (float)obj1.pixels[i].red;
        float red2 = (float)obj2.pixels[i].red;

        blue1 = (blue1 / 255.0f);
        blue2 = (blue2 / 255.0f);
        green1 = (green1 / 255.0f);
        green2 = (green2 / 255.0f);
        red1 = (red1 / 255.0f);
        red2 = (red2 / 255.0f);

        float pixB = (1 - ((1 - blue1) * (1 - blue2))) * 255;
        float pixG = (1 - ((1 - green1) * (1 - green2))) * 255;
        float pixR = (1 - ((1 - red1) * (1 - red2))) * 255;

        pixB += 0.5f;
        pixG += 0.5f;
        pixR += 0.5f;

        imR.pix.blue = (unsigned char)(pixB);
        imR.pix.green = (unsigned char)(pixG);
        imR.pix.red = (unsigned char)(pixR);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image Overlay(Image& obj1, Image& obj2) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        float blue1 = (float)obj1.pixels[i].blue;
        float blue2 = (float)obj2.pixels[i].blue;
        float green1 = (float)obj1.pixels[i].green;
        float green2 = (float)obj2.pixels[i].green;
        float red1 = (float)obj1.pixels[i].red;
        float red2 = (float)obj2.pixels[i].red;

        blue1 = (blue1 / 255.0f);
        blue2 = (blue2 / 255.0f);
        green1 = (green1 / 255.0f);
        green2 = (green2 / 255.0f);
        red1 = (red1 / 255.0f);
        red2 = (red2 / 255.0f);

        float pixB;
        float pixG;
        float pixR;

        if (blue2 <= 0.5f) {
            pixB = 2 * blue1 * blue2;
        }
        else {
            pixB = 1 - (2 * (1 - blue1) * (1 - blue2));
        }

        if (green2 <= 0.5f) {
            pixG = 2 * green1 * green2;
        }
        else {
            pixG = 1 - (2 * (1 - green1) * (1 - green2));
        }

        if (red2 <= 0.5f) {
            pixR = 2 * red1 * red2;
        }
        else {
            pixR = 1 - (2 * (1 - red1) * (1 - red2));
        }

        pixB = (pixB * 255) + 0.5f;
        pixG = (pixG * 255) + 0.5f;
        pixR = (pixR * 255) + 0.5f;

        imR.pix.blue = (unsigned char)(pixB);
        imR.pix.green = (unsigned char)(pixG);
        imR.pix.red = (unsigned char)(pixR);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image AddGreen(Image& obj1, int val) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj1.pixels[i].green + val;
        int red = (int)obj1.pixels[i].red;

        if (green > 255) {
            green = 255;
        }
        else if (green < 0) {
            green = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image AddRed(Image& obj1, int val) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj1.pixels[i].green;
        int red = (int)obj1.pixels[i].red + val;

        if (red > 255) {
            red = 255;
        }
        else if (red < 0) {
            red = 0;
        }
        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image AddBlue(Image& obj1, int val) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue + val;
        int green = (int)obj1.pixels[i].green;
        int red = (int)obj1.pixels[i].red;

        if (blue > 255) {
            blue = 255;
        }
        else if (blue < 0) {
            blue = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image ScaleR(Image& obj1, int scale1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj1.pixels[i].green;
        int red = (int)obj1.pixels[i].red * scale1;

        if (red > 255) {
            red = 255;
        }
        else if (red < 0) {
            red = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image ScaleG(Image& obj1, int scale1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj1.pixels[i].green * scale1;
        int red = (int)obj1.pixels[i].red;

        if (green > 255) {
            green = 255;
        }
        else if (green < 0) {
            blue = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image ScaleB(Image& obj1, int scale1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue * scale1;
        int green = (int)obj1.pixels[i].green;
        int red = (int)obj1.pixels[i].red;

        if (blue > 255) {
            blue = 255;
        }
        else if (blue < 0) {
            blue = 0;
        }

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image SepBlue(Image& obj1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(blue);
        imR.pix.red = (unsigned char)(blue);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image SepGreen(Image& obj1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int green = (int)obj1.pixels[i].green;

        imR.pix.blue = (unsigned char)(green);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(green);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image SepRed(Image& obj1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int red = (int)obj1.pixels[i].red;

        imR.pix.blue = (unsigned char)(red);
        imR.pix.green = (unsigned char)(red);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image Combine(Image& obj1, Image& obj2, Image& obj3) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj2.pixels[i].green;
        int red = (int)obj3.pixels[i].red;

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.push_back(imR.pix);
    }
    return imR;
}

Image Flipped(Image& obj1) {
    Image imR;
    imR.head = obj1.head;
    imR.pixels.clear();

    int countingVal = imR.head.width * imR.head.height;
    for (int i = 0; i < countingVal; i++) {
        int blue = (int)obj1.pixels[i].blue;
        int green = (int)obj1.pixels[i].green;
        int red = (int)obj1.pixels[i].red;

        imR.pix.blue = (unsigned char)(blue);
        imR.pix.green = (unsigned char)(green);
        imR.pix.red = (unsigned char)(red);

        imR.pixels.insert(imR.pixels.begin(), imR.pix);
    }
    return imR;
}

bool TestFileNameTGA(string fileName) {
    if (fileName.length() < 4) {
        return false;
    }
    string tgaTest;
    for (int i = fileName.length() - 4; i < (fileName.length()); i++) {
        tgaTest += fileName[i];
    }
    if (tgaTest != ".tga") {
        return false;
    }
    return true;
}

bool TestFileNameIO(string fileName) {
    fstream inFile(fileName);
    if (!inFile.is_open()) {
        return false;
    }
    return true;
}


//argc = 3 is the first argument 
//terminal call:     .\x64\Debug\bennett.sam.project2.exe out.tga car.tga scaleblue 3 combine layer1.tga text.tga addgreen 200
//out.tga input / car.tga nonexistent

//part 4: must be out.tga layer2.tga multiply circles.tga
//                out.tga pattern2.tga subtract out.tga
//part 9: must be layer_blue.tga combine layer_green.tga layer_red.tga



//while(!Filename.eof())




int main(int argc, char* argv[]) {
    //if not even any arguments 
    if (argc < 2) {
        cout << "Project 2: Image Processing, Spring 2023" << endl << endl;
        cout << "Usage:" << endl;
        cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }

    string arg_1 = (string)argv[1];

    if (arg_1 == "--help") {
        cout << "Project 2: Image Processing, Spring 2023" << endl << endl;
        cout << "Usage:" << endl;
        cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }

    int curPos = 3;
    string arg_2 = (string)argv[2]; 

    // if not given source file name (2nd argument)
    if (argc == 2) {
        cout << "Invalid file name." << endl;
        return 0;
    }
    //if not given menthod name (3rd argument)
    else if (argc == 3) {
        if (!TestFileNameTGA(arg_2) == 1) {
            cout << "Invalid file name." << endl;
            //cout << "output: " << TestFileNameTGA(arg_1) << endl;
            return 0;
        }
        else if (!TestFileNameIO(arg_1) == true) {
            cout << "File does not exist." << endl;
            return 0;
        }
        else {
            cout << "Invalid method name." << endl;
            return 0;
        }
        
    }

    if (!TestFileNameTGA(arg_1) == 1) {
        cout << "Invalid file name." << endl;
        //cout << "output: " << TestFileNameTGA(arg_1) << endl;
        return 0;
    }
    
    //string arg_2 = (string)argv[2];
    if (!TestFileNameTGA(arg_2) == 1) {
        cout << "Invalid file name." << endl;
        return 0;
    }
    else if (!TestFileNameIO(arg_2) == true) {
        cout << "File does not exist." << endl;
        return 0;
    }
    Image currImage((string)argv[2]);

    while (curPos < argc) {
        string curr_Arg = (string)argv[curPos];
        //cout << "currArg: " << currArg << endl;


        if (curr_Arg == "multiply") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Missing argument." << endl;
                }
                curPos++;
                string curr_F2 = argv[curPos];

                //cout << "currFile2: " << curr_F2 << endl;

                if (!TestFileNameTGA(curr_F2) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F2) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                Image other(curr_F2);
                currImage = Multiply(currImage, other);
                cout << "multiply working..." << endl;
            }
            catch (const out_of_range& ex) {
                cout << "Missing argument." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "subtract") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Missing argument." << endl;
                }
                curPos++;
                string curr_F2 = (string)argv[curPos];

                if (!TestFileNameTGA(curr_F2) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F2) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                Image other(curr_F2);
                currImage = Subtract(other, currImage);
                cout << "subtract working..." << endl;
            }
            catch (...) {
                cout << "Missing argument." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "overlay") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Missing argument." << endl;
                }
                curPos++;
                string curr_F2 = argv[curPos];
                if (!TestFileNameTGA(curr_F2) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F2) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                Image other(curr_F2);
                currImage = Overlay(currImage, other);
                cout << "overlay working..." << endl;
            }
            catch (...) {
                cout << "Missing argument." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "screen") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Missing argument." << endl;
                }
                curPos++;
                string curr_F2 = argv[curPos];
                if (!TestFileNameTGA(curr_F2) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F2) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                Image other(curr_F2);
                currImage = Screen(currImage, other);
                cout << "screen working..." << endl;
            }
            catch (...) {
                cout << "Missing argument." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "combine") {
            //curPos++;
            try {
                if (argc < 6) {
                    cout << "Missing argument." << endl;
                }
                curPos++;
                string curr_F2 = argv[curPos];
                if (!TestFileNameTGA(curr_F2) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F2) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                curPos++;
                string curr_F3 = argv[curPos];
                if (!TestFileNameTGA(curr_F3) == true) {
                    cout << "Invalid argument, invalid file name." << endl;
                    return 0;
                }
                else if (!TestFileNameIO(curr_F3) == true) {
                    cout << "Invalid argument, file does not exist." << endl;
                    return 0;
                }
                Image other1(curr_F2);
                Image other2(curr_F3);
                currImage = Combine(other2, other1, currImage);
                cout << "combine working..." << endl;
            }
            catch (...) {
                cout << "Missing argument." << endl;
                return 0;
            }       
            curPos ++;
            continue;
        }
        else if (curr_Arg == "flip") {
            currImage = Flipped(currImage);
            //cout << "flip working..." << endl;
            curPos++;
            continue;
        }
        else if (curr_Arg == "onlyred") {
            currImage = SepRed(currImage);
            cout << "onlyred working..." << endl;
            curPos++;
            continue;
        }
        else if (curr_Arg == "onlygreen") {
            currImage = SepGreen(currImage);
            cout << "onlygreen working..." << endl;
            curPos++;
            continue;
        }
        else if (curr_Arg == "onlyblue") {
            currImage = SepBlue(currImage);
            cout << "onlyblue working..." << endl;
            curPos++;
            continue;
        }
        else if (curr_Arg == "addred") {
            curPos++;
            try {
                string val = (string)argv[curPos];
                int addVal = stoi(val);
                currImage = AddRed(currImage, addVal);
                cout << "addred working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "addgreen") {
            curPos++;
            try {
                string val = (string)argv[curPos];
                int addVal = stoi(val);
                currImage = AddGreen(currImage, addVal);
                cout << "addgreen working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "addblue") {
            try {
                curPos++;

                string val = (string)argv[curPos];

                int addVal = stoi(val);
                currImage = AddBlue(currImage, addVal);
                cout << "addblue working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "scalered") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Invalid argument, expected number." << endl;
                }

                curPos++;
                string val = (string)argv[curPos];

                //cout << "val: " << val << endl;

                int addVal = stoi(val);
                currImage = ScaleR(currImage, addVal);
                cout << "scalered working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "scalegreen") {
            //curPos++;
            try {
                if (argc < 5) {
                    cout << "Invalid argument, expected number." << endl;
                }
                curPos++;
                string val = (string)argv[curPos];
                int addVal = stoi(val);
                currImage = ScaleG(currImage, addVal);
                cout << "scalegreen working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else if (curr_Arg == "scaleblue") {
            try {
                if (argc < 5) {
                    cout << "Missing argument." << endl;
//Invalid argument, expected number.
                    return 0;
                }
                if (argc == 5) {
                    cout << "Missing argument." << endl;
                    return 0;
                }
                curPos++;
                string val = (string)argv[curPos];
                int addVal = stoi(val);
                currImage = ScaleB(currImage, addVal);
                cout << "scaleblue working..." << endl;
            }
            catch (...) {
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
            curPos++;
            continue;
        }
        else {
            cout << "Invalid method name." << endl;
            //cout << "currArg: " << curr_Arg << endl;
            return 0; 
        }
    }

    //make currImage as tga file (arg1 = output file name)
    currImage.Write(currImage, arg_1);





}