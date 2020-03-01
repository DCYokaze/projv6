#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include "commonLib.h"
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include "HS.h"
#include "stdpatch.h"
#include <time.h>
#include <unistd.h>
#include <math.h>
using namespace patch;
using namespace std;
#define NUMCLASS0 3267
#define NUMCLASS1 3052
// const string MEMORY_FOR_JAVA = "-Xmx512m";
const string FS_preFix = "FS_Top50000_";
// const string MEMORY_FOR_JAVA = "-Xmx1024m";
const string MEMORY_FOR_JAVA = "-Xmx4g";
// const string MEMORY_FOR_JAVA = "-Xmx2048m";
// const string MEMORY_FOR_JAVA = "-Xmx4096m";
int HS::getFileList()
{
  //vector<string> vectorOfFile;
  if(vectorOfFile.size()>0)
  {
    cout<<"Clear vectorOfFile"<<endl;
    vectorOfFile.clear();
  }
  string line;
  ifstream myfile (config.fileList.c_str());
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      vectorOfFile.push_back(line);
      // cout << line << '\n';
    }
    myfile.close();
  }
  else 
    cout << "Unable to open file"; 
  return 0;
}


/*
 * ArrResult is the result.
 * bytes = length of each column, like FFFF will be 2
 * shift = each shift, normally multipled of 2
 * longLine = .... That long line from extraction. be an input here.
//*/
int HS::longHexToArray(int * ArrResult, int bytes, int shift, string longLine)
{
  //int* attrValueHex = new int[commonLib::powInt(256,bytes)];//to keep all value.
  char* c = new char[bytes*2+1];
  c[bytes*2]='\0';
  cout<<"longLine.length()-bytes = "<<longLine.length()-bytes<<endl;
  for(int i=0;i<(longLine.length()-(bytes*2)+1);i=i+shift)
  {
    for(int j=0;j<(bytes*2);j++)
    {
      c[j]=longLine.c_str()[i+j];
    }
    int idx = (int)strtol(c, NULL, 16);
    cout<<"i="<<i<<endl;
    ArrResult[idx]++;
  }
  // cout<<"_"<<attrValueHex[0]<<"_"<<endl;
  delete[] c;
  return 0;//attrValueHex;
}

//Extract02
/*
int genARFFFile2(string outName,int b)
{
  
  
}
//*/
int HS::genARFFFile(string outName,int isStr,int bytes,int filterAttrTo)
{
  
  return 0;
}

int HS::initARFF(string outName)//,int bytes, unsigned int min, unsigned int max)
{//cout<<"call init arff"<<endl;
  initARFF_file = "";//reset
  initARFF_file.append("@relation " + outName + '\n' + '\n');
  initARFF_file.append("@attribute isVirus {0,1}\n\n");
  initARFF_file.append("@data\n\n");
  //cout<<initARFF_file<<endl;
  for(int i=0;i<vectorOfFile.size();i++)
  {
    string fileName = vectorOfFile[i];
    if(fileName.find("/benign/")!=std::string::npos)
      initARFF_file.append(",0\n");
    else if(fileName.find("/virus/")!=std::string::npos)
      initARFF_file.append(",1\n");
  }
  // cout<<initARFF_file<<endl;
  return 0;

  ofstream oFile;
  oFile.open (outName.c_str());
  if(oFile.is_open())
  {
    oFile<<"@relation "<<outName<<endl<<endl;
    oFile<<"@attribute isVirus {0,1}"<<endl<<endl;
    oFile<<"@data"<<endl<<endl;
    // for(std::vector<std::string>::iterator it = vectorOfFile.begin(); it != vectorOfFile.end(); ++it) 
    for(int i=0;i<vectorOfFile.size();i++)
    {
      string fileName = vectorOfFile[i];
      //check if extractable or not
      {
        /*
        string fileNameToExtract=fileName;
        string longLine="";
        longLine=fileNameToLongHex(fileNameToExtract);//cout<<longLine.length()<<endl;
        if(longLine.length()<config.lengthExtractable)
          continue;
        */
      }
      
      int fileType = -1;
      if(fileName.find("/benign/")!=std::string::npos)
        fileType=0;
      else if(fileName.find("/virus/")!=std::string::npos)
        fileType=1;
      oFile<<","<<fileType<<endl;
    }
    oFile.close();
  }
  else
  {
    cout<<"ERRROR open file "<<outName<<endl;
    return 1;
  }
  return 0;
}

