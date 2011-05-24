#include "Light.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        int Light::nextID = 0;
        vector<LightPtr> Light::stdLights;
        map<int, LightPtr> Light::lights;

        /*--------------------------------------------------------------------------------
            Function    : Light::findLightIndex
            Description : Given the name of a light, this searches the stdLights list for
                          a light with that name and returns its index in the list.
                          Helper function for findLight().
            Inputs      : light name
            Outputs     : None
            Return      : int
        --------------------------------------------------------------------------------*/
        int Light::findLightIndex(const string& lightName)
        {
            int end = Light::stdLights.size();
            for(int i=0; i < end; ++i)
            {
                if(Light::stdLights[i]->name == lightName)
                    return i;
            }
            // light name not found
            return -1;
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::Light(RLNSZip&)
            Description : Creates a light given the information in the given RLNSZip save
                          buffer.  Used when loading saved games.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        Light::Light(RLNSZip& zip)
        : id(zip.getInt()),
          position(zip),
          intensity(zip.getFloat()),
          color(zip.getColor()),
          flickerRate(zip.getFloat()),
          flickerDelta(zip.getFloat()),
          noiseType(static_cast<TCOD_noise_type_t>(zip.getInt())),
          noise(new TCODNoise(1, noiseType)) {}



        /*--------------------------------------------------------------------------------
            Function    : Light::createLight
            Description : Given a name of a standard light, this function creates a light
                          of that name, adds it to the lights vector, and returns that
                          light's ID.
            Inputs      : name of the light to make
            Outputs     : None
            Return      : int
        --------------------------------------------------------------------------------*/
        int Light::createLight(const string& lightName)
        {
            int id = getNextID();
            LightPtr newLight(new Light(*findLight(lightName)));
            Light::lights.insert(pair<int, LightPtr>(id, newLight));
            return id;
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::findLight
            Description : Returns the light with the given name.
            Inputs      : light name
            Outputs     : None
            Return      : LightPtr
        --------------------------------------------------------------------------------*/
        LightPtr Light::findLight(const string& lightName)
        {
            int i = findLightIndex(lightName);
            if(i < 0) utl::fatalError("Light " + lightName + " not found!");
            LightPtr result(Light::stdLights.at(i));
            return result;
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::getLight
            Description : This function returns the light associated with the given 
                          light ID.
            Inputs      : light ID
            Outputs     : None (possible error message)
            Return      : LightPtr
        --------------------------------------------------------------------------------*/
        LightPtr Light::getLight(const int lightID) 
        { 
            std::map<int, LightPtr>::iterator it = Light::lights.find(lightID); 
            if(it == lights.end())
                utl::fatalError("Unknown Light ID.");
            return it->second;
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::saveToDisk
            Description : Saves the Light to the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Light::saveToDisk(RLNSZip& zip) const
        {
            zip.putInt(id);
            position.saveToDisk(zip);
            zip.putFloat(intensity);
            zip.putColor(&color);
            zip.putFloat(flickerRate);
            zip.putFloat(flickerDelta);
            zip.putInt(static_cast<int>(noiseType));
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::saveLights
            Description : Saves the Light::lights vector to the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Light::saveLights(RLNSZip& zip)
        {
            // save nextID
            zip.putInt(nextID);
            
            // save the lights vector
            map<int, LightPtr>::const_iterator it, end;
            end = lights.end();
            zip.putInt(lights.size());
            for(it=lights.begin(); it!=end; ++it)
            {
                zip.putInt(it->first);
                it->second->saveToDisk(zip);
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Light::loadLights
            Description : Loads the Light::lights vector from the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Light::loadLights(RLNSZip& zip)
        {
            // load nextID
            nextID = zip.getInt();

            // load lights vector
            int numLights = zip.getInt();
            for(int i=0; i<numLights; ++i)
            {
                int id = zip.getInt();
                LightPtr light(new Light(zip));
                lights.insert(pair<int, LightPtr>(id, light));
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::LightListener::parserNewStruct
            Description : Called when the parser finds a new Light struct.  This
                          function doesn't do any data reading; it just resets the flag
                          tracking variables.
            Inputs      : parser, parser struct, name of the struct
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool LightParser::LightListener::parserNewStruct(TCODParser* parser, const TCODParserStruct* str, const char* name)
        {
            // unused parameters
            (void) parser;
            (void) str;

            lightName = name;
            return true; 
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::LightListener::parserFlag
            Description : Called when the parser finds a flag, this updates the relevant
                          tracking variable in LightListener.
            Inputs      : parser, flag name
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool LightParser::LightListener::parserFlag(TCODParser* parser, const char* name)
        {
            // unused parameters
            (void) parser;
            (void) name;

            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::LightListener::parserProperty
            Description : Called when the parser finds a property, this updates the
                          relevant tracking variable in LightListener.
            Inputs      : parser, property name, value type, value data
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool LightParser::LightListener::parserProperty
        (TCODParser* parser, const char* name, TCOD_value_type_t valtype, TCOD_value_t value)
        {
            // unused parameters
            (void) valtype;

            if(strcmp(name, "lightColor") == 0)
            {
                color = value.col;
            }
            else if(strcmp(name, "intensity") == 0)
            {
                intensity = value.f;
            }
            else if(strcmp(name, "flickerRate") == 0)
            {
                flickerRate = value.f;
            }
            else if(strcmp(name, "flickerDelta") == 0)
            {
                flickerDelta = value.f;
            }
            else if(strcmp(name, "noiseType") == 0)
            {
                if(strcmp(value.s, "Simplex") == 0)
                    noiseType = TCOD_NOISE_SIMPLEX;
                else if(strcmp(value.s, "Perlin") == 0)
                    noiseType = TCOD_NOISE_PERLIN;
                else
                    noiseType = TCOD_NOISE_WAVELET;
            }
            else
            {
                parser->error("Unknown property '%s' in struct '%s'", name, lightName);
            }
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::LightListener::parserEndStruct
            Description : Called when the parser comes to the end of a terrain structure,
                          this callback creates a new Light object out of the previously
                          read data and adds it to the Light list.  If a Light entry
                          has multiple characters in its character set, then a separate
                          entry is created for each variant.
            Inputs      : parser, terrain struct, name of the struct
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool LightParser::LightListener::parserEndStruct
        (TCODParser* parser, const TCODParserStruct* str, const char* name)
        {
            // unused paramters
            (void) parser;
            (void) str;

            LightPtr newLight(new Light(name, intensity, color, flickerRate, flickerDelta, noiseType));
            Light::stdLights.push_back(newLight);
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::LightListener::error
            Description : Called when the parser detects an error.  Prints the given error
                          message.  If this function is called, it will terminate the
                          program.
            Inputs      : error message
            Outputs     : error message
            Return      : void
        --------------------------------------------------------------------------------*/
        void LightParser::LightListener::error(const char* error)
        {
            cerr << "LIGHT PARSING ERROR: " << error << endl;
            abort();
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::verify
            Description : Testing function that prints out the contents of the Light
                          list to verify that everything was read correctly.
            Inputs      : None
            Outputs     : contents of Light list
            Return      : void
        --------------------------------------------------------------------------------*/
        void LightParser::verify()
        {
            vector<LightPtr>::iterator it, end;
            end = Light::stdLights.end();
            for(it = Light::stdLights.begin(); it != end; ++it)
            {
                cout << (*it)->getName() << endl;
                cout << "intensity: " << (*it)->getIntensity() << endl;
                cout << "color: " << (*it)->getColor().r << "," << (*it)->getColor().g << "," << (*it)->getColor().b << endl;
                cout << "flickerRate: " << (*it)->getFlickerRate() << endl;
                cout << "FlickerDelta: " << (*it)->getFlickerDelta() << endl;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::defineSyntax
            Description : defines the syntax for the terrain config file.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LightParser::defineSyntax()
        {
            const char* noiseTypes[] = 
            {
                "Simplex", "Perlin", "Wavelet", NULL
            };

            // define Light structure
            TCODParserStruct LightParser = *(parser.newStructure("Light"));
            LightParser.addProperty("intensity", TCOD_TYPE_FLOAT, true);
            LightParser.addProperty("lightColor", TCOD_TYPE_COLOR, true);
            LightParser.addProperty("flickerRate", TCOD_TYPE_FLOAT, true);
            LightParser.addProperty("flickerDelta", TCOD_TYPE_FLOAT, true);
            LightParser.addValueList("noiseType", noiseTypes, true);
        }



        /*--------------------------------------------------------------------------------
            Function    : LightParser::run
            Description : Entry point into the Light Parser.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LightParser::run()
        {
            defineSyntax();
            parser.run(filename.c_str(), &listener);
            //verify();
        }
    }
}
