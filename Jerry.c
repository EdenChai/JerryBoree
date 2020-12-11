#include "Jerry.h"

/* --------------- Creation Functions --------------- */

/* initialize a new planet object */
Planet* createPlanet(char* planetName, double x, double y, double z)
{
	if (planetName == NULL) return NULL;

	Planet* planet = (Planet*) malloc(sizeof(Planet));
	if(planet == NULL) return NULL;

	planet->_planetName = (char*) malloc(strlen(planetName) + 1);
	if (planet->_planetName == NULL) return NULL;

	strcpy(planet->_planetName, planetName);
	planet->_x = x;
	planet->_y = y;
	planet->_z = z;

	return planet;
}

/* initialize a new origin object */
Origin* createOrigin(char* dimension, Planet* planet)
{
	if (dimension == NULL || planet->_planetName == NULL) return NULL;

	Origin* origin = (Origin*) malloc(sizeof(Origin));
	if (origin == NULL) return NULL;

	origin->_dimension = (char*) malloc(strlen(dimension) + 1);
	if (origin->_dimension == NULL) return NULL;

	strcpy(origin->_dimension, dimension);
	origin->_planet = planet;

	return origin;
}

/* initialize a new physical characteristic object */
PhysicalCharacteristics* createPhysicalCharacteristic(char* physicalName, double physicalValue)
{
	if (physicalName == NULL) return NULL;

	PhysicalCharacteristics* pc = (PhysicalCharacteristics*) malloc(sizeof(PhysicalCharacteristics));
	if (pc == NULL) return NULL;

	pc->_physicalName = (char*) malloc(strlen(physicalName) + 1);
	if (pc->_physicalName == NULL) return NULL;

	strcpy(pc->_physicalName, physicalName);
	pc->_physicalValue = physicalValue;

	return pc;
}

/* initialize a new jerry object */
Jerry* createJerry(char* id, int happinessLevel, Origin* origin)
{
	if (id == NULL) return NULL;

	Jerry* jerry = (Jerry*) malloc(sizeof(Jerry));
	if (jerry == NULL) return NULL;

	jerry->_jerryID = (char*) malloc(strlen(id) + 1);
	if (jerry->_jerryID == NULL) return NULL;

	strcpy(jerry->_jerryID, id);
	jerry->_happinessLevel = happinessLevel;
	jerry->_jerryOrigin = origin;
	jerry->_jerryPC = NULL;
	jerry->_jerryPcCounter = 0;

	return jerry;
}


/* --------------- Check Function --------------- */


/* check if jerry has a specific physical characteristic
 * if exist, return true, else return false */
bool checkPhysicalCharacteristics(Jerry* jerry, char* physicalName)
{
	if (jerry == NULL || physicalName == NULL) return false;

	int i;
	for (i = 0; i < jerry->_jerryPcCounter; i++)
	{
		if(strcmp(jerry->_jerryPC[i]->_physicalName, physicalName) == 0)
			return true;
	}

	/* the physical characteristic not exists in this jerry */
	return false;
}


/* --------------- Insertion Functions --------------- */


/* adding a new physical characteristic to specific jerry */
status addPhysicalCharacteristics(Jerry* jerry, PhysicalCharacteristics* physical)
{
	if (jerry == NULL || physical == NULL) return failure;

	/* the jerry's physical characteristic already exist */
	if (checkPhysicalCharacteristics(jerry, physical->_physicalName) == true) return failure;

	/* if the list is empty, allocate memory for it */
	if (jerry->_jerryPcCounter == 0)
	{
		jerry->_jerryPC = (PhysicalCharacteristics**) malloc(sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL)
			return failure;

		jerry->_jerryPC[0] = physical;
		jerry->_jerryPcCounter++;

		return success;
	}
	 /* the list is not empty */
	else
	{
		jerry->_jerryPC = (PhysicalCharacteristics**) realloc(jerry->_jerryPC, ((jerry->_jerryPcCounter) + 1) * sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL) return failure;

		jerry->_jerryPC[jerry->_jerryPcCounter] = physical;
		jerry->_jerryPcCounter++;

		return success;
	}
}


/* --------------- Remove Functions --------------- */


/* removing physical characteristic from specific jerry */
status removePhysicalCharacteristics(Jerry* jerry, char* physicalName)
{
	if (jerry == NULL || physicalName == NULL || jerry->_jerryPcCounter == 0)
		return failure;

	if (checkPhysicalCharacteristics(jerry, physicalName) == false)
		return failure;

	if (jerry->_jerryPcCounter == 1) //if only one Type remains, empty the list
	{
		free(jerry->_jerryPC);
		jerry->_jerryPC = NULL;
		jerry->_jerryPcCounter--;

		return success;
	}
	else
	{
		int counter = 0;
		int i = 0;

		for (i = 0; i < jerry->_jerryPcCounter; i++)
		{
			if(strcmp(((jerry->_jerryPC)[i])->_physicalName, physicalName) == 0)
				break;
			else
				counter++;
		}

		for (i = counter; i < jerry->_jerryPcCounter - 1; i++)
			*(jerry->_jerryPC[i] )= *(jerry->_jerryPC[i+1]);

		free(jerry->_jerryPC[(jerry->_jerryPcCounter)-1]);
		jerry->_jerryPC[(jerry->_jerryPcCounter)-1] = NULL;
		jerry->_jerryPcCounter--;

		jerry->_jerryPC = (PhysicalCharacteristics**) realloc(jerry->_jerryPC, (jerry->_jerryPcCounter) * sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL) return failure;

		return success;
	}
}

/* a function that returns a decimal number with 2 digits after the dot */
double roundTwoDigit(double number)
{
	return ((int)(number * 100.0 + 0.50001)) / 100.0;
}


