// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: main_example_generateParkProg_online.cpp
//Content: program for ilustrate the use of the online generation function
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

//###########################################################################
//This file contains an example to ilustrate the use of the FMPT library
//for generate a parking program using the online function.
//If you want compile and execute this example perform the following actions:
//
//    1. Decompress the release 'megara-fmpt-3.9.3.tar.gz (or xz)
//
//    2. Move this file to 'megara-fmpt-3.9.3/src'.
//
//    3. Edit the file 'megara-fmpt-3.9.3/src/Makefile.am', and change:
//           bin_PROGRAMS = fmpt_saa
//           fmpt_saa_SOURCES = main.cpp
//           fmpt_saa_LDADD = libfmtp.la
//           fmpt_saa_CPPFLAGS = $(AM_CPPFLAGS)
//       By:
//           bin_PROGRAMS = fmpt_example_generateParkProg_online
//           fmpt_example_generateParkProg_online_SOURCES = main_example_generateParkProg_online.cpp
//           fmpt_example_generateParkProg_online_LDADD = libfmtp.la
//           fmpt_example_generateParkProg_online_CPPFLAGS = $(AM_CPPFLAGS)
//
//    4. Install and execute the example in a separated directory:
//           $ make build
//           $ cd build
//           $ ../megara-fmpt-3.9.3/configure (or the corresponding path)
//           $ sudo make install
//
//    Then the executable will be written in '/usr/local/bin' (or similar).
//    If you have installed previously the FMPT SAA, the directory will look:
//        fmpt_saa
//        fmpt_example_generateParkProg_online
//
//    If you don't have installed the FMPT SAA previously, maybe you need
//    execute $ sudo ldcondig for assimilate the FMPT library in the system.
//
//    Now you may execute the example with the command:
//        $ fmpt_example_generateParkProg_online.
//    The output files will be written in the directory where you execute it.
//###########################################################################

#include "../megarafmpt/src/FileMethods.h"
#include "../megarafmpt/src/MotionProgramGenerator.h"
#include "../megarafmpt/src/FMOSATable.h"
#include "../megarafmpt/src/PositionerCenter.h"
#include "../megarafmpt/src/Strings.h"
#include "../megarafmpt/src/TextFile.h"
#include "../megarafmpt/src/Geometry.h" //distanceSegmentPoint
#include "../megarafmpt/src/Outputs.h" //Outputs

#include <QCoreApplication> //Qt only
#include <locale.h> //setlocale, LC_NUMERIC
#include <iostream> //std::cout

using namespace Strings;
using namespace Models;
using namespace Positioning;

//---------------------------------------------------------------------------
//GENERAL FUNCTIONS:

//print a text line in the standard output and in the log file
void append(const string& str, const char *log_filename)
{
    string str_aux = str;
    str_aux += "\r\n";

    std::cout << str_aux;

    char mode[] = "a";
    TTextFile TF(log_filename, mode);
    TF.Print(str_aux.c_str());
    TF.Close();
}

//split a path
void splitpath(string& parent_path, string& filename, const string& path)
{
    int i = path.length() - 1;
    while(i>=0 && path[i] != '/')
        i--;

    parent_path.clear();
    if(i >= 0)
        parent_path = path.substr(0, i);

    filename.clear();
    int aux = path.length() - 1;
    if(i < aux) {
        int count = aux - i;
        filename = path.substr(i+1, count);
    }
}

