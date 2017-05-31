## Integration into the INET framework of OMNeT++

##### Step 1: Clone the repo
- cd inet/src/inet
- git clone https://github.com/BenSliwa/LIMoSim

##### Step 2: Update the include paths of the INET framework
- Right click on the inet project -> Properties -> C/C++ General -> Paths and Symbols
- Includes Tab: **Select GNU C++** and click on 'Add...'
- Workspace... -> inet/src/inet/LIMoSim

##### Step 3: Setup resource filters to exclude the UI
- Resource -> Resource Filters --> Add Filter...
- Filter type: Exclude all
- Applies to: Files and folders, All children (recursive)
- Name matches: ui


## UI Part

#### Qt 5.7
- Download and install Qt from https://www.qt.io/download/ (at least Qt 5.7 is required)
- Open the Qt-project file LIMoSim/ui/LIMoSim.pro
- Make sure that the build folder is outside the inet workspace. Qt generates .cpp files during the build process, which would interfer with the .cc files of INET

#### Running an Example
- Currently, the path the map directory needs to be specified in the main.cpp file
- Change the **dir** variable to the absolute path of your LIMoSim/resources/ location
- The current version does not include a UI-based tool for generating vehicle configuration files yet but the cars will print their current configuration as XML-data to the command line if they are clicked in the UI
