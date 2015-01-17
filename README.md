__Note:__ This file contains a basic detail about the project which helps with building and testing the C++ code and description of the directory structure.

---

Building the Project
--------------------

To build the project type __gmake__ or __make__ from the home project directory.  
To cleaning up old files type __gmake clean__ or __make clean__ from the root project directory.  
To test the project type __gmake__ or __make__ from the test project directory.

Directory structure
-------------------

- __/project:__ *project home directory to organize all project components.*

- __/doc:__ *this directory holds all the project documentation.*

- __/include:__ *this directory contains declarations and definitions to be shared between several source files.*

- __/ext:__ *this directory holds all external declarations for libraries provided by third-party.*

- __/local:__ *this directory contains all project declarations.*

- __/os:__ *directory used to hold different operating system components like executables and libraries.*

- __/osname:__ *operating system directory.*

- __/build:__ *directory used to hold the built files.*

- __/bin:__ *directory used to hold the executable binary files.*

- __/lib:__ *directory used to hold the built library files.*

- __/obj:__ *directory used to hold the built object files.*

- __/extlib:__ *this directory holds all libraries provided by third-party.*

- __/rc:__ *this directory holds all resources which could consist of images, string tables, version info resources, etc.*

- __/test:__ *the tests you write (unit tests, fixtures, and functional tests) all go here.*