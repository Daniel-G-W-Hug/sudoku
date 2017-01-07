// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream>
#include <string>
#include <exception>        // terminate()

using namespace std;

void dynamic_assert(bool assertion, string message);
