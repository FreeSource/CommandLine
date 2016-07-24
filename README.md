__Note:__ This file contains a basic detail about the project which helps with building and testing the C++ code and description of the directory structure.

---

Building the Project
--------------------

To build the project type __gmake__ or __make__ from the home project directory.  
To cleaning up old files type __gmake clean__ or __make clean__ from the home project directory.  
To test the project type __gmake__ or __make__ from the test project directory.

Directory structure
-------------------

project-name
+---doc
+---include
+---os
|   \---os-name
|       +---build
|       |   +---bin
|       |   +---lib
|       |   \---obj
|       \---lib
+---rc
+---src
\---test

- __project-name:__ *Project home directory to organize all project components.*

- __doc:__ *This directory holds all the project documentation.*

- __include:__ *This directory contains all project declarations.*

- __os:__ *Directory used to hold different operating system components like executables and libraries.*

- __os/include:__ *This directory holds all external declarations for libraries provided by third-party.*

- __os-name:__ *Operating system directory. Examples: Linux, Windows, MacOS, FreeBSD, etc.*

- __build:__ *Directory used to hold the built files.*

- __bin:__ *Directory used to hold the executable binary files.*

- __build/lib:__ *Directory used to hold the built library files.*

- __obj:__ *Directory used to hold the built object files.*

- __lib:__ *This directory holds all libraries provided by third-party.*

- __rc:__ *This directory contains the resource files for the project.*

- __src:__ *This directory contains the implementation source files for the project.*

- __test:__ *The tests you write (unit tests, fixtures, and functional tests) all go here.*