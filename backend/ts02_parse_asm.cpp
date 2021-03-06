#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "commonLib.h"
#include "stdpatch.h"
// #include "getMemorySize.c"

// using namespace patch;
using namespace std;
int a1() { return 0; };

/*
NOTE STEP
0.extractAllFileToHEX got HEX (CASE 1)
1.featureSelectionToFile got fsarff
2.fsattrToarff got arff, and ready to go to weka.
*/

int main(int argc, char *argv[]) {
  time_t start, end;
  time(&start);

  // objdump -s for asm
  ifstream iFile;
  time(&start);
  iFile.open("listdataset_raw");
  if (iFile.is_open()) {
    string fileName;
    while (getline(iFile, fileName)) {
      // cout<<line<<endl;
      string command = "objdump -D " + fileName;
      string objdumpCommRes = commonLib::exec(command);
      string longLine = exportObjbyteToPcd(objdumpCommRes);
    }
  } else {
    cout << "cannot open file" << endl;
  }
  iFile.close();
  time(&end);
  double dif = difftime(end, start);
  printf("Elasped time is %.2lf seconds.", dif);

  return 0;
}
