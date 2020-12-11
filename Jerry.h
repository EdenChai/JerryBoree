#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"
#include "HashTable.h"

typedef struct Planet_t // defining Planet object
{
	char* _planetName;
	double _x;
	double _y;
	double _z;
} Planet;


typedef struct Origin_t	// defining Origin object
{
	 char* _dimension;
	 Planet* _planet;
} Origin;


typedef struct PhysicalCharacteristics_t // defining Physical Characteristics object
{
	char* _physicalName;
	double _physicalValue;
} PhysicalCharacteristics;

typedef struct Jerry_t // defining Jerry object
{
	char* _jerryID;
	int _happinessLevel;
	Origin* _jerryOrigin;
	PhysicalCharacteristics** _jerryPC;
	int _jerryPcCounter;
} Jerry;

/* object creation functions */
Planet* createPlanet(char* planetName, double x, double y, double z);
Origin* createOrigin(char* dimension, Planet* planet);
PhysicalCharacteristics* createPhysicalCharacteristic(char* physicalName, double physicalValue);
Jerry* createJerry(char* id, int happinessLevel, Origin* origin);

bool checkPhysicalCharacteristics(Jerry* jerry, char* physicalName);
status addPhysicalCharacteristics(Jerry* jerry, PhysicalCharacteristics* physical);
status removePhysicalCharacteristics(Jerry* jerry, char* physicalName);

/* object printing functions */
status printJerry(Element element);
status printPlanet(Planet* planet);
double roundTwoDigit(double number);

/* object destruction functions */
status destroyPlanet(Planet* planet);
status destroyOrigin(Origin* origin);
status destroyPhysicalCharacteristic(PhysicalCharacteristics* pc);
status destroyJerry(Element element);

/* implementation of copy functions */
Element copyJerry(Element element);
//Element shallowCopyJerry(Element element);
Element copyID(Element element);
Element copyPC(Element element);

/* implementation of free functions */
status freeJerry(Element element);
status freeID(Element element);
status freePC(Element element);

/* implementation of equal functions */
bool compareID(Element e1, Element e2);
bool compareJerries(Element e1, Element e2);
bool comparePC(Element e1, Element e2);

/* implementation of print functions */
status printID(Element element);
status printPC(Element element);

int transformFunc(Element element);

#endif /* JERRY_H */