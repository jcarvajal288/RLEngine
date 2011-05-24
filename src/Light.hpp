#ifndef RLNS_LIGHTSOURCE_HPP
#define RLNS_LIGHTSOURCE_HPP

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "CheckedSave.hpp"
#include "FileParser.hpp"
#include "Point.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Light
            Description : Object that holds information on a light source.  Includes
                          position on the map, intensity, and color. (possibly flicker
                          method using TCODNoise?)
            Parents     : None
            Children    : None
            Friends     : LightParser
        --------------------------------------------------------------------------------*/
        class Light
        {
            // Member Variables
            private:
                std::string name;
                int id; // uniquely identifies this light
                utl::Point position;
                float intensity;
                TCODColor color;
                float flickerRate;
                float flickerDelta;
                TCOD_noise_type_t noiseType; // keeps track of the noise type for save game purposes
                TCODNoisePtr noise;

                static int nextID;

                // list of standard light sources read in from a data file
                static std::vector<LightPtr> stdLights;

                // map of all currently existing lights, using the light ID as key
                static std::map<int, LightPtr> lights;


            // Member Functions
            private:
                static int findLightIndex(const std::string&);
                static int  getNextID() { return nextID++; }
                void setPosition(const utl::Point& p) { position = p;  }

            public:
                Light(const char* n, const float i, const TCODColor& c, const float fr=0.0f, 
                      const float fd=0.0f, const TCOD_noise_type_t t=TCOD_NOISE_DEFAULT)
                : name(n), id(getNextID()), position(0,0), intensity(i), color(c),
                  flickerRate(fr), flickerDelta(fd), noiseType(t), noise(new TCODNoise(1, noiseType)) {}

                Light(const utl::Point l, const float i, const TCODColor c, const float fr=0.0f, 
                      const float fd=0.0f, const TCOD_noise_type_t t=TCOD_NOISE_DEFAULT)
                : name(""), id(getNextID()), position(l), intensity(i), color(c),
                  flickerRate(fr), flickerDelta(fd), noiseType(t), noise(new TCODNoise(1, noiseType)) {}

                Light(utl::RLNSZip&); // used for loading saved games

                std::string getName()    const { return name;      }
                utl::Point getPosition() const { return position;  }
                float getIntensity()     const { return intensity; }
                float getFlickerRate()   const { return flickerRate; }
                float getFlickerDelta()  const { return flickerDelta; }
                TCODColor getColor()     const { return color; }
                int getID()              const { return id; }
                float getNoise(float n) { return noise->get(&n); }

                static void setNextID(const int a) { nextID = a; } // TODO: make this use the max of lights' keys
                                                                   //       This function should not exist!

                static void moveLight(const int, const utl::Point&);
                static int createLight(const std::string&);

                static LightPtr findLight(const std::string&);

                static LightPtr getLight(const int i); 

                void saveToDisk(utl::RLNSZip&) const;

                static void saveLights(utl::RLNSZip&);
                static void loadLights(utl::RLNSZip&);

                friend class LightParser;
        };


        // inline functions

        inline void Light::moveLight(const int lightID, const utl::Point& pt)
        {
            lights[lightID]->setPosition(pt);
        }



        /*--------------------------------------------------------------------------------
            Class       : LightParser
            Description : Reads in the standard light sources from lightsource.txt
            Parents     : FileParser
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class LightParser: public FileParser
        {
            // Member Variables
            private:
                /*--------------------------------------------------------------------------------
                    Class       : LightListener
                    Description : Manages constructing the standard light sources from the file
                                  data.
                    Parents     : ITCODParserListener (see libtcod file parser documentation)
                    Children    : None
                    Friends     : None
                --------------------------------------------------------------------------------*/
                class LightListener: public ITCODParserListener
                {
                    // Member Variables
                    private:
                        const char* lightName;
                        float intensity;
                        TCODColor color;
                        float flickerRate;
                        float flickerDelta;
                        TCOD_noise_type_t noiseType;

                    // Member Functions
                    public:
                        bool parserNewStruct(TCODParser*, const TCODParserStruct*, const char*);
                        bool parserFlag(TCODParser*, const char*);
                        bool parserProperty(TCODParser*, const char*, TCOD_value_type_t, TCOD_value_t);
                        bool parserEndStruct(TCODParser*, const TCODParserStruct*, const char*);
                        void error(const char*);
                } listener;

            // Member Functions
            private:
                void verify();
                
            protected:
                void defineSyntax();

            public:
                LightParser(const std::string& n)
                : FileParser(n) {}

                void run();
        };
    }
}

#endif