int HS::updateARFF(string inName,string outName,int bytes,int shift, long min, long arrLen)
{
  cout<<"updateARFF"<<endl;
  //pap na
  // int bytes = config.byteLength;
  //EO pap na
  std::stringstream  iFile;
  iFile.str(initARFF_file);
  // cout<<iFile.str()<<endl;
  time_t start,end;
  time (&start);

  ofstream oFile;
  //ifstream iFile;
  //iFile.open(inName.c_str());
  string aLargerBuff = "";
  {
    int fileNum=0;
    int * attrVal = new int[arrLen]();
    /*
      Collect ...
      longHexToArrInterval of ==min to ==max
    //*/
    
    // int * attrValueHex = longHexToArrInterval(1,fileNameToLongHex(),0,10)
    string line;
    vector<string> ultStr;
    // string format = "%0"+to_string(bytes*2)+"x";
    // int fileNum=0;

    while ( getline (iFile,line) )
    {
      // cout<<line<<endl;
      string aLargeBuff = "";
      //handle /r at the end of line
      /*
      if(line.size() && line[line.size()-1]=='\r'){
        line = line.substr(0,line.size()-1);
      }
      //*/
      if(!line.size())
        continue;
      
      //start do some ARFF header
      if(line.find("relation")!=string::npos)
      {
        //oFile<<line<<endl;
        //Better relation's name a little.
        //What should the relation be? inName? outName?
        // oFile<<"@relation "<<inName<<endl<<endl;
        aLargeBuff.append("@relation " + inName + '\n' + '\n');
      }
      else if(line.find("attribute")!=string::npos)
      {
        if(line.find("isVirus")!=string::npos)
        {
          //Last attribute of input file...
          string buf = "";
          for(long i=min;i<min+arrLen;i++)
          {
            // char buf[30];
            // std::sprintf(buf, format.c_str(), i);
            
            string buf2 = longToHex(i,bytes);
            //oFile<<"@attribute "<<buf<<" numeric"<<endl;
            buf+="@attribute "+buf2+" numeric"+'\n';
          }
          aLargeBuff.append(buf + line + "\n\n");
          // aLargeBuff = aLargeBuff + buf;
          // aLargeBuff = aLargeBuff + line + '\n' + '\n';
          // oFile<<buf;
          // oFile<<line<<endl<<endl;
        }
        else
        {//found normal attribute line
          // oFile<<line<<endl;
          aLargeBuff.append(line+'\n');
        }
      }
      else if(line.find("data")!=string::npos)
      {
        aLargeBuff.append(line+"\n\n");// = aLargeBuff + line + '\n' + '\n';
        // oFile<<line<<endl<<endl;
      }
      else
      {
        //start do some add to the new file
        /*
        string longLine="";
        longLine=fileNameToLongHex(vectorOfFile[fileNum]);
        */
        // int * attrVal = longHexToArrInterval(bytes,shift,longLine,min,arrLen);
        // attrVal = longHexToArrInterval_nged(bytes,shift,vectorOfFile[fileNum],min,arrLen);
        longHexToArrInterval_nged(attrVal,bytes,shift,vectorOfFile[fileNum],min,arrLen);
        fileNum++;
        //write output NEW~
        string buff = "";
        buff += line.substr(0,line.length()-2);
        // oFile<<line.substr(0,line.length()-2);
        for(int i=0;i<arrLen;i++)
        {
          buff+=","+patch::to_string(attrVal[i]);
          // oFile<<","<<attrVal[i];
        }
        buff+=line.substr(line.length()-2);
        // oFile<<buff<<endl;
        // oFile<<line.substr(line.length()-2)<<endl;
        aLargeBuff.append(buff+'\n');// = aLargeBuff + buff + '\n';
        //delete [] attrVal;
      }
      aLargerBuff.append(aLargeBuff);// = aLargerBuff + aLargeBuff;
      // oFile<<aLargeBuff<<std::flush;
    }
    time (&end);
    double dif = difftime (end,start);
    printf ("Elasped time is %.2lf seconds.", dif ); 
    delete [] attrVal;
  }

  oFile.open(outName.c_str());
  if(oFile.is_open()/*&&iFile.is_open()*/)
  {
    oFile<<aLargerBuff<<std::flush;
    //iFile.close();
    oFile.close();
  }
  else
  {
    cout<<"ERRROR open file "<<outName;
    cout<<" or "<<inName<<endl;
    return 1;
  }
    cout<<"updateARFF END"<<endl;
  return 0;
}


/*
  A basic function. to call objdump and made it into a very long hexadecimal.
//*/
string HS::fileNameToLongHex_old(string fileName)
{
  if(fileName.length()<=0)
  {
    cout<<"Error  blank file name in fileNameToLongHex of "<<fileName<<endl;
    return "";
  }
  string command = "objdump -s "+fileName;
  string objdumpCommRes = commonLib::exec(command);
  // int sumLen = 0;
  string longLine = "";
  std::istringstream f(objdumpCommRes);
  string line;
  while (std::getline(f, line)){
    if(line[0]==' ')//found space
    {
      int firstSpace = line.substr(1).find(" ");
      string lineProcess1=line.substr(firstSpace+2);
      string lineProcess2=lineProcess1.substr(0,35);
      longLine+=lineProcess2;
      // sumLen+=lineProcess2.length();
      // cout<<line<<endl;
      // cout<<lineProcess2<<endl;
    }
  }
  if(longLine.length()<config.lengthExtractable)
  {
    cout<<("something wrong with "+fileName)<<endl;
  }
  longLine.erase(std::remove_if(longLine.begin(), longLine.end(), (int(*)(int))std::isspace), longLine.end());
  return longLine;
}

int * HS::longHexToArrInterval(int bytes, int shift, string & longLine, long min, long arrLen)
{
  // unsigned int arrLen = max-min+1;
  int* attrValueHex = new int[arrLen];//to keep value in the interval
  for(int i=0;i<arrLen;i++){
    attrValueHex[i]=0;
  }
  // char* c = new char[bytes*2+1];
  // c[bytes*2]='\0';
  // int shift = config.shift;
  // for(int i=0;i<longLine.length()-bytes;i=i+(shift*2))
  for(int i=0;i<=longLine.length()-(bytes*2);i=i+(shift*2)){
    /*
    for(int j=0;j<(bytes*2);j++)
    {
      c[j]=longLine.c_str()[i+j];
    }*/
    string sSub = longLine.substr(i,(bytes*2));
    long idx = (long)strtol(sSub.c_str(), NULL, 16);
    
    if(idx>=min&&idx<(min+arrLen)){
      // attrValueHex[idx]++;
      attrValueHex[idx-min]++;
    }
  }
  // delete[] c;
  return attrValueHex;
}

int HS::longHexToArrInterval_nged(int * arrIn, int bytes, int shift, string fileName, long min, long arrLen)
{
  //int * attrValueHex = new int[arrLen]();
  std::fill(arrIn,arrIn + arrLen,0);
  //arrIn = memset
  /*
  for(int i=0; i < arrLen; i++){
    attrValueHex[i] = 0;
  }*/
  //fileName = fileName + ".pcd.sep.nged";
  readFromNgedArr(arrIn, fileName, bytes, min, arrLen);
  /*
  std::map<long,int> aMap;
  readFromNged(aMap, fileName, bytes);
  for(long i = 0; i < arrLen; i++){
    if(aMap.find(i+arrLen)!=aMap.end()){
      attrValueHex[i] = aMap[i+arrLen];
    }
  }
  //*/
  return 0;//attrValueHex;
}

