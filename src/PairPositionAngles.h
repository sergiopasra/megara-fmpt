// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//File: PairPositionAngles.h
//Content: pair of projection angles
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PAIRPOSITIONANGLES_H
#define PAIRPOSITIONANGLES_H

#include "ProjectionPoint.h"
#include "Figure.h"
#include "PointersList.h"
#include "Vector.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TPairPositionAngles
//---------------------------------------------------------------------------

//predeclares classes to avoid loops
class TRoboticPositioner;

//class pair position angles
class TPairPositionAngles {
protected:
    //ATTACHED EXTERN OBJECTS:

    TRoboticPositioner *p_RP;
    TProjectionPoint *p_PP;

public:
    //SATIC PROPERTIES:

    //label of property Id
    //default value: "Id"
    static AnsiString IdLabel;
    //label of property p_1
    //default value: "p_1"
    static AnsiString p_1Label;
    //label of property p___3
    //default value: "p___3"
    static AnsiString p___3Label;

    //ATTACHED EXTERN OBJECTS:

    //extern attached robotic positioner
    //default value: NULL
    TRoboticPositioner *getRP(void) const {return p_RP;}
    //extern attached projection point
    //default value: NULL
    TProjectionPoint *getPP(void) const {return p_PP;}

    //PROPERTIES:

    //single identification number of the attached robotic positioner
    //  RoboticPositioner->Id
    int getId(void) const;

    //position angle for rotor 1
    double p_1;
    //position angle for rotor 2
    double p___3;

    //PROPERTIES IN TEXT FORMAT:

    AnsiString getIdText(void) const;
    AnsiString getp_1Text(void) const; void setp_1Text(const AnsiString&);
    AnsiString getp___3Text(void) const; void setp___3Text(const AnsiString&);

    //SETS OF PROPERTIES IN TEXT FORMAT:

    //values of all properties in row fortam
    AnsiString getRowText(void) const; void setRowText(const AnsiString&);

    //STATIC METHODS:

    //labels of all properties in a row:
    //  IdLabel+"\t"+p_1Label+"\t"+p___3Label
    static AnsiString getLabelsRow(void);
    //travel the labels of the properties
    //in row text format
    static void travelLabelsRow(const AnsiString& S, int& i);

    //print the properties of a pair of position angles in a string
    //in row format
    static void  printRow(AnsiString &S,
                          const TPairPositionAngles *PPA);
    //read the properties of a pair of position angles in a string
    //in row format
    static void  readRow(TPairPositionAngles *PPA,
                         const AnsiString &S, int &i);

    //PUBLIC METHODS:

    //build a PPA attached to a RP
    //pointer RP can be null
    TPairPositionAngles(TRoboticPositioner *RP= NULL);

    //copy all properties of a PPA
    void clone(TPairPositionAngles*);
    TPairPositionAngles &operator=(const TPairPositionAngles&);

    //build a clon a PPA
    TPairPositionAngles(TPairPositionAngles*);

    //set the PPA
    void setPPA(double p_1, double p___3);

    //randomize the PPA
    void randomize(double p_1min, double p_1max,
                   double p___3min, double p___3max);
};

//---------------------------------------------------------------------------
//TPairPositionAnglesList
//---------------------------------------------------------------------------

//class list of PPAs
class TPairPositionAnglesList : public TPointersList<TPairPositionAngles> {
public:
    //PROPERTIES IN TEXT FORMAT:

    //PPA list in text format
    AnsiString getText(void); void setText(const AnsiString &);

    //WARNING: setText will build new PPAs without attached PP.

    //PUBLIC METHODS:

    //builds a PPA list
    TPairPositionAnglesList(void);

    //copy a PPA list
    TPairPositionAnglesList &operator=(const TPairPositionAnglesList&);

    //add the PPAs attached to the PPs of a PP list
//    void Build(TProjectionPointList&);

    //search the first PPA with a given identifier
    int SearchId(int Id) const;

    //randomize the PPAs of the list
    void Randomize(double p_1min, double p_1max,
                   double p___3min, double p___3max);

    //check if all PPAs are addresed to different RPs
    bool notAllPPAsAreAddresedToDifferentRPs(void) const;

    //check if all Ids of a list are in the PPA list
    bool notAllIdsAreFound(const TVector<int>& Ids) const;
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // PAIRPOSITIONANGLES_H
