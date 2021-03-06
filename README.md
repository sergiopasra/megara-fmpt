This is a package to install the MEGARA FMPT, the Fiber MOS Positioning Tool for MEGARA.

Copyright IAA-CSIC 2012-2017.

##################
Table of Contents
##################

*Brief overview
*License
*Requirements
*Installation
*Using MEGARA FMPT
*Generating a new release

Brief overview
-----------------------------------
The Fiber MOS Positioning Tool (FMPT) forms part of the MOPSS. This package (megara-fmpt) provides the library FMPT (libfmtp) and an the application MFPT SAA for facilitate the use of the offline functions provided by the library. Has been implemented other package (megara-fmpt-gui) which provided the application FMPT GUI for ilustrate the use of the functions for generate animations.

The library FMPT provide functions for generate motion programs which allow move the RPs avoiding collisions among adjacents. The FMPT_SAA facilitates the use of the offline functions of the library FMPT. The offline functions generate a file type outputs from a file type FMOSA provided by the FMAT. A file type FMOSA contains a table of allocation of RPs to OS (observing sources). 

the FMPT generates two class of motion programs:
- parking aprogram: for move the RPs fron any position to their parking prosition (with their rotors in the origins).
- pair (PP, DP): for move the RPs from their initial positions to their observing positions, and bak to their initial positions.

Archivos typo outputs generados con la función generateParkProg, contains a parking program. A file type outputs generated by the function generatePairPPDP_offline, contains a pair (PP, DP) followed by the content of the file type FMOSA from which it was generated.

The FMPT otimize the following factor:
- Reduce the risk of collission. Garantee that al movements keep a safe distance among the arms of the RPs, and when a RP is moved, none of their adjacent RPs is moved.
- Maximice the use of the available space. Only the space ocupied for each arm and a little space around their contour will can not be used.
- Keet limited the displacement time of the RPs.

License
-----------------------------------

Megara FMPT is distributed under GPLv3 License. See the file 'COPYING' for details.
Provided that this S/W is used to the instrument MEGARA, this license is intended that GRANTECAN will be able to use and modify it without restrictions, while developers retain rights to modify and use it for other uses. The software has two years of warranty while it remains without being modified.

Requirements
----------------------------------

This package is written in C++, requires GNU C++ to compile.

It has been tested in:
- Ubuntu 16.04 x86 64-bit.

Installation
---------------------------------

The process to install this S/W can be found in the file INSTALL. The summarized process is explained below:

1. Decompress tar file 'megara-fmpt-4.5.0.tar.gz' (or .xz).
2. Change to directory './megara-fmpt-4.5.0'.
3. Execute './configure; make;' to configure and build this package.
4. Grant superuser privileges to have access to '/usr/local/...'.
5. Execute 'make install' (as superuser) to install the library and the application. 
6. If yout operative system is Linux, maybe you need execute 'ldconfig' (as superuser) for update the linkage with the libraries of the system.

This process will install the libraries (static and dynamic) FMPT, and the FMPT SAA application, in the usual directories of the user distribution of the user operative system. For example:
- the libraries (estatic and dynamic) will be installed in '/usr/local/lib';
- the executable file fmpt_saa will be installed in '/usr/local/bin';
- the instance of the Fiber MOS Model will be installed in: '/usr/local/share/megara-fmpt/models/MEGARA_FiberMOSModel_Instance';
- the sample input files will be installed in: 'usr/local/bin/share/megara-fmpt/Samples';

These locations can vary depending on the operating system and distribution where this package is installed.

Using MEGARA FMPT
---------------------------------

The FMPT have three principal functionalities:
1. Generation offline of pairs (PP, DP).	(Function generatePairPPDP_offline)
2. Generation online of the parking programs.	(Function generateParkProg_online).
2. Regeneration online of pairs (PP, DP).	(Function generatePairPPDP_online).

