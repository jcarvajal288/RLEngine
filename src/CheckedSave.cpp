#include "CheckedSave.hpp"

using namespace std;

namespace rlns
{
    namespace utl
    {
        /*
         * All the following functions wrap the equivalent TCODZip functions.  Before the value itself is
         * saved, an enum is saved (as an int) indicating what value is saved.  When the value is
         * loaded, the enum found in the buffer is compared to the enum expected by the loading function.
         * If they are different, it prints an error and aborts the program.
         */


        // Input Functions

        char RLNSZip::getChar()
        {
            //cerr << "char" << endl;
            assert(TCODZip::getInt() == SaveType::CHAR);
            return TCODZip::getChar();
        }

        int RLNSZip::getInt()
        {
            //cerr << "int" << endl;
            assert(TCODZip::getInt() == SaveType::INT);
            return TCODZip::getInt();
        }

        float RLNSZip::getFloat()
        {
            //cerr << "float" << endl;
            assert(TCODZip::getInt() == SaveType::FLOAT);
            return TCODZip::getFloat();
        }

        const char* RLNSZip::getString()
        {
            //cerr << "string" << endl;
            assert(TCODZip::getInt() == SaveType::STRING);
            return TCODZip::getString();
        }

        TCODColor RLNSZip::getColor()
        {
            //cerr << "color" << endl;
            assert(TCODZip::getInt() == SaveType::COLOR);
            return TCODZip::getColor();
        }

        TCODImage* RLNSZip::getImage()
        {
            //cerr << "image" << endl;
            assert(TCODZip::getInt() == SaveType::IMAGE);
            return TCODZip::getImage();
        }

        TCODConsole* RLNSZip::getConsole()
        {
            //cerr << "console" << endl;
            assert(TCODZip::getInt() == SaveType::CONSOLE);
            return TCODZip::getConsole();
        }

        int RLNSZip::getData(int nbBytes, void* data)
        {
            //cerr << "data" << endl;
            assert(TCODZip::getInt() == SaveType::DATA);
            return TCODZip::getData(nbBytes, data);
        }


        // Output Functions

        void RLNSZip::putChar(char val)
        {
            //cerr << "char" << endl;
            TCODZip::putInt(SaveType::CHAR);
            TCODZip::putChar(val);
        }

        void RLNSZip::putInt(int val)
        {
            //cerr << "int" << endl;
            TCODZip::putInt(SaveType::INT);
            TCODZip::putInt(val);
        }

        void RLNSZip::putFloat(float val)
        {
            //cerr << "float" << endl;
            TCODZip::putInt(SaveType::FLOAT);
            TCODZip::putFloat(val);
        }

        void RLNSZip::putString(const char* val)
        {
            //cerr << "string" << endl;
            TCODZip::putInt(SaveType::STRING);
            TCODZip::putString(val);
        }

        void RLNSZip::putColor(const TCODColor* val)
        {
            //cerr << "color" << endl;
            TCODZip::putInt(SaveType::COLOR);
            TCODZip::putColor(val);
        }

        void RLNSZip::putImage(const TCODImage* val)
        {
            //cerr << "image" << endl;
            TCODZip::putInt(SaveType::IMAGE);
            TCODZip::putImage(val);
        }

        void RLNSZip::putConsole(const TCODConsole* val)
        {
            //cerr << "console" << endl;
            TCODZip::putInt(SaveType::CONSOLE);
            TCODZip::putConsole(val);
        }

        void RLNSZip::putData(int nbBytes, const void* data)
        {
            //cerr << "data" << endl;
            TCODZip::putInt(SaveType::DATA);
            TCODZip::putData(nbBytes, data);
        }
    }
}