int * HS::longHexToARFFValueHexNStrArrIdxMethod(int bytes,string longLine)
{
  int* attrValueHex = new int[commonLib::powInt(256,bytes)];//to keep all value.
  for(int i=0;i<commonLib::powInt(256,bytes);i++)
  {
    attrValueHex[i]=0;
  }
  char* c = new char[bytes*2+1];
  c[bytes*2]='\0';
  for(int i=0;i<longLine.length()-bytes;i=i+2)
  {
    for(int j=0;j<(bytes*2);j++)
    {
      c[j]=longLine.c_str()[i+j];
    }
    int idx = (int)strtol(c, NULL, 16);
    attrValueHex[idx]++;
  }
  // cout<<"_"<<attrValueHex[0]<<"_"<<endl;
  delete[] c;
  return attrValueHex;//attrValueHex;
}

std::string HS::longToHex( long l,int bytes)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(bytes*2) 
         << std::hex << l;
  return stream.str();
};

string HS::featureSelectionToFile(int bytes, int shift, int h_start, int h_doForNSegment)
{
  // a1.config.byteLength=i;
  string fileOutPath="./OutARFF/";
  long step = config.step;//256;
  long nTh_start = 0;//should be h_start*256^(bytes-1) /*and add limit to doForNSegment*/
  long maxLen = commonLib::powUInt(256,bytes);
  // string preFix = "FS_Top50000_";
  string fileInName = FS_preFix+"n"+patch::to_string(bytes);
  fileInName+=".arff";
  //+"1.arff";
  string fileOutName = FS_preFix+"n"+patch::to_string(bytes);
  fileOutName+="_s"+patch::to_string(shift);
  //fileOutName+="_m"+patch::to_string(method);
  fileOutName+=".arff";
  
  initARFF(fileOutPath+fileInName);
  
  std::vector<attrValue> attrCollect[4];
  for(;nTh_start+step<=maxLen;nTh_start+=step)
  {
    long nTh_end = nTh_start+step;
    long step_ = step;
    if( (nTh_start) > (maxLen-step) ){//better check around the limit, maybe overflow and mad them broken.
      step_ = nTh_start+(step-maxLen);
    }
    updateARFF(fileOutPath+fileInName,fileOutPath+fileOutName,bytes,shift,nTh_start,step_);//0->256*256(FF)
    //Then move from Out to In?
    std::vector<attrValue> moreValue[4];
    //Edit to run only information gain
    //for(int i=0;i<4;i++)
    {
      int i=2;
      moreValue[i]= wekaFeatureSelection(i+1, fileOutPath+fileOutName);
      //remove file now, the new file will be written by thread.
      // std::remove((fileOutPath+fileOutName).c_str());
      attrCollect[i].insert( attrCollect[i].end(), moreValue[i].begin(), moreValue[i].end() );
      if(attrCollect[i].size()>20001)
      {
        std::sort(attrCollect[i].begin(), attrCollect[i].end());
        attrCollect[i].erase(attrCollect[i].begin()+20000,attrCollect[i].end());
      }
    }
  }
  
  //remove file after used, to preserve disk space
  //std::remove((fileOutPath+fileOutName).c_str());
  
  
  for(int i=0;i<4;i++)
  {
    cout<<"got size = "<<attrCollect[i].size()<<endl;
    std::sort(attrCollect[i].begin(), attrCollect[i].end());
    // attrCollect.erase(attrCollect.begin()+5,attrCollect.end());

    string buff = "";
    for(int idx=0;idx<attrCollect[i].size();idx++)
    {//print all in vect
      buff += attrCollect[i][idx].name + '\n' ;
      buff += patch::to_string(attrCollect[i][idx].val) + '\n' ;
    }

    ofstream oFile;
    string outName = fileOutPath+fileOutName.substr(0,fileOutName.length()-5)+ "_m"+patch::to_string(i+1)+".fsattr";
    oFile.open(outName.c_str());
    if(oFile.is_open())
    {
      oFile<<buff<<std::flush;
      /*
      for(int idx=0;idx<attrCollect[i].size();idx++)
      {//print all in vect
        oFile<<attrCollect[i][idx].name<<endl;
        oFile<<attrCollect[i][idx].val<<endl;
      }*/
    }
    oFile.close();
    attrCollect[i].clear();
  }
  return fileOutName;
};

std::vector<attrValue> HS::wekaFeatureSelection(int method,string filePath)
{
  std::vector<attrValue> attrVect;
  std::string command = "java " + MEMORY_FOR_JAVA ;
#ifdef ON_DIGITALOCEAN
  command += " -cp /root/weka-3-7-13/weka.jar ";
#else
  command += " -cp \"C:/Program Files/Weka-3-9/weka.jar\" ";
#endif
  command += "weka.Run -no-scan ";
  // std::string command = "java -cp /root/weka-3-7-13/weka.jar weka.Run -no-scan ";
  switch(method)
  {
    case 1:command += "weka.attributeSelection.CorrelationAttributeEval ";break;
    case 2:command += "weka.attributeSelection.GainRatioAttributeEval ";break;
    case 3:command += "weka.attributeSelection.InfoGainAttributeEval -B ";break;
    case 4:command += "weka.attributeSelection.SymmetricalUncertAttributeEval ";break;
    // default:break;
  }
  // command += "-s \"weka.attributeSelection.Ranker -T -1.7976931348623157E308 -N -1\" ";
  command += "-s \"weka.attributeSelection.Ranker -T -1.7976931348623157E308 -N 20005\" ";
  command += "-i ";
  command += "\""+filePath+"\"";
  
  // cout<<command<<endl;
  // command += "\"/root/proj/projv5/OutARFF/s22.arff\" ";
  
  string wekaRes=commonLib::exec(command);
  string longLine = "";
  std::istringstream f(wekaRes);
  string line;
  while (std::getline(f, line)){
    if(line.find("Ranked attributes")!=std::string::npos){
      std::getline(f,line);
      for(std::getline(f,line);line.length()>0;std::getline(f,line))//Keep reading until got a blank line. May not works with result from windows(/r)
      {
        std::vector<std::string> vStr = commonLib::StringSplit(line);
        if(vStr.size()==3)//this should be it.
        {
          attrValue av;
          av.val = strtod(vStr[0].c_str(),NULL);
          av.name = vStr[2];
          attrVect.push_back(av);
          //do something with vStr[0](value) and vStr[2](attr's name)
        }
      }
    }
  }
  if(attrVect.size()==0)
  {
    cout<<"ERROR WEKA"<<endl;
    cout<<wekaRes<<endl;
  }
  return attrVect;
}

