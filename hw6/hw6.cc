/*
 * File:                hw6.cc
 * Date:                11/21/2020
 * Author:              Garrett Strealy
 * Email:               garrett.strealy@utdallas.edu
 * Version:             1.0
 * Copyright:           2020, All Rights Reserved
 *
 * Description:
 *
 *  Perform Binary File IO and display results
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <string>
#include <bits/stdc++.h>
#include <array>
#include "cdk.h"

#define MATRIX_ROWS 5
#define MATRIX_COLS 3
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxRecordStringLength = 25;

class BinaryFileHeader {
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

class BinaryFileRecord { 
public:
  uint8_t strLength;
  char stringBuffer [maxRecordStringLength];
};

int main() {

  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix 

  const char *rowTitles [MATRIX_ROWS+1] = {"IGNORE", "a", "b", "c", "d", "e"};
  const char *columnTitles [MATRIX_COLS+1] = {"IGNORE", "a", "b", "c"};
  int colWidths [MATRIX_COLS+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int boxTypes [MATRIX_COLS+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen (window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix = newCDKMatrix (cdkscreen, CENTER, CENTER, MATRIX_ROWS, MATRIX_COLS, MATRIX_ROWS, MATRIX_COLS,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, colWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix == NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  ifstream binInfile;
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  BinaryFileRecord *myRecord = new BinaryFileRecord();

  // open supplied binary file for reading
  binInfile.open ("/scratch/perkins/cs3377.bin", ios::in | ios::binary);

  if (!binInfile) {
  // An error occured.
    cout << "Error\n";
    return 0;
  }

  binInfile.read ((char *) myHeader, sizeof(myHeader));
  for (int i = 0; i < 4; i++) {
    binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));
  }

  binInfile.close();

  for (int i = 0; i < 4; i++) {
    cout << "strlength: " << unsigned(myRecord->strLength) << endl;
  }

  stringstream magicNumSS;
  magicNumSS << std::hex << std::uppercase << myHeader->magicNumber;
  string magicNumStr = magicNumSS.str();
  string magicNumStrFinal = "0x" + magicNumStr;
  string magicNumStrMatrix = "Magic: " + magicNumStrFinal;

  stringstream versionNumSS;
  versionNumSS << unsigned(myHeader->versionNumber);
  string versionNumStr = versionNumSS.str();
  string versionNumStrMatrix = "Version: " + versionNumStr;

  string numRecordsString = to_string(myHeader->numRecords);
  string numRecordsStrMatrix = "NumRecords: " + numRecordsString;

  /* Display the Matrix */
  setCDKMatrixCell (myMatrix, 1, 1, magicNumStrMatrix.c_str());
  setCDKMatrixCell (myMatrix, 1, 2, versionNumStrMatrix.c_str());
  setCDKMatrixCell (myMatrix, 1, 3, numRecordsStrMatrix.c_str());
  drawCDKMatrix (myMatrix, true);

  /* so we can see results */
  sleep (10);

  // Cleanup screen
  endCDK();

  return 0; 
}
























  



















































