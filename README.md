This is a package to install the MEGARA FMPT, the Fiber MOS Positioning Tool for MEGARA.

Copyright IAA-CSIC 2014-2016.

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
The Fiber MOS Positioning Tool (FMPT) forms part of the MOPSS. The FMPT will compute the movements to be done by the RPs in a configuration block to ensure no collisions among adjacent positioners while minimizing the time to configure all of them. This release includes the implementation of the function to generate depositioning programs of type step-by-step using as an input a table with RPs assignments provided by the Fiber MOS Assignment Tool (FMAT).

This package provides a positioning library (libfmtp) and command line interface
to it called 'fmpt_saa'

License
-----------------------------------

Megara FMPT is distributed under GPLv3 License. See the file 'COPYING' for details.
Provided that this S/W is used to the instrument MEGARA, this license is intended that GRANTECAN will be able to use and modify it without restrictions, while developers retain rights to modify and use it for other uses. The software has two years of warranty while it remains without being modified.

Requirements
----------------------------------

This package is written in C++, requires GNU C++ to compile.

It has been tested in:
- Ubuntu 14.04.3 x86 64-bit.
- SUSE Linux Enterprises SLES 11 SP1 64-bit.
- Mac OS X 10.7, 10.9 and 10.10.

Installation
---------------------------------

The process to install this S/W can be found in the file INSTALL. The summarized process is explained below:

1. Decompress tar file 'megara-fmpt-3.9.3.tar.gz' (or .xz).
2. Change to directory './megara-fmpt-3.9.3'.
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

Execute the FMPT SAA application writing at the shell:

$ fmpt_saa <path>

where path is the absolute path of the file to be executed. For example, to execute one of the installed examples:

$ fmpt_saa /usr/local/share/megara-fmpt/Samples/megara-cb1.txt

These locations can vary depending on the operating system and distribution where this package is installed.

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

Developers
----------

* Doxygen Documentation: <a href="https://codedocs.xyz/guaix-ucm/megara-fmpt/"><img src="https://codedocs.xyz/guaix-ucm/megara-fmpt.svg"/></a>


Webpage: http://guaix.fis.ucm.es/megara 
FMPT Maintainer: isaac@iaa.es
