/*
 * Filename     toUppercase.cc
 * Date         09/06/2020
 * Author       Garrett Strealy
 * Email        GJS160430@utdallas.edu
 * Course       CS 3377.001 Fall 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved

 * Description
 *    Takes a vector of chars as an argument, and returns -
 *    a string of all of those chars converted to uppercase
 */
#include <iostream>
#include <vector>
#include <string>
#include "header.h"

std::string toUppercase(std::vector <char> chars)
{
  std::string uppercase = "";
  
  // Fill string  with chars converted to uppercase
  int i = 0;
  for (;i < chars.size(); i++)
  {
    uppercase += toupper(chars[i]);
  }

  return uppercase;
}
