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