int HS::filterSomeFiles()
{
  ofstream oFile;
  // ifstream iFile;
  // oFile.open("BrokenFile"); 
  oFile.open("UsableFiles"); 
  for(int i=0;i<vectorOfFile.size();i++)
  {
    // cout<<vectorOfFile[i]<<endl;
    string fileName=vectorOfFile[i];
    string command1 = "objdump -s "+fileName;
    string result1 = commonLib::exec(command1);
    string command2 = "objdump -d "+fileName;
    string result2 = commonLib::exec(command2);
    // cout<<fileName<<" and Length = "<< result1.length()<<endl;
    int flag=0;
    if(result1.length()<config.lengthExtractable){
      cout<<fileName<<" failed -s"<<endl;
      // oFile<<fileName<<endl;
      flag++;
      // cout<<result1.si<<endl;
      // return 0;
    }
    
    if(result2.length()<config.lengthExtractable){
      cout<<fileName<<" failed -d"<<endl;
      // oFile<<fileName<<endl;
      flag++;
      // cout<<result2<<endl;
    }
    if(flag==0)
    {
      oFile<<fileName<<endl;
    }
  }
  oFile.close();
  return 0;
}

string HS::fsattrToarff(string fileInName, int iAttrNum, int shift)
{
  //to make this easier arranged.
  string sAttrNum = patch::to_string(iAttrNum);
  while(sAttrNum.length()<4)
  {
    sAttrNum = "0"+sAttrNum;
  }
  string fileOutName = fileInName.substr(0,fileInName.length()-7)+"_"+sAttrNum+".arff";//arff to go into some classifier
  
  ofstream oFile;
  ifstream iFile;
  oFile.open(fileOutName.c_str());
  iFile.open(fileInName.c_str());
  // std::vector<attrFreq> vSelected;//i choose the easier method now. Go! string.
  std::map<string,int> mSelected;
  if(oFile.is_open()&&iFile.is_open())
  {
    // int attrCount = 0;
    string line;
    while ( getline (iFile,line) && mSelected.size() < iAttrNum)
    {
      // cout<<line<<endl;
      /*attrFreq n;
      n.name = line;
      n.freq = 0;
      vSelected.push_back(n);*/
      mSelected[line] = 0;
      getline (iFile,line);//just flush out
      // attrCount++;
    }
    // std::sort(vSelected.begin(),vSelected.end());
    
    // int strLen = vSelected[0].length();
    
    //Then start write the file
    oFile<<"@relation "<<fileOutName<<endl<<endl;
    // for(int i=0;i<mSelected.size();i++)
      // oFile<<"@attribute "<<vSelected[i].name<<" numeric"<<endl;
    std::map<string,int>::iterator it;
    for(it = mSelected.begin();it!=mSelected.end();it++)
      oFile<<"@attribute "<<it->first<<" numeric"<<endl;
    oFile<<"@attribute isVirus {0,1}"<<endl<<endl;
    oFile<<"@data"<<endl<<endl;
    for(int i=0;i<vectorOfFile.size();i++)//all files
    {
      //copy vector
      // std::vector<attrFreq> vTmp = vSelected;
      std::map<string,int> mTmp = mSelected;
      
      string fileName = vectorOfFile[i];
      // cout<<vectorOfFile[i]<<endl;
      string longLine=fileNameToLongHex(fileName);
      if(longLine.length()<config.lengthExtractable)
        continue;
      
      //To calculate to vSelected here.
      basicCount3(mTmp, shift, longLine);
      
      for(it = mTmp.begin();it!=mTmp.end();it++)
        oFile<<","<<it->second;
      /*
      for(int j=0;j<vTmp.size();j++)//all in vSelected
      {
        int freq = vTmp[j].freq;//basicCount(vSelected[j],shift,longLine);
        oFile<<","<<freq;
      }*/
      int fileType = -1;
      if(fileName.find("/benign/")!=std::string::npos)
        fileType=0;
      else if(fileName.find("/virus/")!=std::string::npos)
        fileType=1;
      oFile<<","<<fileType<<endl;
      
      mTmp.clear();
    }
  }
  else{
    cout<<"somethings goes wrong in fsattrToarff"<<endl;
  }
  mSelected.clear();
  oFile.close();
  iFile.close();
  return fileOutName;
}

int HS::basicCount(string attr, int shift, string longLine)
{
  int bytes = attr.length()/2;
  int freq=0;
  for(int i=0;i<=longLine.length()-(bytes*2);i=i+(shift*2))
  {
    string s=longLine.substr(i,(bytes*2));
    if(s.compare(attr)==0)
      freq++;
  }
  return freq;
}

int HS::basicCount2(std::vector<attrFreq> & vS, int shift, string longLine)
{
  int bytes = vS[0].name.length()/2;//attr.length()/2;
  // int freq=0;
  for(int i=0;i<=longLine.length()-(bytes*2);i=i+(shift*2))
  {
    string s=longLine.substr(i,(bytes*2));
    attrFreq n;
    n.name = s;
    n.freq = 0;
    vector<attrFreq>::iterator it = find(vS.begin(), vS.end(), n);
    if(it == vS.end())
      continue;
    else
      (*it).freq++;
    
    // if(s.compare(attr)==0)
      // freq++;
  }
  return 0;
}

int HS::basicCount3(std::map<string,int> & mS, int shift, string longLine)
{
  int bytes = mS.begin()->first.length()/2;//attr.length()/2;
  // int freq=0;
  for(int i=0;i<=longLine.length()-(bytes*2);i=i+(shift*2))
  {
    string s=longLine.substr(i,(bytes*2));
    std::map<string,int>::iterator it;
    it = mS.find(s);
    // vector<attrFreq>::iterator it = find(vS.begin(), vS.end(), n);
    if(it == mS.end())
      continue;
    else
      (*it).second++;
  }
  return 0;
}

