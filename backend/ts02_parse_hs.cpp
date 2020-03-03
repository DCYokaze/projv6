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

string exportObjbyteToPcd(string &sin)  // export all of them
{
  std::istringstream f(sin);
  string longLine = "";  // i++;
  string line;
  while (std::getline(f, line)) {
    if (line[0] == ' ') {  // found space
      int firstSpace = line.substr(1).find(" ");
      string lineProcess1 = line.substr(firstSpace + 2);
      string lineProcess2 = lineProcess1.substr(0, 35);
      longLine += lineProcess2;
    }
  }
  if (longLine.length() < 1) {
    // cout<<("something wrong with "+fileNameObjbyte)<<endl;
  }
  longLine.erase(std::remove_if(longLine.begin(), longLine.end(),
                                (int (*)(int))std::isspace),
                 longLine.end());
  return longLine;
}

int basicCount(int bytes, string &longLine) {
  int freq = 0;
  for (int i = 0; i <= longLine.length() - (bytes * 2); i = i + 2) {
    string s = longLine.substr(i, (bytes * 2));
    long l = std::strtol(s.c_str(), NULL, 16);
    // cout<< "from "<<
    // if(s.compare(attr)==0)
    //   freq++;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  time_t start, end;

  time(&start);

  for (int bytes = 2; bytes <= 4; bytes++) {
    // objdump -s for hex
    ifstream iFile;
    time(&start);
    iFile.open("listdataset_raw");
    if (iFile.is_open()) {
      string fileName;
      while (getline(iFile, fileName)) {
        // cout<<line<<endl;
        string command = "objdump -s " + fileName;
        string objdumpCommRes = commonLib::exec(command);
        string longLine = exportObjbyteToPcd(objdumpCommRes);
        basicCount(bytes, longLine);  // change the bytes here.
      }
    } else {
      cout << "cannot open file" << endl;
    }
    iFile.close();
    time(&end);
    double dif = difftime(end, start);
    printf("Elasped time is %.2lf seconds.", dif);
  }
  return 0;
}
