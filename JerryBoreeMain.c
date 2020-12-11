#include "MultiValueHashTable.h"
#include "Jerry.h"
#define MAX_SIZE 300

Planet* searchPlanet(char* planet_name, int numOfPlanets, Planet** planetsArray);
status destroyAllPlanets(Planet** planetsArray, int numOfPlanets);
void clearAll(Planet** planetsArray, linkedList list, hashTable hash, multiValueHashTable multi, int numOfPlanets);


int main(int argc, char* argv[]) {
    int numOfPlanets = atoi(argv[1]);    /* load Planet amount */
    char *configuration = argv[2];          /* load configuration file */
    int planetsArraySize = 0;

    Planet **planetsArray = (Planet **) malloc(numOfPlanets * sizeof(Planet *));  /* create array pointers of planet */
    if (planetsArray == NULL) {
        printf("Memory Problem");
        exit(1);
    }

    linkedList jerriesLinkedList = createLinkedList(copyJerry, destroyJerry, printJerry, compareID, compareJerries);
    if (jerriesLinkedList == NULL) {
        free(planetsArray);
        printf("Memory Problem");
        exit(1);
    }

    hashTable hash = createHashTable(copyID, freeID, printID, copyJerry, freeJerry, printJerry, compareID,
                                     transformFunc, 11);
    if (hash == NULL) {
        free(planetsArray);
        destroyList(jerriesLinkedList);
        printf("Memory Problem");
        exit(1);
    }

    multiValueHashTable multi = createMultiValueHashTable(copyPC, freePC, printPC, copyJerry, freeJerry, printJerry,
                                                          comparePC, compareJerries, transformFunc, 11);
    if (jerriesLinkedList == NULL) {
        free(planetsArray);
        destroyList(jerriesLinkedList);
        destroyHashTable(hash);
        printf("Memory Problem");
        exit(1);
    }

    /* start to reading the file configuration */

    FILE *f;
    char str[MAX_SIZE];
    f = fopen(configuration, "r");
    fgets(str, MAX_SIZE, f);

    while (strstr(fgets(str, MAX_SIZE, f), "Jerries") == NULL) // insert all planets to the planetsArray
    {
        char *planet_name;
        double x, y, z;
        char *token = str;

        token = strtok(token, ",");
        planet_name = token;

        token = strtok(NULL, ",");
        x = atof(token);

        token = strtok(NULL, ",");
        y = atof(token);

        token = strtok(NULL, "\n\r");
        z = atof(token);

        planetsArray[planetsArraySize] = createPlanet(planet_name, x, y, z);

        if (planetsArray[planetsArraySize] == NULL)
            clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

        planetsArraySize++;
    }


    while (fgets(str, MAX_SIZE, f) != NULL)  // insert all jerries to the jerriesLinkedList
    {
        if (strstr(str, "\t") == NULL) {
            char *id;
            char *dimension;
            char *planet_name;
            int happiness_level;
            char *token = str;

            token = strtok(token, ",");
            id = token;

            token = strtok(NULL, ",");
            dimension = token;

            token = strtok(NULL, ",");
            planet_name = token;

            token = strtok(NULL, "\n\r");
            happiness_level = atoi(token);

            /* searching if the plant already exist in planetsArray */
            Planet *planet = searchPlanet(planet_name, numOfPlanets, planetsArray);

            /* create new origin for jerry */
            Origin *origin = createOrigin(dimension, planet);

            if (origin == NULL)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            /* create new jerry and adding him to jerriesArray */
            Jerry *newJerry = createJerry(id, happiness_level, origin);
            if (newJerry == NULL)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            /* insert jerry to linked list */
            if (appendNode(jerriesLinkedList, newJerry) == failure)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            /* insert jerry to hash table */
            if (addToHashTable(hash, newJerry->_jerryID, newJerry) == failure)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
        } else // adding physical characteristic to jerry
        {
            char *cp_name;
            double value;
            char *token = str;

            token = strtok(token, "\t");

            token = strtok(token, ":");
            cp_name = token;

            token = strtok(NULL, "\n\r");
            value = atof(token);

            /* create physical characteristic object */
            PhysicalCharacteristics *pc = createPhysicalCharacteristic(cp_name, value);

            if (pc == NULL)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            int index = getLengthList(jerriesLinkedList);

            /* get jerry from jerriesLinkedList by index */
            Jerry *jerry = (Jerry *) getDataByIndex(jerriesLinkedList, index);
            if (jerry == NULL)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            /* insert physical characteristic to jerry */
            if (addPhysicalCharacteristics(jerry, pc) == failure)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

            /* insert jerry to multiHash by his physical characteristic */
            if (addToMultiValueHashTable(multi, pc->_physicalName, jerry) == failure)
                clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
        }
    }
    fclose(f);

    /* finish to reading the file configuration */

    char input[MAX_SIZE];
    char choice;
    do {
        char id[MAX_SIZE];
        char planet_name[MAX_SIZE];
        char dimension[MAX_SIZE];
        char happiness_level[MAX_SIZE];
        int int_happiness_level;
        char pc_name[MAX_SIZE];
        char char_value[MAX_SIZE];
        double double_value;

        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");

        scanf("%s", input);
        if (strlen(input) != 1) // check if the input is one char
            choice = '0';
        else
            choice = input[0];

        switch (choice) {

            case '1':  // Insert new jerry to daycare

                printf("What is your Jerry's ID ? \n");

                scanf("%s", id);

                /* jerry already exists in the daycare */
                if (lookupInHashTable(hash, id) != NULL) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }

                printf("What planet is your Jerry from ? \n");

                scanf("%s", planet_name);

                Planet *planet = searchPlanet(planet_name, numOfPlanets, planetsArray);

                /* planet doesn't exist */
                if (planet == NULL) {
                    printf("%s is not a known planet ! \n", planet_name);
                    break;
                }

                printf("What is your Jerry's dimension ? \n");

                scanf("%s", dimension);

                printf("How happy is your Jerry now ? \n");

                scanf("%s", happiness_level);
                int_happiness_level = atoi(happiness_level);

                /* create a new origin object */
                Origin *origin = createOrigin(dimension, planet);
                if (origin == NULL)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* create a new jerry and adding him to jerriesArray */
                Jerry *newJerry = createJerry(id, int_happiness_level, origin);
                if (newJerry == NULL)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* insert jerry to linked list */
                if (appendNode(jerriesLinkedList, newJerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* insert jerry to hash table */
                if (addToHashTable(hash, newJerry->_jerryID, newJerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                printJerry(newJerry);

                break;


            case '2':  // insert physical characteristic to specific jerry

                printf("What is your Jerry's ID ? \n");
                scanf("%s", id);

                Jerry *jerry = lookupInHashTable(hash, id);

                /* jerry doesn't exist in the daycare */
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic can you add to Jerry - %s ? \n", id);

                scanf("%s", pc_name);

                /* physical characteristic already exists */
                if (checkPhysicalCharacteristics(jerry, pc_name) == true) {
                    printf("The information about his %s already available to the daycare ! \n", pc_name);
                    break;
                }

                printf("What is the value of his %s ? \n", pc_name);

                scanf("%s", char_value);
                double_value = atof(char_value);

                PhysicalCharacteristics *pc = createPhysicalCharacteristic(pc_name, double_value);
                if (pc == NULL)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                if (addPhysicalCharacteristics(jerry, pc) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                if (addToMultiValueHashTable(multi, pc_name, jerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                printf("%s : \n",pc_name);
                displayMultiValueHashElementsByKey(multi, pc_name);

                break;


            case '3':  // Remove physical characteristic from Jerry
                printf("What is your Jerry's ID ? \n");
                scanf("%s", id);

                jerry = lookupInHashTable(hash, id);

                /* jerry doesn't exist in the daycare */
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id);
                scanf("%s", pc_name);

                /* physical characteristic doesn't exist */
                if (checkPhysicalCharacteristics(jerry, pc_name) == false) {
                    printf("The information about his %s not available to the daycare ! \n", pc_name);
                    break;
                }

                if (removeFromMultiValueHashTable(multi, pc_name, jerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                if (removePhysicalCharacteristics(jerry, pc_name) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                printJerry(jerry);

                break;


            case '4':   // remove jerry from daycare

                printf("What is your Jerry's ID ? \n");
                scanf("%s", id);

                jerry = lookupInHashTable(hash, id);

                /* jerry doesn't exist in the daycare */
                if (jerry == NULL) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                int size = jerry->_jerryPcCounter;
                char *pc_name4;

                /* remove jerry from multiHash */
                for (int i = 0; i < size; ++i) {
                    pc_name4 = jerry->_jerryPC[i]->_physicalName;
                    if (removeFromMultiValueHashTable(multi, pc_name4, jerry) == failure)
                        clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
                }

                /* remove jerry from hash table */
                if (removeFromHashTable(hash, id) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* remove jerry from linked list */
                if (deleteNode(jerriesLinkedList, jerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                break;


            case '5':    //  remove jerry that has the most similar physical characteristic value

                printf("What do you remember about your Jerry ? \n");
                scanf("%s", pc_name);
                linkedList similarJerry = lookupInMultiValueHashTable(multi, pc_name);

                /* there is no jerries with this physical characteristic */
                if (similarJerry == NULL) {
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc_name);
                    break;
                }

                printf("What do you remember about the value of his %s ? \n", pc_name);
                scanf("%s", char_value);
                double_value = atof(char_value);

                double value = 0;
                double max_similar_value = double_value;
                int index = 0;

                /* passing all similarJerry list */
                for (int i = 1; i <= getLengthList(similarJerry); ++i) {
                    jerry = (Jerry *) getDataByIndex(similarJerry, i);

                    /* passing all physical characteristic until the pc is equal to pc_name */
                    for (int j = 0; j < jerry->_jerryPcCounter; j++) {
                        if (strcmp(jerry->_jerryPC[j]->_physicalName, pc_name) == 0) {
                            value = double_value - jerry->_jerryPC[j]->_physicalValue;

                            if (value < 0)
                                value = value * (-1);

                            if (value < max_similar_value) {
                                max_similar_value = value;
                                index = i;  /* jerry's index in similarJerry list */
                            }
                        }
                    }
                }

                jerry = (Jerry *) getDataByIndex(similarJerry, index);
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerry);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                size = jerry->_jerryPcCounter;
                char *pc_name5;

                /* remove jerry from multiHash */
                for (int i = 0; i < size; ++i) {
                    pc_name5 = jerry->_jerryPC[i]->_physicalName;
                    if (removeFromMultiValueHashTable(multi, pc_name5, jerry) == failure)
                        clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
                }
                /* remove jerry from hash table */
                if (removeFromHashTable(hash, jerry->_jerryID) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* remove jerry from linked list */
                if (deleteNode(jerriesLinkedList, jerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
                break;
//
            case '6':    // remove the most saddest jerry from daycare

                /* there is no jerries in daycare */
                if (getLengthList(jerriesLinkedList) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }

                int saddest_jerry_value = 101;
                int happinessLevel;

                for (int i = 1; i <= getLengthList(jerriesLinkedList); i++) {
                    jerry = (Jerry *) getDataByIndex(jerriesLinkedList, i);

                    happinessLevel = jerry->_happinessLevel;

                    if (happinessLevel < saddest_jerry_value) {
                        saddest_jerry_value = happinessLevel;
                        index = i;
                    }
                }

                jerry = (Jerry *) getDataByIndex(jerriesLinkedList, index);
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(jerry);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                size = jerry->_jerryPcCounter;
                char *pc_name6;

                /* remove jerry from multiHash */
                for (int i = 0; i < size; ++i) {
                    pc_name6 = jerry->_jerryPC[i]->_physicalName;
                    if (removeFromMultiValueHashTable(multi, pc_name6, jerry) == failure)
                        clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);
                }

                /* remove jerry from hash table */
                if (removeFromHashTable(hash, jerry->_jerryID) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                /* remove jerry from linked list */
                if (deleteNode(jerriesLinkedList, jerry) == failure)
                    clearAll(planetsArray, jerriesLinkedList, hash, multi, numOfPlanets);

                break;


            case '7':   // printing information about jerry


                printf("What information do you want to know ? \n");
                printf("1 : All Jerries \n");
                printf("2 : All Jerries by physical characteristics \n");
                printf("3 : All known planets \n");

                scanf("%s", input);
                if (strlen(input) != 1) // check if the input is one char
                    choice = '0';
                else
                    choice = input[0];

                switch (choice) {
                    case '1':   // print all jerries

                        /* there is no jerries in daycare */
                        if (getLengthList(jerriesLinkedList) == 0) {
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }

                        displayList(jerriesLinkedList);
                        break;

                    case '2':   // print all Jerries by physical characteristics

                        printf("What physical characteristics ? \n");
                        scanf("%s", pc_name);

                        linkedList list = lookupInMultiValueHashTable(multi, pc_name);

                        /* there is no jerry with this physical characteristic */
                        if (list == NULL) {
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc_name);
                            break;
                        }

                        printf("%s : \n", pc_name);
                        displayMultiValueHashElementsByKey(multi, pc_name);
                        break;

                    case '3':   // print all planets

                        for (int i = 0; i < numOfPlanets; ++i)
                            printPlanet(planetsArray[i]);
                        break;

                    default:    // upon invalid input, exit

                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;


            case '8': // let the jerries play

                /* there is no jerries in daycare */
                if (getLengthList(jerriesLinkedList) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }

                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");

                scanf("%s", input);
                if (strlen(input) != 1) // check if the input is one char
                    choice = '0';
                else
                    choice = input[0];

                switch (choice) {
                    case '1':   // interact with fake Beth

                        for (int i = 1; i <= getLengthList(jerriesLinkedList); i++) {
                            jerry = (Jerry *) getDataByIndex(jerriesLinkedList, i);

                            happinessLevel = jerry->_happinessLevel;

                            if (happinessLevel >= 20) {
                                happinessLevel = happinessLevel + 15;
                                if (happinessLevel > 100) happinessLevel = 100;

                                jerry->_happinessLevel = happinessLevel;
                            } else {
                                happinessLevel = happinessLevel - 5;
                                if (happinessLevel < 0) happinessLevel = 0;
                                jerry->_happinessLevel = happinessLevel;
                            }
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerriesLinkedList);
                        break;

                    case '2':   // let jerries play golf

                        for (int i = 1; i <= getLengthList(jerriesLinkedList); i++) {
                            jerry = (Jerry *) getDataByIndex(jerriesLinkedList, i);

                            happinessLevel = jerry->_happinessLevel;

                            if (happinessLevel >= 50) {
                                happinessLevel = happinessLevel + 10;
                                if (happinessLevel > 100) happinessLevel = 100;

                                jerry->_happinessLevel = happinessLevel;
                            } else {
                                happinessLevel = happinessLevel - 10;
                                if (happinessLevel < 0) happinessLevel = 0;
                                jerry->_happinessLevel = happinessLevel;
                            }
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerriesLinkedList);
                        break;


                    case '3':   // print all planets

                        for (int i = 1; i <= getLengthList(jerriesLinkedList); i++) {
                            jerry = (Jerry *) getDataByIndex(jerriesLinkedList, i);

                            happinessLevel = jerry->_happinessLevel + 20;

                            if (happinessLevel > 100) happinessLevel = 100;

                            jerry->_happinessLevel = happinessLevel;
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerriesLinkedList);
                        break;

                    default:    // upon invalid input, exit

                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '9': // Go home

                printf("The daycare is now clean and close ! \n");
                destroyMultiValueHashTable(multi);
                destroyHashTable(hash);
                destroyList(jerriesLinkedList);
                destroyAllPlanets(planetsArray, numOfPlanets);
                break;


            default: //upon invalid input, exit
                printf("Rick this option is not known to the daycare ! \n");
                break;

        }
    } while (choice != '9');

    return 0;
}


/* searching specific planet in planetArray
 * if exist return pointer to planet
 * if not exist return NULL */
Planet* searchPlanet(char* planet_name, int numOfPlanets, Planet** planetsArray)
{
    int i;
    for (i = 0; i < numOfPlanets; i++)
    {
        if (strcmp(planet_name, planetsArray[i]->_planetName) == 0)
            return planetsArray[i];
    }
    return NULL;
}


status destroyAllPlanets(Planet** planetsArray, int numOfPlanets) {
    if (planetsArray == NULL) return failure;

    int i;
    for (i = 0; i < numOfPlanets; i++)
        destroyPlanet(planetsArray[i]);
    free(planetsArray);
    planetsArray = NULL;

    return success;
}

void clearAll(Planet** planetsArray, linkedList list, hashTable hash, multiValueHashTable multi, int numOfPlanets) {
    printf("Memory Problem");

    destroyMultiValueHashTable(multi);
    destroyHashTable(hash);
    destroyList(list);
    destroyAllPlanets(planetsArray, numOfPlanets);

}