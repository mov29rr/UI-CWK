# Team members
 - Maxim Varea 201694348
 - Naputt
 - Leyna
 - Yuval
 - Aidan

# Project Overview

The application is meant to display water monitoring data, it reads data to view the water compliance, pollutant contamination levels from different categories, and gives overviews for all data presented.

# How to run

1. Unzip the database.zip file in the database folder
2. Enter the following into the command line:
```sh
mkdir build
cd build
cmake ..
make
./WaterQualityInspector
```

# Libraries

CSV reader provided by University.

# File Structure

Top level folders:
 - src: holds all source code data
 - stylesheets: holds the QSS stylesheets
 - database: holds the database
 - translations: holds the translation files
 - sandbox: holds testing code

# Known issues

- Currently the main dashboard only displays overviews once the pages are clicked and the data is loaded, this means that before hand they are empty.
- The points don't unselect when clicked elsewhere
- Pollutants page doesn't show line graph data correctly