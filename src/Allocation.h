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
//File: Allocation.h
//Content: RP to projection point allocation
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TALLOCATION_H
#define TALLOCATION_H

#include "FiberMOSModel.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//spacename for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TAllocation:
//---------------------------------------------------------------------------

//class RP to projection point allocation
class TAllocation {
        //STATIC PROPERTIES:

        //list of pointers to built allocations
        static TItemsList<TAllocation*> Builts;

        //Allows to control the construction of a single allocation by RP,
        //and the destruction of existing allocations.

        //DYNAMIC PROPERTIES:

        TRoboticPositioner *p_RP;

public:
        //DYNAMIC PROPERTIES:

        //pointer to the allocated RP
        TRoboticPositioner *getRP(void) const {return p_RP;}

        //projection point allocated to the RP,
        //where shall be positioned the point P3 of the attached RP
        TProjectionPoint PP;

        //The target point is the more close stable point to
        //the projection point.

        //PROPERTIES IN TEXT FORMAT:

        //PP in text format
        AnsiString getPPText(void) const;
        void setPPText(const AnsiString&);
        AnsiString getPPRowText(void) const;

        //SETS OF PROPERTIES IN TEXT FORMAT:

        //allocation in text format
        AnsiString getText(void) const;
        AnsiString getRowText(void) const;

        //------------------------------------------------------------------
        //STATIC METHODS:

        //compare the identifiers of the RPs attached to two allocations
        static int  CompareIds(TAllocation *TPA1, TAllocation *TPA2);
        //this method shall be pointed in a pointer list
        //to allow the functioning of shorting and comparing methods

        //get the labels of the properties
        //in row text format
        static AnsiString GetIdPPLabelsRow(void);

        //travel the labels of the properties
        //in a text string from the position i
        static void  TravelLabels(const AnsiString& S, int& i);

        //read the values of the properties
        //in a text string from the position i
        static void  ReadSeparated(int& Id, double& x, double& y,
                                   const AnsiString& S, int& i);

        //print the properties of an allocation in a string
        //in row format
        static void  PrintRow(AnsiString &S, TAllocation *A);

        //BUILDING AND DESTROYING METHODS:

        //build an allocation attached a RP
        //if the RP already has an attached allocation
        //  throw an exception EImproperArgument
        TAllocation(TRoboticPositioner  *RP, double x, double y);
        TAllocation(TRoboticPositioner  *RP, TDoublePoint PP);

        //destroy a TAllocation
        //if thereisn't a built allocation
        //  throw an exception EImproperCall
        ~TAllocation();

        //CHECKING METHODS:

        //determines if the target point is out of the domain
        //of the point P3 of the attached RP
        bool IsOutDomainP3(void);
        //determines if the target point is in the secure area
        //of the point P3 of the attached RP
        bool IsInSafeAreaP3(void);

        //MOTION METHODS:

        //assign the point P3o of the attacheed RP to the point PP
        void SetP3o(void) {PP = getRP()->getActuator()->getP3o();}
        //assign the point P3 of the attacheed RP to the point PP
        void SetP3(void) {PP = getRP()->getActuator()->getArm()->getP3();}
        //randomize the point PP with uniform distribution
        //in the domain of the point P3 of its attached RP
        void RandomizePP(void);

        //There are three ways to randomize a point in the domain of a RP:
        //  - Randomizing the angular positions of their rotors in their
        //    respective domains;
        //  - Randomizing the point in polar coordinates respect S1 with
        //    uniform distribution in the radial interval. Iterating the
        //    process until the point is in the domain of the RP.
        //  - Randomizing the point in Cartesian coordinates respect S0,
        //    in the Cartesian insterval of the domain of the RP, until
        //    thepoint is inthedomain of the RP.
        //Themethod Ramize, implement the las way.

        //assign the target point to the point P3 of its attached RP
        //and return the distance from the target point to the projection point
        //if the the projection point isn't on the domain of its attached RP:
        //  throw an exception EImpropercall
        double MoveToPP(void);

        //NOTE: method MoveToPP will move the rotors of the RP to the
        //positions corresponding to the P3 is positioned on the PP.
        //When the quantification of the rotors is enabled, will be quantified
        //first the rotor 1, and after the rotor 2, in any case, uncheking
        //which is the neares stable point.
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // TALLOCATION_H
