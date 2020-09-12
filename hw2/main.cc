/*
 * Filename     main.cc
 * Date         09/06/2020
 * Author       Garrett Strealy
 * Email        GJS160430@utdallas.edu
 * Course       CS 3377.001 Fall 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved

 * Description
 *   Opens an input file for reading and output file for writing.
 *   Copies all data from the input file to the output file while -
 *   optionally converting case as directed by the command line args.
 *   -u and -l are mutually exclusive. Only one may be given but -
 *   neither is required. If the output file is not specified, writes -
 *   to a file with the default name off "output.txt"
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <tclap/CmdLine.h>
#include "header.h"

using namespace std;

int main(int argc, char *argv[])
{
  try{
    // Define the command line object
    TCLAP::CmdLine cmd("CS 3377.001 Program 2", ' ', "1.0");
    // Define SwitchArg to set all text to lowercase
    TCLAP::SwitchArg lowercaseSwitch("l","lower","Convert all text to lowercase", cmd,  false);
    // Define SwitchArg to set all text to uppercase
    TCLAP::SwitchArg uppercaseSwitch("u","upper","Convert all text to uppercase", cmd, false);
    // Define ValueArg to write output to a file
    TCLAP::ValueArg<std::string> outfileArg("o","outfile","The name of the output file",false,"output.txt","output filename");
    // Define UnlabledValueArg for taking input file name
    TCLAP::UnlabeledValueArg<std::string> infileArg("infile", "(required) Input file", true, "infile.txt", "input filename", false);

    // Add the argument outfileArg to the CmdLine object
    cmd.add(outfileArg);
    // Add the argument infileArg to the CmdLine object
    cmd.add(infileArg);
    
    // Parse the argv array
    cmd.parse(argc, argv);
    
    // Fill map with parsed data
    map<int, string> optionMap;
    enum names { LOWER, UPPER, OUTFILE, INFILE }; 
      optionMap[LOWER] = convertToString(lowercaseSwitch.getValue());
      optionMap[UPPER] = convertToString(uppercaseSwitch.getValue());
      optionMap[OUTFILE] = outfileArg.getValue();
      optionMap[INFILE] = infileArg.getValue();   
      
    // Print error message and exit if both -u and -l commands are given
    if (optionMap[LOWER].compare("true") == 0 && optionMap[UPPER].compare("true") == 0)
    {
      cout << "error: Only one of -l or -u commands may be given, not both." << endl;
      return 0;
    }
    
    // Open input file for reading
    ifstream inputFile;
    inputFile.open(optionMap[INFILE].c_str());
    // Open outputFile for writing
    ofstream outputFile(optionMap[OUTFILE].c_str());
    
    // char array to hold file input
    vector<char> inputArr;
    // string to hold file output
    string output;
    
    // Neither -u or -l commands given
    if (optionMap[LOWER].compare("true") != 0 && optionMap[UPPER].compare("true") != 0)
    {
      char c;
      while(inputFile.get(c))
      {
        output += c;
      }

      inputFile.close();
      outputFile << output << endl;
      return 0;
    }

    // -l command given
    else if (optionMap[LOWER].compare("true") == 0)
    {
	char c;
	while(inputFile.get(c))
        {
          inputArr.push_back(c);
        }

	inputFile.close();
	output = toLowercase(inputArr);
	outputFile << output << endl;
	return 0;
    }

    // -u command given
    else
    {
      char c;
      while(inputFile.get(c))
      {
        inputArr.push_back(c);
      }
      inputFile.close();
      output = toUppercase(inputArr);
      outputFile << output << endl;
      return 0;
    }

  } catch (TCLAP::ArgException &e)  // catch any exceptions
    { cerr << "error: " << e.error() << " for arg " << e.argId() << endl; }
}