double HS::arffToWeka(string fileInName,int classifier)
{
  double result=-1;
  std::vector<attrValue> attrVect;
  std::string command = "java " + MEMORY_FOR_JAVA ;
#ifdef ON_DIGITALOCEAN
  command += " -cp /root/weka-3-7-13/weka.jar ";
#else
  command += " ";
#endif
  command += "weka.Run -no-scan ";
  // std::string command = "java -cp /root/weka-3-7-13/weka.jar weka.Run -no-scan ";
  switch(classifier)
  {
    case 1:command += "weka.classifiers.trees.J48 ";break;
    case 2:command += "weka.classifiers.trees.RandomForest -I 100 -K 0 -S 1 -num-slots 1 ";break;
    case 3:command += "weka.classifiers.functions.LibSVM -C 0.25 -M 2 ";break;
    case 4:command += "weka.classifiers.functions.MultilayerPerceptron -L 0.3 -M 0.2 -N 500 -V 0 -S 0 -E 20 -H a ";break;
    case 5:command += "weka.classifiers.rules.JRip -F 3 -N 2.0 -O 2 -S 1 ";break;
    case 6:command += "weka.classifiers.bayes.NaiveBayes ";break;
    case 7:command += "weka.classifiers.bayes.NaiveBayesMultinomial ";break;
    case 8:command += "weka.classifiers.lazy.KStar -B 10 -M a ";break;
    // default:break;
  }
  command += "-t ";
  // command += fileInName;
  command += "\""+fileInName+"\"";
  
  // cout<<command<<endl;
  string resWeka = commonLib::exec(command);
  // cout<<resWeka<<endl;
  string fileOutName = fileInName + "_c" + patch::to_string(classifier);
  ofstream oFile;
  oFile.open(fileOutName.c_str());
  oFile<<resWeka<<endl;
  oFile.close();
  
  std::istringstream f(resWeka);
  string line;
  while (std::getline(f, line)){
    if(line.find("Stratified cross-validation")!=std::string::npos){
      std::getline(f, line);
      std::getline(f, line);
      // cout<<line<<endl;
      std::vector<std::string> vStr = commonLib::StringSplit(line);
      result=strtod(vStr[4].c_str(),NULL);
      //should got it here.
      break;
    }
  }
  
  
  //java -cp /root/weka-3-7-13/weka.jar weka.core.WekaPackageManager
  // java weka.Run -no-scan weka.classifiers.lazy.KStar -B 10 -M a -t "C:\cygwin64\home\DCMote\_copy_oldDoc\v_proj\SP2\FileBackup\ARFF2\hex\lastPack_swap128.arff"
  return result;
}

int HS::extractAllFileToHEX()
{
  for(int i=0;i<vectorOfFile.size();i++)
  {
    string fileNameIn = vectorOfFile[i];
    string fileNameOut = fileNameIn+"HEX";
    string result = fileNameToLongHex_old(fileNameIn);
    ofstream oFile;
    oFile.open(fileNameOut.c_str());
    if(oFile.is_open())
    {
      oFile<<result<<endl;
    }
    oFile.close();
  }
  return 0;
}

int HS::exportObjbyteToPcd() //export all of them
{
  //int i=0;
  ifstream iFile;
  iFile.open("./listdataset_objbyte");
  if (iFile.is_open()){
    string fileNameObjbyte;
    while (std::getline(iFile, fileNameObjbyte)){
      cout<<"exportObjbyteToPcd "+fileNameObjbyte<<endl;

      ifstream iFileObjbyte;
      iFileObjbyte.open(fileNameObjbyte.c_str());
      string longLine = "";//i++;
      if(iFileObjbyte.is_open())
      {//if(i>10)return 0;
        string line;
        while (std::getline(iFileObjbyte, line)){
          if(line[0]==' '){ //found space
            int firstSpace = line.substr(1).find(" ");
            string lineProcess1=line.substr(firstSpace+2);
            string lineProcess2=lineProcess1.substr(0,35);
            longLine+=lineProcess2;
          }
        }
        if(longLine.length()<config.lengthExtractable){
          cout<<("something wrong with "+fileNameObjbyte)<<endl;
        }
        longLine.erase(std::remove_if(longLine.begin(), longLine.end(), (int(*)(int))std::isspace), longLine.end());
      }
      ofstream oFile;
      string fileOutPathName = fileNameObjbyte + ".pcd";
      oFile.open(fileOutPathName.c_str());
      if(oFile.is_open()){
        oFile<<longLine<<endl;
      }
      else{cout<<("something wrong with "+fileOutPathName)<<endl;}
      oFile.close();
    }
  }
  iFile.close();
  return 0;
}

string HS::fileNameToLongHex(string fileName)
{
  string fileNameHEX = fileName+".pcd";
  string sOut="";
  ifstream iFile;
  iFile.open(fileNameHEX.c_str());
  if(iFile.is_open()){
    getline(iFile,sOut);
  }
  iFile.close();
  return sOut;
}

//not really use this function
int HS::isHEXAble()
{
  for(int i=0;i<vectorOfFile.size();i++)
  {
    string fileName = vectorOfFile[i];//*it;
    //check if extractable or not
    {
      string fileNameToExtract=fileName;
      string longLine="";
      longLine=fileNameToLongHex(fileNameToExtract);
      if(longLine.length()<config.lengthExtractable)
      {
        /*
        if(longLine.length()>0)
        {
          cout<<fileNameToExtract<<endl;
          cout<<longLine<<endl;
        }*/
        cout<<fileNameToExtract<<endl;
      }
    }
  }
  return 0;
}

