// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: MotionProgramValidator.h
//Content: validator of motionprograms
//Last update: 26/12/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMVALIDATOR_H
#define MOTIONPROGRAMVALIDATOR_H

#include "FiberMOSModel2.h"
#include "MotionProgram.h"

using namespace Models;

//--------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramValidator:
//###########################################################################

//class validator of motion programs
class TMotionProgramValidator {
protected:
    //EXTERN-ATTACHED OBJECTS:

    TFiberMOSModel *__FiberMOSModel;

public:
    //EXTERN-ATTACHED OBJECTS:

    //extern attached Fiber MOS Model
    TFiberMOSModel *getFiberMOSModel(void) const {
        return __FiberMOSModel;}

    //BUILDING AND DESTROYING METHODS:

    //built a validator of motion programs
    //attached to an extern Fiber MOS Model
    TMotionProgramValidator(TFiberMOSModel *_FiberMOSModel);

    //METHODS TO VALIDATE MOTION PROGRAMS:

    //DEFINITION: minimun free distance between two barriers (Dfmin)
    //is the distance between the barriers less the SPM of each:
    //  Dfmin = Barrier1->Contour.DistanceMin(Barrier2->Contour) -
    //      Barrier1->SPM - Barrier2->SPM.
    //Each barrier can be an arm or to be part of an exclusion area.

    //DEFINITION: minimun free time between two barriers (Tfmin)
    //is displacement time during which there is garantee
    //that cannot collide.

    //calculates the time free of collission of two RPs
    double calculateTf(const TRoboticPositioner *RP,
                       const TRoboticPositioner *RPA) const;
    //calculates the minimun step time of two RPs
    double calculateTmin(const TRoboticPositioner *RP,
                         const TRoboticPositioner *RPA) const;

    //calculates the minimun time free of collission of
    //a RP with their adjacents
    double calculateTfmin(const TRoboticPositioner *RP) const;
    //calculates the minimun step time of
    //a RP with their adjacents
    double calculateTminmin(const TRoboticPositioner *RP) const;

    //calculates the minimun time free of collission of
    //the RPs of a list
    double calculateTfmin(const TRoboticPositionerList& RPL) const;
    //calculates the minimun step time of
    //the RPs of a list
    double calculateTminmin(const TRoboticPositionerList& RPL) const;

    //Get the list of RPs includes in a MP.
    //Precondition:
    //  All message of instruction in the MP shall be addressed
    //  to an existent RP of the Fiber MOS Model.
    void getRPsIncludedInMP(TRoboticPositionerList& RPL,
                            const TMotionProgram& MP) const;

    //Determines if the execution of a motion program, starting from
    //given initial positions, avoid collisions.
    //Preconditions:
    //  All RPs included in the MP:
    //      must be enabled the quantifiers of their rotors.
    //  All RPs of the FMM:
    //      shall be in their initial positions;
    //      must have the aadecuate SPM.
    //Postconditions:
    //  If the MP produces a collision, all RPs of the FMM:
    //      will have disabled the quantifiers of their rotors.
    //      will be in the firstposition where collission was detected.
    //  If the MP avoid collisions, all RPs of the FMM:
    //      will have the quantifiers of their rotors in their initial status,
    //      sill be in their final positions.
    //Inputs:
    //  MP: motion program to be validated
    //Outputs:
    //  motionProgramIsValid: flag indicating if the motion program
    //      avoid collisions.
    //Notes:
    //  The validation process of a MP consume a component of the SPM, even
    //  when the process is successfully passed. So if a MP pass the validation
    //  process with a value of SPM, the validation shall be make with
    //  the value of SPM inmediately lower.
    //  The validation method of a MP will be used during the generation process
    //  with the individual MP of each RP, and at the end of the process for
    //  validate the generated recovery program.
    bool motionProgramIsValid(const TMotionProgram &MP) const;

    //Validation process can end of two ways:
    //- If the motion program not produce a dynamic collision, being all RPs
    //  in their final positions.
    //- If the motion program produce a dynamic collisions, being all RPs
    //  in the firs position where collision has been detected.
};

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMVALIDATOR_H