For use the function generatePairPPDP_offline you shall write in the shell:
    $ fmpt_saa generatePairPPDP_offline <FMOSA_path>

where FMOSA_path is the absolute path of the file type FMOSA to be executed. For example, to execute one of the installed examples:
    $ fmpt_saa /usr/local/share/megara-fmpt/Samples/megara-cb1.txt

These locations can vary depending on the operating system and distribution where this package is installed.

For use the online functions, you shall include the header file:
    #include <megara-fmpt/MotionProgramGenerator.h>

Functions online are in the namespace "Positioning". Them you can simplify their access writing:
    using namespace Positioning;

An offline function for generate parking programs has been included with intention that the user becomes familiar with its operation. For use this function you can write in the shell:
    $ fmpt_saa generateParkProg_offline <FMOSA_path>

The bames of the generated files will have the sintax:
    ParkProg_<string>_from_<FMOSA_filename>
    PairPPDP_<string>_from_<FMOSA_filename>

Files for strings "outputs" and "other-outputs" will be generated by default.

Offline functions have arguments for indicate additional files to be saved. A detailed help for each offline function can be got writing in the shell:
    $ fmpt_saa help_generateParkProg_offline
    $ fmpt_saa help_generatePairPPDP_offline

Argument "all" may be specified for generate all additional files:
    $ fmpt_saa generateParkProg_offline <FMOSA_path> all
    $ fmpt_saa generatePairPPDP_offline <FMOSA_path> all

Help for aditional functionalities can be obtained writting in the shell:
    $ fmpt_saa help_others

When in the course of life of instrument MEGARA, the Fiber MOS 

Generating a new release
---------------------------------

1. Decompress file 'megara-fmpt-<n>.tar.gz' (or .xz). Where <n> is the number of version.
2. Perform the changes in the code, if any.
3. Update version number in the following files:
     'megara-fmpt-<n>/src/main.cpp'
     'megara-fmpt-<n>/configure.ac'
     'megara-fmpt-<n>/README'
     'megara-fmpt-<n>/TEC_MEG_171.doc'
4. Change to directory './megara-fmpt-<n>'.
5. Execute './configure; make [-j];' to configure and build this package.
6. Execute 'make distcheck [-j]' to build the new release.
   During generation are executed the tests, and for pass the tests are used the RP model and the Fiber MOS Model instance stored in:
    'data/Models/MEGARA_RP_Instance'
    'data/Models/MEGARA_FiberMOSModel_Instance'

Known issues
============
If you dont use ldconfig for update the linkage to the libraries of the system, when you execute the fmpt_saa application, would be printed the following error message:
	fmpt_saa. error while loading shared libraries: libfmp.so: can not open shared object file: No such file or directory.

One alternative to the use of ldconfig is add the path '/usr/local/lib' to the environmen variable LD_LIBRARY_PATH.
For this you can add the following lines to the file '.bashrc' or the corresponding to the shell of your system:
	LD_LiBRARY_PATH=/usr/local/lib
	export LD_LIBRARY_PATH

The package fails to compile with clang, the default compiler in Mac OSX 10.9
and higher versions. To compile in these versions, the GNU C++ compiler is required.
To override the default compiler use the following syntax:

$ ./configure CXX=/path/to/gnu-g++/binary

and then 'make', 'make install' as usual.

Webpage: http://guaix.fis.ucm.es/megara 
FMPT Maintainer: isaac@iaa.es

Developers
----------

* Build Status: <a href="https://travis-ci.org/guaix-ucm/megara-fmpt"><img src="https://secure.travis-ci.org/guaix-ucm/megara-fmpt.png?branch=master"/></a>

* Doxygen Documentation: <a href="https://codedocs.xyz/guaix-ucm/megara-fmpt/"><img src="https://codedocs.xyz/guaix-ucm/megara-fmpt.svg"/></a>


Webpage: http://guaix.fis.ucm.es/megara 
FMPT Maintainer: isaac@iaa.es

