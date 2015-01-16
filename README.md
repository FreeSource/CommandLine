__Note:__ This file contains a basic detail about the project which helps with building and testing the C++ code and description of the directory structure.

---

Building the Project
--------------------

To build the project type __gmake__ or __make__ from the root project directory.  
To cleaning up old files type __gmake clean__ or __make clean__ from the root project directory.  
To test the project type __gmake__ or __make__ from the test project directory.

Directory structure
-------------------

- __/project:__ *this organizes your project components.*

- __build:__ *directory used to hold the built files.*

- __build/system:__ *compilation results for each operational system.*

- __build/system/bin:__ *directory used to hold the executable binary files.*

- __build/system/obj:__ *directory used to hold the built object files.*

- __build/system/lib:__ *directory used to hold the built library files.*

- __doc:__ *this directory holds all the project documentation.*

- __ext:__ *libraries provided by third-party go here.*

- __ext/include:__ *this directory holds all header files declarations for libraries provided by third-party.*

- __ext/lib:__ *this directory holds all libraries provided by third-party.*

- __ext/lib/system:__ *this directory holds all libraries provided by third-party for each operational system.*

- __include:__ *this directory holds all header files declarations for the src directory.*

- __src:__ *this directory contains the implementation source files for the project.*

- __test:__ *the tests you write (unit tests, fixtures, and functional tests) all go here.*