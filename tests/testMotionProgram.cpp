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
//File: testMotionProgram.cpp
//Content: test for the module MotionProgram
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testMotionProgram.h"
#include "../src/MotionProgram.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestMotionProgram);

//overide setUp(), init data, etc
void TestMotionProgram::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestMotionProgram::tearDown() {
}

//---------------------------------------------------------------------------

void TestMotionProgram::test_()
{
    CPPUNIT_ASSERT(true);
}

