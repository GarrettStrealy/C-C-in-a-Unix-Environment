/*  Filename      main.cc
    Date          09/24/2020
    Author        Garrett Strealy
    Email         GJS160430@utdallas.edu
    Course        CS 3377.001 Fall 2020
    Version       1.0
    Copyright     2020, All Rights Reserved

    Description:
        call your gawk program and collect/display the
        results of the gawk output
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
  // buffer for reading
  char buff[1024];
  // output file
  ofstream outfile;
  outfile.open ("output.txt");
  
  FILE *inpipe = popen("gawk --version", "r");

  if(!inpipe)
    {
      outfile << "Error creating pipe stream\n";
      return -1;
    }

  // write inpipe to outfile
  while(fgets(buff, 1024, inpipe))
    {
      outfile << buff;
    }
  outfile << "---------------------------------------------\n";
  // close pipe stream
  pclose(inpipe);

  // open pipe stream again
  inpipe = popen("gawk -f gawk.code numbers.txt", "r");

  if(inpipe == NULL)
    {
      outfile << "Error creating pipe stream\n";
      return -1;
    }

  if(fgets(buff, 1024, inpipe) == NULL)
    {
      outfile << "Error receiving data on pipe\n";
      pclose(inpipe);
      return -3;
    }

  // output has been read into buff, close pipe stream
  pclose(inpipe);

  // parse output
  istringstream istream(buff);

  // extract from buff
  int sum1, sum4;
  istream >> sum1 >> sum4;

  // print output
  outfile << "\nThe second call to gawk returned: " << sum1 << " " << sum4 << endl;
  outfile << "\nThe sum of Column 1 is: " << sum1 << endl;
  outfile << "The sum of Column 4 is: " << sum4 << endl;
  sum1 = sum1 + sum4;
  outfile << "The sum of the two numbers is: " << sum1 << endl;

  outfile.close();
  return 0;
}
