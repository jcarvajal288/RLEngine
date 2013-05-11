#include "CheckedSave.hpp"

using namespace std;

namespace rlns
{
    namespace utl
    {
       /*-------------------------------------------------------------------------------- 
            Function    : CheckedZip::interpretTypeCode
            Description : Converts an integer code into the appropriate SaveType.
            Inputs      : SaveType
            Outputs     : None
            Return      : string
        --------------------------------------------------------------------------------*/
        string CheckedZip::interpretTypeCode(const int code) const
        {
            switch(code)
            {
                case 0: return "CHAR"; break;
                case 1: return "INT"; break;
                case 2: return "FLOAT"; break;
                case 3: return "STRING"; break;
                case 4: return "COLOR"; break;
                case 5: return "IMAGE"; break;
                case 6: return "CONSOLE"; break;
                case 7: return "DATA"; break;
                default: return "INVALID CODE";
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : CheckedZip::error
            Description : Prints the SaveType it recieved and the SaveType it expected,
                          then quits the program.
            Inputs      : recieved type, expected type
            Outputs     : error message
            Return      : void
        --------------------------------------------------------------------------------*/
        void CheckedZip::error(const int received, const int expected) const
        {
            cerr << "SAVE ERROR: Expected type " << interpretTypeCode(expected) << ", " 
                 << "received type " << interpretTypeCode(received) << endl;
            abort();
        }


        /*
         * All the following functions wrap the equivalent TCODZip functions.  Before the value itself is
         * saved, an enum is saved (as an int) indicating what value is saved.  When the value is
         * loaded, the enum found in the buffer is compared to the enum expected by the loading function.
         * If they are different, it prints an error and aborts the program.
         */


        // Input Functions

        char CheckedZip::getChar()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::CHAR)
            {
                error(result, SaveType::CHAR);
            }
            return TCODZip::getChar();
        }

        int CheckedZip::getInt()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::INT)
            {
                error(result, SaveType::INT);
            }
            return TCODZip::getInt();
        }

        float CheckedZip::getFloat()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::FLOAT)
            {
                error(result, SaveType::FLOAT);
            }
            return TCODZip::getFloat();
        }

        const char* CheckedZip::getString()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::STRING)
            {
                error(result, SaveType::STRING);
            }
            return TCODZip::getString();
        }

        TCODColor CheckedZip::getColor()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::COLOR)
            {
                error(result, SaveType::COLOR);
            }
            return TCODZip::getColor();
        }

        TCODImage* CheckedZip::getImage()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::IMAGE)
            {
                error(result, SaveType::IMAGE);
            }
            return TCODZip::getImage();
        }

        TCODConsole* CheckedZip::getConsole()
        {
            int result = TCODZip::getInt();
            if(result != SaveType::CONSOLE)
            {
                error(result, SaveType::CONSOLE);
            }
            return TCODZip::getConsole();
        }

        int CheckedZip::getData(int nbBytes, void* data)
        {
            int result = TCODZip::getInt();
            if(result != SaveType::DATA)
            {
                error(result, SaveType::DATA);
            }
            return TCODZip::getData(nbBytes, data);
        }


        // Output Functions

        void CheckedZip::putChar(char val)
        {
            TCODZip::putInt(SaveType::CHAR);
            TCODZip::putChar(val);
        }

        void CheckedZip::putInt(int val)
        {
            TCODZip::putInt(SaveType::INT);
            TCODZip::putInt(val);
        }

        void CheckedZip::putFloat(float val)
        {
            TCODZip::putInt(SaveType::FLOAT);
            TCODZip::putFloat(val);
        }

        void CheckedZip::putString(const char* val)
        {
            TCODZip::putInt(SaveType::STRING);
            TCODZip::putString(val);
        }

        void CheckedZip::putColor(const TCODColor* val)
        {
            TCODZip::putInt(SaveType::COLOR);
            TCODZip::putColor(val);
        }

        void CheckedZip::putImage(const TCODImage* val)
        {
            TCODZip::putInt(SaveType::IMAGE);
            TCODZip::putImage(val);
        }

        void CheckedZip::putConsole(const TCODConsole* val)
        {
            TCODZip::putInt(SaveType::CONSOLE);
            TCODZip::putConsole(val);
        }

        void CheckedZip::putData(int nbBytes, const void* data)
        {
            TCODZip::putInt(SaveType::DATA);
            TCODZip::putData(nbBytes, data);
        }
    }
}
