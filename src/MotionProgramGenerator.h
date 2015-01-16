//---------------------------------------------------------------------------
//File: MotionProgramGenerator.h
//Content: generator of motionprograms
//Last update: 26/12/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMGENERATOR_H
#define MOTIONPROGRAMGENERATOR_H

#include "MotionProgramValidator.h"
#include "TargetPointList.h"
#include "FiberMOSModel2.h"
#include "MotionProgram.h"

//---------------------------------------------------------------------------

using namespace Models;

//namespace for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//A MPG (Motion Program Generator) provide functions to generates MPs (motion
//programs). For this, the MPG shall be use the Fiber MOS Model to perform
//simulations. All MPs  generated by the MPG will be valid, is to say they
//will avoid dynamic collisions. For accomplish of this purpose the MPG
//will make use of the validator function, to assure that the generation
//process is valid. Each MP shall go with an IPL (Initial Position List)
//of type TPairsPositionAngleList.

//class motion program generator
class TMotionProgramGenerator : public TMotionProgramValidator,
        public TTargetPointList {
protected:
    //SETTING PARAMETERS OF THE ALGORITHMS:

    double __dt_jmp;

public:
    //SETTING PROPERTIES:

    //jump (in negative direction) in radians, it is performed each time
    //that a collission is detected during the radial retraction
    //shall be upper zero
    //default value: -M_PI/36 rad
    double getdt_jmp(void) const {return __dt_jmp;}

    //It is desirable that the jump is expressed in radians and not in steps,
    //for several reasons:
    // - If the number of steps change, the jump not should vary.
    // - The rotor position and their domain limits, are stored in radians
    //   so that the checking belonging to domain, could introduce numerical
    //   errors.

    //Note that when the jump correspond to a integer number of steps, when
    //quantification is enabled, the rotors do not change their positions.

    //-----------------------------------------------------------------------
    //BUILDING AND DESTROYING METHODS:

    //build a motion program generator
    //attached to a robotic positionerlist
    TMotionProgramGenerator(TFiberMOSModel *FiberMOSModel);
    //destroy the targetpoints and destroy the MPG
    ~TMotionProgramGenerator(void);

    //-----------------------------------------------------------------------
    //METHODS TO GENERATE MPs:

    //Programs the retraction of all RP of the list Outsiders_.
    //Preconditions:
    //  All RPs of the list Outsiders_ shall be in unsecurity position.
    //  All RPs of the list Outsiders_ shall has enabled the quantifiers.
    void programRetraction(TRoboticPositionerList &Outsiders_);

    //Segregate the RPs of the list Outsiders, in disjoint subsets.
    //Preconditions:
    //  All RPs in the list Outsiders shall be operatives
    //  All RPs in the list Outsiders shall be in unsecure positions
    void segregateRPsInDisjointSets(
            TPointersList<TRoboticPositionerList>& DisjointSets,
            const TRoboticPositionerList& Outsiders);

    //Determines if not all RPs of a list follow the MEGARA distribution.
    //Preconditions:
    //  All RPs of the disjoint set shall be separated
    //  a distance D - 2*L or upper.
    bool notAllRPsFollowMEGARADistribution(
            const TRoboticPositionerList& Set);

    //Segregates the RPs of a set, in disperse subsets.
    //Preconditions:
    //  Pointer Subsets should point to built list of disperse subsets.
    //  All RPs of the list Set shall follow the MEGARA distribution.
    //  All RPs in the list Set shall be in the Fiber MOS Model.
    void segregateRPsInDisperseSubsets(
            TPointersList<TRoboticPositionerList > *Subsets,
            TRoboticPositionerList& Set);

    //Determines the limits of the open interval (l1, l2)
    //where RP collide with RPA.
    //Inputs:
    //  RP: robotic positioner which rotor 1 shall be displaced.
    //  RPA: roboticpositioner adjacentto RP which shall remains stoped.
    //Outputs:
    //  l1: lower limit of interval (l1, l2)
    //  l2: upper limit of interval (l1, l2)
    //Preconditions:
    //  RP shall beenabled their rotor 1 quantifier.
    //Posconditions:
    //  Robotic positioner RP and RPA remains without change,
    //      and the limits l1and l2 has been determined.
    //Meaning:
    //  When l1 == l2 there isn't collision. (In this casemoreover l1 == p1 ==l2).
    //  When l1 < l2, the soltion can be searched in both senses.
    //  When l2 < l1, the solution shall be searched:
    //      in positive sense, when p_1 is in [p_1min, l2)
    //      in negative sense, when p_1 is in (l1, p_1max]
    void determinesCollisionInterval(double& l1, double& l2,
            TRoboticPositioner *RP, const TRoboticPositioner *RPA);

    //Determines if a RP in collision status can solve
    //their collision turning the rotor 1,
    //and calculates the new position of the rotor 1.
    //Inputs:
    //  RP: the robotic positioner in collission status.
    //Onputs:
    //- collisionCanbesolved: flag indicating if collision can be solved
    //  turning the rotor 1.
    //- p_1new: the position which the rotor 1 of the RP shall jump
    //  to solve the collision.
    //Preconditions:
    //  Pointer RP shall  point to built robotic positioner.
    //  The RP shall be in collision sattus with one or more adjacent RPs.
    //  The RP shall has enabled their rotor 1 quantifier.
    //  The rotor 1 of the RP shall has stacked one position almost.
    //Posconditions:
    //  Te RP will be in their initial status, and the proposed quastion
    //  will be solved.
    bool collisionCanBesolved(double& p_1new, TRoboticPositioner *RP);

    //Determines the RPs which can be retracted in each subset of each set.
    //Inputs:
    //  DDS: structure to contains disjoint disperse subsets
    //Outputs:
    //  RetractilesDDS: structure to contains the RPs which can be retracted
    //  in one or two gestures avoiding dynamic collisions.
    //  InvadersDDS: structure to contains the RPs which can not be retracted
    //  in one or two gestures avoiding dynamic collisions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model shall be configurated to
    //  motion program generation.
    //  All RPs of DDS shall be in the Fiber MOS  Model
    //  All RPs of DDS shall be in insecurity position
    //  All RPs of DDS shall have enabled the quantifiers
    //  All RPs of DDS shall be free of dynamic collisions
    //  All RPs of DDS shall to have programmed a gesture of retraction
    //Posconditions:
    //  RetractilesDDS will contains the RPs which can be retracted
    //  in one or two gestures.
    //  InvadersDDS will contains the RPs which can not be retracted
    //  in one or two gestures.
    void segregateRetractilesInvaders(
      TPointersList<TPointersList<TRoboticPositionerList> >& RetractilesDDS,
      TPointersList<TPointersList<TRoboticPositionerList> >& InvadersDDS,
      TPointersList<TPointersList<TRoboticPositionerList> >& DDS);

    //get and add to the DP, the corresponding list or lists
    //of message of instructions
    //Inputs:
    //  RPsToBeRetracted: list of the RPs which has been retracted.
    //  DP: depositioning program to bemodified.
    //Outputs:
    //  DP: depositioning program modified.
    //Preconditions:
    //  All RPs of the list RPsToBeRetracted shall have stacked the initial
    //      positions of their rotors.
    //  All RPs of the list RPsToBeRetracted shall be in security positions.
    void getTheMessageInstructionLists(TMotionProgram& DP,
            const TRoboticPositionerList& RPsToBeRetracted);

    //Generates a DP for a given set of operative RPs in unsecurity
    //positions and determines the RPs of the given set,
    //which can not be recovered because are in collision status
    //or because are obstructed in unsecurity positions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their initial positions.
    //  All RPs of the list Outsidersm shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecurity positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.
    //Inputs:
    //  FiberMOSModel: Fiber MOS Model with RPs in their initial positions.
    //  Outsiders: list of operative RPs in insecurity positions which
    //      we want recover the security positions.
    //Outputs:
    //  generateDepositioningProgram: flag indicating if the DP can be
    //      generated or not with this function.
    //  Collided: list of RPs collided in unsecurity position.
    //  Obstructed: list of RPs obstructed in unsecurity position.
    //  DP: depositioning program.
    bool generateDepositioningProgram(TRoboticPositionerList& Collided,
        TRoboticPositionerList& Obstructed, TMotionProgram& DP,
        const TRoboticPositionerList& Outsiders);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMGENERATOR_H