/* --------------- Print Functions --------------- */


/* print jerry object */
status printJerry(Element element)
{
    if (element == NULL) return failure;

    Jerry* jerry = (Jerry*) element;

	printf("Jerry , ID - %s : \n", jerry->_jerryID);
	printf("Happiness level : %d \n", jerry->_happinessLevel);
	printf("Origin : %s \n", jerry->_jerryOrigin->_dimension);

	printPlanet(jerry->_jerryOrigin->_planet);

	if (jerry->_jerryPcCounter > 0)
	{
		printf("Jerry's physical Characteristics available : \n");
		printf("\t");
		int i;
		for (i = 0; i < jerry->_jerryPcCounter; i++)
		{
			double value = roundTwoDigit(jerry->_jerryPC[i]->_physicalValue);
			if (i != jerry->_jerryPcCounter - 1)
				printf("%s : %0.2f , ",jerry->_jerryPC[i]->_physicalName, value);
			else
				printf("%s : %0.2f \n",jerry->_jerryPC[i]->_physicalName, value);
		}
	}
	return success;
}

/* print planet object */
status printPlanet(Planet* planet)
{
	if (planet == NULL) return failure;

	double x = roundTwoDigit(planet->_x);
	double y = roundTwoDigit(planet->_y);
	double z = roundTwoDigit(planet->_z);
	printf("Planet : %s (%0.2f,%0.2f,%0.2f) \n", planet->_planetName, x, y, z);

	return success;
}


/* --------------- Destruction Functions --------------- */


/* releases the memory allocated for the planet object */
status destroyPlanet(Planet* planet)
{
	if (planet == NULL) return failure;

	free(planet->_planetName);
	planet->_planetName = NULL;
	free(planet);
	planet = NULL;

	return success;
}

/* releases the memory allocated for the origin object */
status destroyOrigin(Origin* origin)
{
	if (origin == NULL) return failure;

	free(origin->_dimension);
	origin->_dimension = NULL;
	free(origin);
	origin = NULL;

	return success;
}

/* releases the memory allocated for the physical characteristic object */
status destroyPhysicalCharacteristic(PhysicalCharacteristics* pc)
{
	if (pc == NULL) return failure;

	free(pc->_physicalName);
	pc->_physicalName = NULL;
	free(pc);
	pc = NULL;

	return success;
}

/* releases the memory allocated for the jerry object */
status destroyJerry(Element element)
{
	if (element == NULL) return failure;

	Jerry* jerry = (Jerry*) element;

	free(jerry->_jerryID);
	jerry->_jerryID = NULL;

	int i;
	for (i = 0; i < jerry->_jerryPcCounter; i++)
		if(destroyPhysicalCharacteristic(jerry->_jerryPC[i]) == failure) return failure;

	free(jerry->_jerryPC);
	jerry->_jerryPC = NULL;

	if (destroyOrigin(jerry->_jerryOrigin) == failure) return failure;

	free(jerry);
	jerry = NULL;

	return success;
}


/* --------------- Generic Functions --------------- */


//Element deepCopyJerry(Element element) // TODO: check if we need this func
//{
//    if (element == NULL) return NULL;
//
//    Jerry* jerry1 = (Jerry*) element;
//    Jerry* jerry2 = createJerry(jerry1->_jerryID, jerry1->_happinessLevel, jerry1->_jerryOrigin);
//    if (jerry2 == NULL) return NULL;
//    return jerry2;
//}

Element copyJerry(Element element)
{
    if (element == NULL) return NULL;
    Jerry* jerry = (Jerry*) element;
    return jerry;
}

Element copyID(Element element)
{
    if (element == NULL) return NULL;
    char* temp = (char*) element;
    char* id = (char*) malloc(strlen(temp)+1);
    if (id == NULL) return NULL;
    strcpy(id, temp);

    return id;
}

Element copyPC(Element element)
{
    if (element == NULL) return NULL;
    char* temp = (char*) element;
    char* pc = (char*) malloc(strlen(temp)+1);
    if (pc == NULL) return NULL;
    strcpy(pc, temp);
    return pc;
}

status freeJerry(Element element)
{
    if (element == NULL) return failure;
    return success;
}

status freeID(Element element)
{
    if (element == NULL) return failure;
    char* id = (char*) element;

    free(id);
    id = NULL;

    return success;
}

status freePC(Element element)
{
    if (element == NULL) return failure;
    char* pc = (char*) element;

    free(pc);
    pc = NULL;

    return success;
}

bool compareID(Element e1, Element e2)
{
    if (e1 == NULL || e2 == NULL) return false;
    char* key1 = (char*) e1;
    char* key2 = (char*) e2;

    return (strcmp(key1, key2) == 0);
}

bool compareJerries(Element e1, Element e2)
{
    if (e1 == NULL || e2 == NULL) return false;

    Jerry* jerry1 = (Jerry*) e1;
    Jerry* jerry2 = (Jerry*) e2;

    return (strcmp(jerry1->_jerryID, jerry2->_jerryID) == 0);
}

bool comparePC(Element e1, Element e2)
{
    if (e1 == NULL || e2 == NULL) return false;
    char* pc1 = (char*) e1;
    char* pc2 = (char*) e2;

    return (strcmp(pc1, pc2) == 0);
}

status printID(Element element)
{
    if (element == NULL) return failure;

    char* id = (char*) element;
    printf("%s : \n", id);

    return success;
}

status printPC(Element element)
{
    if (element == NULL) return failure;
    char* PC = (char*) element;

    printf("%s : \n", PC);
    return success;
}

int transformFunc(Element element)
{
    if (element == NULL) return -1;
    char* str = (char*)element;
    int number = 0;
    for (int i = 0; i < strlen(str); ++i)
        number+= (int)str[i];

    return number;
}