//main function
int main(int argc, char *argv[])
{
    //-----------------------------------------------------------------------
    //BUILD THE PATH FOR THE INSTANCE OF THE Fiber MOS Model:

    //###################################################################
    //Using autotools over Linux:
    //-------------------------------------------------------------------
    //
    //Selecting '/home/User/MEGARA' how working directory, before compilation:
    //  sources shall be in:    '/home/User/MEGARA/megarafmpt/src'
    //  data shall be in:       '/home/User/MEGARA/megarafmpt/data'
    //
    //When compilation has been make in the same directory named megarafmpt:
    //  sources will be in:     '/home/User/MEGARA/megarafmpt/src'
    //  data will be in:        '/home/User/MEGARA/megarafmpt/data'
    //
    //When compilation has been make in an appart directory named build:
    //  sources will be in:     '/home/User/MEGARA/build/src'
    //  data will be in:        '/home/User/MEGARA/build/data'
    //
    //When the installation has been make:
    //  executable will be in:  'prefix/bin'
    //  data will be in:        'prefix/share/megara-fmpt'
    //Where prefix is probably '/usr/local'.
    //
    //Then the program fmpt_saa compiled with autotools, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      option 1, when program is installed:    DATADIR+"/Models/MEGARA_FiberMOSModel_Instance"
    //      option 2, when program is compiled:     getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      path(argv[2])
    //
    //DATADIR is a macro which can be used when the program is compiled using autotools.
    //DATADIR will be probably: "/urs/local/share/megara-fmpt"
    //
    //When the program is compiled using Qt Creator over Linux:
    //-------------------------------------------------------------------
    //
    //The Qt project and the main.cpp file will be in:
    //  '/home/User/MEGARA/FMPT_SAA-CLI'
    //The object code and the executable will be in:
    //  '/home/User/MEGARA/build-FMPT_SAA-CLI-Desktop-Debug'
    //The other source files and data files will be in the same repository for autotools,
    //whose relative path from the executable is:
    //  for source files:   '../megarafmpt/src'
    //  for data files:     '../megarafmpt/data'
    //
    //Then the program fmpt_saa compiled with Qt, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      when program is released:  getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //      when program is debugging: getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      for applyPositionerCenterTable: path = getCurrentDir()+"/../megarafmpt/data/Models/positionersCenters.txt";
    //      for applyRP:                    path = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
    //      for generate MPs:               path = getCurrentDir()+"/../megarafmpt/data/Samples/megara-cb6.txt";
    //
    //Using Qt over Windows:
    //-------------------------------------------------------------------
    //
    //Selecting 'D:\User\MEGARA' how working directory:
    //The Qt project and the main.cpp file will be in:
    //  'D:\User\MEGARA\FMPT_SAA-CLI'
    //The object code and the executable will be in:
    //  'D:\User\MEGARA\build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Debug'
    //  'D:\User\MEGARA\build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Release'
    //
    //Then the program fmpt_saa compiled with Qt, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      when program is released:  getCurrentDir()+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
    //      when program is debugging: getCurrentDir()+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      for applyPositionerCenterTable: path = getCurrentDir()+"/../megarafmpt/data/Models/positionersCenters.txt";
    //      for applyRP:                    path = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
    //      for generate MPs:               path = getCurrentDir()+"/../megarafmpt/data/Samples/megara-cb6.txt";
    //
    //###################################################################

    //-----------------------------------------------------------------------
    //BUILD A QCoreApplication (QT ONLY):

    //get the actual directory in argv[0]
    QCoreApplication a(argc, argv);

    //-----------------------------------------------------------------------

    //initalize the log file
    string log_filename; //the log filename
    try {
        log_filename = "example_generateParkProg_online.log";
        char mode[] = "w";
        TTextFile TF(log_filename.c_str(), mode);
        TF.Close();
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        cout << "ERROR: "+E.Message.str << endl;
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknown exception
        cout << "ERROR: unknown exception" << endl;
        return 2;
    }

    try {
        //-------------------------------------------------------------------
        //CONFIGURATE THE SYSTEM:

        //REMEMBER: exceptions in runtime can be due to that
        //the system is not configurated.

        //configurates the decimal separator
        setlocale(LC_NUMERIC, "C");

        //-----------------------------------------------------------------------
        //LOAD THE FIBER MOS MODEL INSTANCE FROM A DIR:

        //indicates that the program is running
        append("FMPT example generateParkProg_online is running...", log_filename.c_str());

        //built the paths where search the Fiber MOS Model
        //string dir_FMM1 = DATADIR;
        //dir_FMM1 += "/Models/MEGARA_FiberMOSModel_Instance";
        //string dir_FMM2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

        //Qt only:
        string dir_FMM1 = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance";
        string dir_FMM2 = dir_FMM1;

        //load the instance of the Fiber MOS Model from a dir
        TFiberMOSModel FMM;
        string dir_FMM;

        try {
            append("\r\nLoading FMM instance from: '"+dir_FMM1+"'.", log_filename.c_str());
            readInstanceFromDir(FMM, dir_FMM1);
            dir_FMM = dir_FMM1;
        }
        catch(Exception& E) {
            append("FMM instance can't be loaded: "+E.Message.str, log_filename.c_str());
            try {
                append("Loading FMM instance from: '"+dir_FMM2+"'.", log_filename.c_str());
                readInstanceFromDir(FMM, dir_FMM2);
                dir_FMM = dir_FMM2;
            }
            catch(Exception& E) {
                E.Message.Insert(1, "loading instance: ");
                throw;
            }
        }
        append("FMM instance loaded.", log_filename.c_str());

        //-------------------------------------------------------------------
        //DECLARE AND BUILD ALL VARIABLES:

        //make the folder to put the outputss
        string output_dir = "example_generateParkProg_online";
        ForceDirectories(AnsiString(output_dir));

        //print the tittle
        unsigned int Bid = 0; //block identifier
        append("\r\nOnline generation example of a parking program for a CB"+inttostr(Bid)+":", log_filename.c_str());
        append("=========================================================", log_filename.c_str());

        //build the filename of reference (including the Bid)
        string filename = "example-"+inttostr(Bid)+".txt";

        //the filename will be used to attach the outputs with the input.

        //###################################################################
        //There are two ways to get a set of allocations:
        // 1. Load the allocations from a FMOSA file and get them from the MPG.
        // 2. Randomize the allocations without collisions.

        //-------------------------------------------------------------------
        //WAY 2: GETTING A SET OF ALLOCATIONS RANDOMIZING THEM WITHOUT COLLISIONS:

        //add to the MPG an allocation for each RP of the FMM
        TMotionProgramGenerator MPG(&FMM);
        for(int i=0; i<FMM.RPL.getCount(); i++)
            MPG.AddAllocation(i);

        //When you re-use the MPG, you need to do:
        //  Destroy(MPG);
        //before add allocation to it.

        //randomize the projection points in the domain of their attached RPs
        FMM.RPL.setPurpose(pGenParPro);
            append("FMM configured for generate a parking program. (Purpose = GenParPro).", log_filename.c_str());
        MPG.RandomizeWithoutCollision();
        append("Starting points randomized avoinding collisions.", log_filename.c_str());

        //Note that before randomize the allocations without collisions,
        //must be configured the FMM for generate a parking program.
        //If randomizing was for generate a pair (PP, DP), you must configure
        //the FMM for generate a pair (PP, DP) making:
        //  FMM.RPL.setPurpose(pGenPairPPDP);

        //save the allocation list
        string str = TAllocation::GetIdPPLabelsRow().str;
        str += "\r\n"+MPG.getColumnText().str;
        string output_filename = output_dir+"/AL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

        //###################################################################

        //-------------------------------------------------------------------
        //TEST THE FUNCTION FOR GENERATE PARKING PROGRAM ONLINE:

        //move the RPs to the more closer stable point to the projection points
        MPG.MoveToTargetP3();
        append("RPs moved to starting positions.", log_filename.c_str());

        //A PPA list shall be stored how a table (Id, p_1, p___3).

        //captures the starting positions of the RPs in a PPA list
        TPairPositionAnglesList SPL;
        FMM.RPL.getPositions(SPL);
        str = TActuator::getPositionPPALabelsRow().str;
        str += "\r\n"+SPL.getColumnText().str;
        ForceDirectories(AnsiString(output_dir));
        output_filename = output_dir+"/SPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Starting position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the starting position list directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //copy the position angles in the input parameters
        vector<double> p_1s, p___3s;
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];
            p_1s.push_back(RP->getActuator()->getp_1());
            p___3s.push_back(RP->getActuator()->getArm()->getp___3());
        }
        append("Input parameters extracted from the FMM. (p_1s, p___3s).", log_filename.c_str());

        //move the RPs to their origin positions
        FMM.RPL.moveToOrigins();
        append("RPs moved to origins.", log_filename.c_str());

        //Move the FMM to the origins is convenient because this function is
        //for test the generating function, but it is not really necessary.

        //determine the RPs to be disabled
        vector<int> Ids;
        //TBD
        append("List of Ids of RPs to be disabled must match with the real Fiber MOS.", log_filename.c_str());

        //When you re-use the Ids, you need to do:
        //  Ids.clear();
        //before determine the RPs to be disabled.

        //The RPs that shall be disabled depend of the status of the RPs
        //of the real Fiber MOS.

        //call the function to test
        TMotionProgram ParkProg;
        append("Calling function generateParkProg_online...", log_filename.c_str());
        append("----------------------------------------------------------------", log_filename.c_str());
        append("ParkProgValid = generateParkProg_online(ParkProg,", log_filename.c_str());
        append("                                        FMM, p_1s, p___3s, Ids);", log_filename.c_str());
        bool ParkProgValid = generateParkProg_online(ParkProg,
                                                     FMM, p_1s, p___3s, Ids);
        append("----------------------------------------------------------------", log_filename.c_str());
        append("Returned from function generateParkProg_online.", log_filename.c_str());

        //When you re-use the MP, you need to do:
        //  ParkProg.clear();
        //before call the generation function.

        //WARNING: function generateParkProg_online not return the lists Collided and Obstructed.
        //So it is possible that the function return true, but there are RPs collided or obstructed.
        //in these case, these RPs will not be included in the parking program.
        //Inputs and outputs parameters of the function generateParkProg_online,
        //was stablished by the programmer of the MCS, which was warned about this circunstance.

        //determine the list of RPs excluded in the parking program
        //and the list of Ids excluded from the pair (PP, DP)
        TRoboticPositionerList Included;
        getRPsIncludedInMP(Included, ParkProg, &FMM);
        TVector<int> Excluded;
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];
            int j = Included.Search(RP);
            if(j >= Included.getCount())
                Excluded.Add(RP->getActuator()->getId());
        }

        //THE DISABLED RPs WILL STAY DISABLED IN THE FMM.

        //restore de enabling status of all RPs
        for(unsigned int i=0; i<Ids.size(); i++) {
            int Id = Ids[i];
            int j = FMM.RPL.searchId(Id);
            if(j >= FMM.RPL.getCount())
                throw EImpossibleError("lateral effect");
            FMM.RPL[j]->Disabled = false;
        }

        //Here are re-enabled the RPs for ilustrate the modo to do this,
        //but in MCS, RPs not should be re-enabled. Instead of that,
        //the new status of the RPs should be indicated in the FMM instance,
        //(writing Disabled = true), because the instance will be re-loaded
        //in case of fault of power supply.

        //---------------------------------------------------------------
        //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

        //You need to know that the generated MPs are in the format of the FMPT.
        //So you may access to the information from each MI of each generated MP,
        //and you may translate each MP to the format of the MCS.
        //Due to the format of the MCS requires the coordinates even the rotors
        //that aren't moved, the translation need the list of initial positions
        //(the SPL for the ParkProg).

        //if generation function was successfully generated
        if(ParkProgValid) {

            //access to each message instruction of the ParkProg
            for(int i=0; i<ParkProg.getCount(); i++) {
                TMessageList *ML = ParkProg.GetPointer(i);
                for(int j=0; j<ML->getCount(); j++) {
                    //point the indicated message instruction
                    TMessageInstruction *MI = ML->GetPointer(j);

                    //get the information from the message instruction
                    int Id = MI->getId();
                    TInstruction I = MI->Instruction;
                    string name = I.getName().str;                    
                    if(name == "M1") { //if the instruction move the rotor 1
                        double p_1 = I.Args[0];
                        //here is possible use the properties (Id, p_1) of the MI (i, j)
                    }                    
                    else if(name == "M2") { //if the instruction move the rotor 2
                        double p___3 = I.Args[0];
                        //here is possible use the properties (Id, p___3) of the MI (i, j)
                    }                    
                    else if(name == "MM") { //if the instruction move both rotors
                        double p_1 = I.Args[0];
                        double p___3 = I.Args[1];
                        //here is possible use the properties (Id, p_1, p___3) of the MI (i, j)
                    }
                    else
                        throw EImpossibleError("lateral effect");
                }
            }

            //indicates the result of the generation
            append("Generated parking program is valid.", log_filename.c_str());

            //save the ParkProg in the format of the FMPT
            str = ParkProg.getText().str;
            output_filename = output_dir+"/ParkProg-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in the format of the FMPT saved in '"+output_filename+"'.", log_filename.c_str());

            //Given that here the generated parking program is valid,
            //all operative outsider RPs which aren't obstructed, should be:
            //- in the final positions.

            //captures the final positions of the RPs in a PPA list
            TPairPositionAnglesList FPL;
            FMM.RPL.getPositions(FPL);
            string str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+FPL.getColumnText().str;
            string output_filename = output_dir+"/FPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());

            //Other whay to obtain the final position table directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //translates the parking program to the format of the MCS
            //and save it in a file
            ParkProg.getInterfaceText(str, "depos", Bid, SPL, true);
            append("Parking program translated to the format of the MCS.", log_filename.c_str());
            output_filename = output_dir+"/ParkProg-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in the format of the MCS saved in '"+output_filename+"'.", log_filename.c_str());
        }
        else {
            //Given that here the generated parking program is invalid,
            //all operative outsider RPs which aren't obstructed, should be:
            //- in the first position where the collision was detected.
            //During the test of ParkProg.

            //print the result of generation of the PP
            append("Generated parking program is invalid.", log_filename.c_str());
        }

        //Below alll instruction about Obstructed or Collided has been comented:
        //
        //if(Obstructed.getCount() > 0)
        //    append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());
        //
        //if(Collided.getCount() > 0)
        //    append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "ParkProgValid: "+BoolToStr(ParkProgValid, true).str;
        //str += "\r\nCollided: "+Collided.getText().str;                         //not returned by the function online
        //str += "\r\nObstructed: "+Obstructed.getText().str;                     //not returned by the function online
        str += "\r\nExcluded: "+Excluded.getText().str;
        if(ParkProg.thereIsSomeComment1())
            str += "\r\nParkProg comments:\r\n"+ParkProg.getComment1sColumnText();
        output_filename = output_dir+"/other-outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

        //END OF SAVE THE OUTPUTS

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the generating example has finished
        append("\r\nFinished online generation example of a parking program for CB"+inttostr(Bid)+"!", log_filename.c_str());
        append("ParkProgValid: "+BoolToStr(ParkProgValid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());     //not returned by the function online
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str()); //not returned by the function online
        append("Excluded: "+Excluded.getText().str, log_filename.c_str());
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        append("ERROR: "+E.Message.str, log_filename.c_str());
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknown exception
        append("ERROR: unknown exception", log_filename.c_str());
        return 2;
    }

    //-----------------------------------------------------------------------

    //indicates that the program has been executed without error
    return a.exec();
}
