#!/bin/bash

#Definicion colores echo
RED='\033[1;31m'
YELLOW='\033[1;33m'
PURPLE='\033[1;35m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
CYAN='\033[1;36m'
NC='\033[0m' #No color

###############################################################################################################################

echo -e "${YELLOW}Checking libraries...${NC}"

if ldconfig -p | grep libSDL2 ; 
then
    echo -e "${BLUE}Library found.${NC}"
else
    if  [ $(id -u) = 0 ] 
    then
        echo -e "${RED}Library not found.${NC}"
        echo -e "${BLUE}Installing libraries...${NC}"
        sudo apt-get install libsdl2-dev
        if [ $? = 0 ] 
        then
            echo -e "${GREEN}Library install successful!${NC}"
        else
            echo -e "${RED}Library install ERROR! Exiting..."
            exit 1
        fi
    else
        echo -e "${RED}Library not found. Install it by using the command: ${GREEN}sudo apt-get install libsdl2-dev${NC}"
        echo -e "${RED}Or execute this script using sudo for automatic install: ${GREEN} sudo ./build.sh"
        exit 1
    fi
fi

###############################################################################################################################

echo -e "${YELLOW}Checking directories...${NC}"

if [ -d "build" ]; then
    echo -e "${BLUE}Build directory found.${NC}"
else
    echo -e "${RED}build directory does not exist, creating directory.${NC}"
    mkdir build
fi

###############################################################################################################################

cd build
echo -e "${YELLOW}Generating files...${NC}"
cmake ..

if [ $? = 0 ]
then
    echo -e "${GREEN}Files generated... executing make!${NC}"
    make
else
    echo -e "${RED}ERROR while cmake!${NC}"
    exit 1
fi

if [ $? = 0 ]
then
    echo -e "${GREEN}Build successful!${NC}"
    mv particles ..
    echo -e "${YELLOW}Execute particles program typing: ${CYAN}./particles${NC}"
    echo ""
    exit 0
else
    echo -e "${RED}ERROR while make!${NC}"
    echo -e "${RED}Exiting...${NC}"
    exit 1
fi

###############################################################################################################################

