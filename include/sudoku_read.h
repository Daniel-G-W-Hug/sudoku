// let emacs know this is a C++ header: -*- C++ -*-

#pragma once

#include <iostream>

using namespace std;

void skip_to_int() // modified routine from "Programming, Principles and
                   // Practice", p. 355
{
  if (cin.fail()) { // found something that was not an integer
    cin.clear();    // clear flags, we'd like to look at the characters
    char ch;
    while (cin >> ch) { // throw away non-digits and comment lines
      if (ch == '#') {
        cin.unget(); // ignore comments starting with # to end of line
        string line;
        getline(cin, line);
      }
      if (isdigit(ch)) {
        cin.unget(); // put back the digit so that it can be read via cin
        return;
      }
    }
    cout << "ERROR: bad input (e.g. EOF or file corrupt)\n\n"; // eof or bad:
                                                               // give up
  }
}

int read_int() {
  int int_var = 0;
  if (!(cin >> int_var)) {
    skip_to_int();
    cin >> int_var;
  }
  return int_var;
}
