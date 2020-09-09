/*
 * Filename     toLowercase.cc
 * Date         09/06/2020
 * Author       Garrett Strealy
 * Email        GJS160430@utdallas.edu
 * Course       CS 3377.001 Fall 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved

 * Description
 *    Takes a vector of chars as an argument, and returns -
 *    a string of all of those chars converted to lowercase
 */

#include <iostream>
#include <vector>
#include <string>
#include "header.h"

std::string toLowercase(std::vector <char> chars)
{
  std::string lowercase = "";

  // Fill string  with chars converted to lowercase
  int i = 0; 
  for (;i < chars.size(); i++)
  {
    lowercase += tolower(chars[i]);
  }

  return lowercase;
}