//ngramMap init
int HS::nged2mapInitialize(int ngram)
{
  /*
  for(int i=0;i<vectorOfFile.size();i++)
  {
    string fileName = vectorOfFile[i];
    readFromNged(ngramMap[i], fileName, ngram);
  }*/
  return 0;
}
//just a helper function
long ngedNumberToLong(string & s)
{
  s.erase(std::remove(s.begin(), s.end(), '_'), s.end());
  long l = std::strtol(s.c_str(),NULL,16);
  return l;
}
int HS::readFromNged(std::map<long,int> & aMap, string fileName, int ngram)
{
  fileName += ".pcd.sep.nged";
  ifstream iFile;
  iFile.open(fileName.c_str());
  if(iFile.is_open()){
    string line;
    int step = 0;
    string wordToFind = patch::to_string(ngram)+"-GRAMS";
    while(getline(iFile,line)){//searching for N-GRAMS
      if(line.find(wordToFind)!=std::string::npos){
        //flush out 2 line.
        getline(iFile,line);//Total 256 unique ngrams in 33837 1-grams.
        getline(iFile,line);//------------------------
        break;
      }
    }
    while(getline(iFile,line)){//reading and parsing
      size_t tabPos = line.find('\t');
      if(tabPos == string::npos)
        break;
      string sKey= line.substr(0,tabPos);
      string sValue= line.substr(tabPos+1);
      long lKey = ngedNumberToLong(sKey);
      int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
      aMap[lKey] = iValue;
    }
  }
  iFile.close();
  return 0;
}
int HS::readFromNgedArr(int * arr, string fileName, int ngram, long min, long arrLen)
{
  fileName += ".pcd.sep.nged";
  ifstream iFile;
  iFile.open(fileName.c_str());
  if(iFile.is_open()){
    string line;
    //string wordToFind = patch::to_string(ngram)+"-GRAMS";
    char c = '0'+ngram;
    while(getline(iFile,line)){//searching for N-GRAMS
      if(line[0]==c && line[1]=='-'){
      // if(line.find(wordToFind)!=std::string::npos){
        //flush out 2 line.
        getline(iFile,line);//Total 256 unique ngrams in 33837 1-grams.
        getline(iFile,line);//------------------------
        break;
      }
    }
    while(getline(iFile,line)){//reading and parsing
      if(line.length()==0)
        break;
      size_t tabPos = 3*ngram-1; //line.find('\t');
      // if(tabPos == string::npos)
      //   break;
      string sKey= line.substr(0,tabPos);
      long lKey = ngedNumberToLong(sKey);
      if( (lKey >= min) && ( lKey < (min+arrLen) ) ){
        string sValue= line.substr(tabPos+1);
        int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
        arr[(lKey-min)] = iValue;
      }
    }
  }
  iFile.close();
  return 0;
}
int HS::readFromNgedArrBool(bool * arrBool, string fileName, int ngram, 
  long min, long arrLen)
{
  fileName += ".pcd.sep.nged";
  ifstream iFile;
  iFile.open(fileName.c_str());
  if(iFile.is_open()){
    string line;
    //string wordToFind = patch::to_string(ngram)+"-GRAMS";
    char c = '0'+ngram;
    while(getline(iFile,line)){//searching for N-GRAMS
      if(line[0]==c && line[1]=='-'){
      // if(line.find(wordToFind)!=std::string::npos){
        //flush out 2 line.
        getline(iFile,line);//Total 256 unique ngrams in 33837 1-grams.
        getline(iFile,line);//------------------------
        break;
      }
    }
    while(getline(iFile,line)){//reading and parsing
      if(line.length()==0)
        break;
      size_t tabPos = 3*ngram-1; //line.find('\t');
      // if(tabPos == string::npos)
      //   break;

      string sKey= line.substr(0,tabPos);
      long lKey = ngedNumberToLong(sKey);
      if( (lKey >= min) && ( lKey < (min+arrLen) ) ){
        arrBool[(lKey-min)] = true;
      }
    }
  }
  iFile.close();
  return 0;
}
//with remember function
int HS::readFromNgedsArr(int * arr, string fileName, int ngram, 
  long min, long arrLen, long & fileLinePointing)
{
  fileName += ".pcd.sep.ngeds";
  ifstream iFile;
  iFile.open(fileName.c_str());
  if(iFile.is_open()){
    string line;
    int lineNum = 0;
    if(lineNum==0&&fileLinePointing!=0){
      while(lineNum < fileLinePointing){
        getline(iFile,line);
        lineNum++;
      }
    }
    while(getline(iFile,line)){//reading and parsing
      //size_t tabPos = 3*ngram-1; //line.find('\t');
      size_t tabPos = line.find('\t');
      if( tabPos != string::npos && tabPos == (ngram*2)){
        string sKey= line.substr(0,tabPos);
        long lKey = ngedNumberToLong(sKey);
        if( (lKey >= min) && ( lKey < (min+arrLen) ) ){
          string sValue= line.substr(tabPos+1);
          int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
          arr[(lKey-min)] = iValue;
        }
        if(lKey >= (min+arrLen) || tabPos > (ngram*2)){//time to jump out of this file
          break;
        }
      }
      fileLinePointing++;
    }
  }
  iFile.close();
  return 0;
}
double Ha(int a)
{
  double res = log2(a);
  res*=a;
  return res;
}
double H(int a,int b)
{
  if(a > b){
    cout<<"ERROR H a="<<a<<" b="<<b<<endl;
    return 0L;
  }

  if(a==b || a==0)
    return 0L;
  double dA = (double)a;
  double dB = (double)b;
  double a1= (dA/dB);
  double a2= ((dB-dA)/dB);
  double res = - a1*log2(a1) - a2*log2(a2);
  // double res = - Ha(a1) - Ha(a2);
  return res;
}
std::vector<attrValue> HS::IGBinaryEval(int bytes, long inStart, 
  long batchSize, double minThreshAdd, bool ** arr, double * arrH)
{
  double HClass = H(NUMCLASS0,NUMCLASS0+NUMCLASS1);
  int dataSetSize = vectorOfFile.size();
  long lStart = inStart;//(commonLib::powLong(256,bytes))*(n-1)/ofN;
  //long lEnd = inEnd;//(commonLib::powLong(256,bytes))*(n)/ofN;
  //long batchSize = lEnd-lStart;//(commonLib::powLong(2,batchSizePow));

  //collect all data
  for(int i=0;i<dataSetSize;i++){
    std::fill(arr[i],arr[i] + batchSize, false);
    readFromNgedArrBool(arr[i],vectorOfFile[i] , bytes, lStart, batchSize );
  }

  //calculate 
  for(int j=0;j<batchSize;j++){
    int x1c_ = 0;
    int x1c0 = 0;
    int x1c1 = 0;
    for(int i=0; i<dataSetSize; i++){
      if(arr[i][j]){
        x1c_++;
        if(i<NUMCLASS0)
          x1c0++;
        else
          x1c1++;
      }
    }
    int x0c_ = dataSetSize - x1c_;
    int x0c0 = NUMCLASS0 - x1c0;
    int x0c1 = NUMCLASS1 - x1c1;

    double E0 = (double)x0c_/dataSetSize;
    double E1 = (double)x1c_/dataSetSize;
    double gain = HClass - (E0 * H(x0c0,x0c_)) - (E1 * H(x1c0,x1c_));
    arrH[j]=gain;
  }
  std::vector<attrValue> newVect;
  for(int j=0;j<batchSize;j++){
    //cout<<"j="<<j<<"    "<<arrH[j]<<"\t"<<endl;
    if(arrH[j] >= minThreshAdd){
      attrValue a;
      a.name = longToHex(lStart+j,bytes);
      a.val = arrH[j];
      newVect.push_back(a);
    }
  }
  return newVect;
}
int HS::IGBinaryToFile(int bytes, long inStart, long inEnd, long batchSize)
{
  int dataSetSize = vectorOfFile.size();
  std::vector<attrValue> attrCollect;
  double minThresh = -1;

  //memory opt
  bool ** arr = new bool*[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    arr[i] = new bool[batchSize]();
  }
  double * arrH = new double[batchSize];

  for(long startTheBatch = inStart;startTheBatch < inEnd; startTheBatch+=batchSize)
  {
    if(startTheBatch+batchSize > inEnd){
      cout<<"something wrong batchSize"<<endl;
    }
    cout<<"startBatch - "<<startTheBatch;
    cout<<" AT "<<commonLib::exec("date")<<endl;
    std::vector<attrValue> moreValue = 
      IGBinaryEval(bytes, startTheBatch, batchSize, minThresh, arr, arrH);
    attrCollect.insert( attrCollect.end(), moreValue.begin(), moreValue.end() );
    if(attrCollect.size()>20003)
    {
      std::sort(attrCollect.begin(), attrCollect.end());
      attrCollect.erase(attrCollect.begin()+20000,attrCollect.end());
      minThresh = attrCollect[19999].val;
    }
  }

  delete [] arrH;
  for(int i=0;i<dataSetSize;i++){
    delete [] arr[i];
  }
  delete [] arr;

  std::sort(attrCollect.begin(), attrCollect.end());
  ofstream oFile;
  string sFileName = "OutARFF/igbinary_" + patch::to_string(bytes) +
  "_" + patch::to_string(inStart) + "-" + patch::to_string(inEnd) + ".fsarff";
  oFile.open(sFileName.c_str());
  for(int i=0;i<attrCollect.size();i++){
    oFile<<attrCollect[i].name<<endl;
    oFile<<attrCollect[i].val<<endl;
  }
  oFile.close();
  return 0;
}


