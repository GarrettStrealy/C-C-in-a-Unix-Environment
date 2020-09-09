/*
 * Filename     convertToString.cc
 * Date         09/06/2020
 * Author       Garrett Strealy
 * Email        GJS160430@utdallas.edu
 * Course       CS 3377.001 Fall 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 
 * Description
 *    Converts boolean value to a string. 1 returns "true" -
 *    and 0 returns "false" 
*/
#include <string>
#include "header.h"

using namespace std;

string convertToString(bool a){
  if (a == 1)
  { 
    return "true";
  }
  return "false";
}