std::vector<attrValue> HS::TfidfEval(int bytes, long inStart, 
  long batchSize, double minThreshAdd, int ** arr, double * arrH, 
  long * FileLinePointing, int * docLen)
{
  int dataSetSize = vectorOfFile.size();
  long lStart = inStart;
  for(int i=0;i<dataSetSize;i++){
    std::fill(arr[i],arr[i] + batchSize, 0);
    readFromNgedsArr(arr[i],vectorOfFile[i] , bytes, lStart, batchSize, FileLinePointing[i] );
  }
  //now testing
/*
  cout<<arr[0][0]<<endl;
  cout<<arr[1][0]<<endl;
  cout<<arr[0][170]<<endl;
  cout<<arr[1][170]<<endl;
  cout<<arr[0][255]<<endl;
  cout<<arr[1][255]<<endl;
  cout<<endl<<endl<<endl;
*/

  int * sumColWithTheWord = new int[batchSize]();
  std::fill(sumColWithTheWord, sumColWithTheWord+batchSize, 0);
  for(int i=0;i<dataSetSize;i++){
    for(int j=0;j<batchSize;j++){
      if(arr[i][j] > 0){
        sumColWithTheWord[j]++;
      }
    }
  }
  double * diffTfidfArr = new double[batchSize];
  for(int j=0;j<batchSize;j++){
    if(sumColWithTheWord[j] == 0){
      diffTfidfArr[j] = 0.0;
      continue;
    }
    double sumClass0 = 0.0;
    double sumClass1 = 0.0;
    for(int i=0;i<dataSetSize;i++){
      double tf = (double)arr[i][j]/docLen[i];
      double idf = log((double)dataSetSize/sumColWithTheWord[j]);
      double tfidf = tf*idf;
      if(i<NUMCLASS0)
        sumClass0 += tfidf;
      else
        sumClass1 += tfidf;
    }
    double meanClass0 = sumClass0/NUMCLASS0;
    double meanClass1 = sumClass1/NUMCLASS1;
    if(meanClass0>meanClass1)
      diffTfidfArr[j] = meanClass0-meanClass1;
    else
      diffTfidfArr[j] = meanClass1-meanClass0;
  }
  delete [] sumColWithTheWord;

  std::vector<attrValue> newVect;
  for(int j=0;j<batchSize;j++){
    if((diffTfidfArr[j]*100000) >= minThreshAdd){
      attrValue a;
      a.name = longToHex(lStart+j,bytes);
      a.val = 100000*diffTfidfArr[j];//100000 just to make them bigger and easier when merge
      newVect.push_back(a);
    }
  }
  delete [] diffTfidfArr;

  return newVect;
}
//to measure the lenght of file, used in tf , for tf-idf
int fileLength(string fileName)
{
  ifstream iFile;
  iFile.open((fileName+".pcd").c_str());
  int length = 0;
  if(iFile.is_open()){
    string s;
    getline(iFile,s);
    length = s.length();
  }
  iFile.close();
  return length;
}
int HS::TfidfToFile(int bytes, long inStart, long inEnd, long batchSize)
{
  std::setprecision(12);
  int dataSetSize = vectorOfFile.size();
  std::vector<attrValue> attrCollect;
  double minThresh = -1999999;
  long * FileLinePointing = new long[dataSetSize]();
  std::fill(FileLinePointing,FileLinePointing + dataSetSize, 0L);
  //memory opt
  int ** arr = new int*[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    arr[i] = new int[batchSize]();
  }
  double * arrH = new double[batchSize];

  int * docLength = new int[dataSetSize];
  for(int i=0;i<dataSetSize;i++){
    docLength[i] = fileLength(vectorOfFile[i])+1-bytes;
  }

  for(long startTheBatch = inStart;startTheBatch < inEnd; startTheBatch+=batchSize)
  {
    if(startTheBatch+batchSize > inEnd){
      cout<<"something wrong batchSize"<<endl;
    }
    cout<<"startBatch - "<<startTheBatch;
    cout<<" AT "<<commonLib::exec("date")<<endl;
    std::vector<attrValue> moreValue = 
      TfidfEval(bytes, startTheBatch, batchSize, minThresh, 
        arr, arrH, FileLinePointing, docLength);
    

    attrCollect.insert( attrCollect.end(), moreValue.begin(), moreValue.end() );
    if(attrCollect.size()>20003)
    {
      std::sort(attrCollect.begin(), attrCollect.end());
      attrCollect.erase(attrCollect.begin()+20000,attrCollect.end());
      minThresh = attrCollect[19999].val;
    }
  }
  delete [] docLength;
  delete [] FileLinePointing;
  delete [] arrH;
  for(int i=0;i<dataSetSize;i++){
    delete [] arr[i];
  }
  delete [] arr;

  std::sort(attrCollect.begin(), attrCollect.end());
  ofstream oFile;
  string sFileName = "OutARFF/tfidf_" + patch::to_string(bytes) +
  "_" + patch::to_string(inStart) + "-" + patch::to_string(inEnd) + ".fsarff";
  oFile.open(sFileName.c_str());
  for(int i=0;i<attrCollect.size();i++){
    oFile<<attrCollect[i].name<<endl;
    oFile<<attrCollect[i].val<<endl;
  }
  oFile.close();
  return 0;
}

int HS::fsToArff(string fsName, int attrNum, int byte)
{
  cout<<"start fsToArff"<<endl;
  cout<<fsName<<" - "<<attrNum<<" - "<<byte<<endl;
  string buf = "@relation "+fsName+".arff";
  buf+='\n';
  std::vector <string> svect;
  ifstream iFile;
  iFile.open(fsName.c_str());
  if(iFile.is_open()){
    string line;
    int aNum = 0;
    while(getline(iFile,line) && (aNum < attrNum)){
      // sset.insert(line);
      svect.push_back(line);
      aNum++;
      buf+="@attribute ";
      buf+=line;
      buf+=" numeric";
      buf+='\n';
      getline(iFile,line);//flush 1 out.
    }
  }
  else{
    cout<<"failed open file "<<fsName<<endl;
    return 1;
  }
  iFile.close();
  std::sort(svect.begin(),svect.end());
  buf+="@attribute isVirus {0,1}";
  buf+='\n';
  buf+="@data";
  buf+='\n';

  if(svect.size() != attrNum){
    cout<<"(svect.size() != attrNum)"<<endl;
    return 1;
  }
  int vSize = attrNum;
  int * v = new int[vSize];
  iFile.open(config.fileList.c_str());
  if(iFile.is_open()){
    string thoseFileName;
    int i=0;
    while(getline(iFile,thoseFileName)){
      i++;
      if(i%1000==0)
        cout<<i<<endl;
      ifstream theFile;
      thoseFileName+=".pcd.sep.ngeds";
      theFile.open(thoseFileName.c_str());
      //then read and count
      if(theFile.is_open()){
        std::fill(v,v + vSize,0);
        string line;
        char c = '0'+byte;
        //string toSearchFor = patch::to_string(byte)+"-GRAM";
        while(getline(theFile,line)){
          /*if(line.find(toSearchFor) != string::npos){
            getline(theFile,line);
            getline(theFile,line);
            break;
          }*/
          if(line[0]==c && line[1]=='-' && line[2]=='G'){
            getline(theFile,line);
            getline(theFile,line);
            break;
          }
        }
        while(getline(theFile,line)){
          size_t tabPos = line.find('\t');
          if(tabPos == string::npos)
            break;
          string sKey= line.substr(0,tabPos);
          if(std::binary_search (svect.begin(), svect.end(), sKey) == false)
            continue;
          vector<string>::iterator it = find(svect.begin(),svect.end(),sKey);//vect.find(sOpcode);
          if(it != svect.end()){
            int pos = it - svect.begin();//distance(vec.begin(), it);
            string sValue = line.substr(tabPos+1);
            int iValue = (int)std::strtol(sValue.c_str(),NULL,10);
            v[pos] = iValue;
          }
        }
      }
      else{
        cout<<"failed open file "<<thoseFileName<<endl;
      }
      theFile.close();
      for(int i=0;i<vSize;i++){
        buf+=patch::to_string(v[i]);
        buf+=",";
      }
      if(thoseFileName.find("/benign/")!=std::string::npos)
        buf+="0";
      else if(thoseFileName.find("/virus/")!=std::string::npos)
        buf+="1";
      buf+='\n';
    }
  }
  else{
    cout<<"failed open file "<<"listdataset_objasm"<<endl;
  }
  iFile.close();

  string fileOutName = fsName+patch::to_string(attrNum)+".arff";
  ofstream oFile;
  oFile.open(fileOutName.c_str());
  if(oFile.is_open()){
    oFile<<buf;
  }
  else{
    cout<<"problem open out file"<<endl;
  }
  oFile.close();
  return 0;
}