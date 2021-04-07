#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sstream> 
#include <ostream> 
#include <istream> 
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#define pb push_back

using namespace std;
map<int,int>percCourse;

int COMP_IT_DIFF = 0+1;  //number of colleges we expect in between 2 colleges.

string capRound = "2019ENGG_CAP1_CutOff";
string BROUCHERE = "C:\\Users\\DELL\\Downloads\\"+capRound+".csv";
string destinationPath = "D:\\Engineering_Admission_analysis\\"+capRound+"\\"; 
//vector<string>UniPref={"Savitribai Phule Pune","Mumbai", "Shivaji","Ambedkar Marathwada","Gondwana","Ahilyadevi Holkar Solapur","Tukadoji Maharaj Nagpur","Gadge Baba Amravati","SNDT Women"};
vector<string>UniPref={"Savitribai Phule Pune","Mumbai", "Shivaji","SNDT Women"};
bool FLAG_UNI_PREF = false;
string YEAR  = "2019-20";
bool FlagUniversityWise = true;
bool FlagTotal = true;

//****************************************START general Util functions*********************************************************************//

template <class T>
void printSet(set<T>&s)
{
	int index = 1;
	cout<<"printing set"<<endl;
	for(auto i:s)
		{
			cout<<index<<"     "<<i<<endl;
			index++;
		}
		
}
template<class T>
void printList(list<T> & l)
{
	cout<<" ----------> printing list"<<endl;
	
	for(auto i:l)
		cout<<i<<endl;
	
	cout<<"\n\n";
}
void removeAllChars(string &s,char c)
{
	s.erase(remove(s.begin(),s.end(),c),s.end());
}

string stripToGetNumber(string&s)
{
	string tempString =  "";
	for(auto i:s)
	{
		if(isdigit(i) || (i=='.'))
			tempString = tempString+i;
	}
	return tempString;
}
string getLowerCaseString(string &s)
{
	string lowerString;
	for(auto i:s)
		lowerString = lowerString+(char)tolower(i);
	return lowerString;
}


//****************************************END general Util functions*********************************************************************//




//*************************************************START project util functions************************************************************//
void getCollegeNames()
	{
		ifstream fin; 
		string line;
		set<string>setCollegeNames;
		fin.open(BROUCHERE); 
		while(fin)
		{
			getline(fin,line);
			//cout<<line<<endl;
			if(line.find(YEAR)!=string::npos)
			{
				getline(fin,line);
				//cout<<line<<endl;
				line.erase(remove(line.begin(),line.end(),','),line.end());
				//cout<<line<<endl;
				setCollegeNames.insert(line);
			}
		}
		fin.close();
		cout<<"Total number of colleges->"<<setCollegeNames.size()<<endl;
		printSet(setCollegeNames);

	}

void printCountsOfMap(map<string, multimap<double, vector<string>, greater<double>>>&desiredList)
{
	int countUni = 0;
	int total=0;
		for(auto e:desiredList)
		{
			countUni++;
			
			int x =e.second.size();
			total+=x;
			cout<<e.first<<" ->"<<x<<endl;			
		}
		cout<<"countUni "<<countUni<<" Total->"<<total<<endl;
		cout<<"\n\n\n\n\n";
}

template <class U, class D>
void printMultimap(multimap<U,D, greater<double>>&m)
{
		int index  = 1;
		//cout<<"Index"<<"   "<<"Percentile"<<"        "<<" course/department "<<"    "<<"College Name"<<endl;
		for(auto &it:m)
		{
			//cout<<index<<"  "<<it.first<<"  ";
			cout<<index<<it.first<<endl;
			for(auto &itv:it.second)
				cout<<itv<<endl;
			index++;
			cout<<endl;
		}
		
}

int getNumberLOPENH(string line,string CAST, bool &LOPENH_found)
{
	int iWord = 0;
	if(line.find(CAST) != string::npos)
	{
							LOPENH_found = true;
							//count what number word.
							int iFound = line.find(CAST);
							int count = 0;
							
							while(count != iFound)
							{
								if(line[count]==',')
									iWord++;   // at this number word present, use this to get percentile. 
								count++;
							}
	}
	return iWord;
	
						
}

//*************************************************END project util functions************************************************************//





//****************************************START print functions*********************************************************************//

template <class U, class D>
void printMultimapAsPerUniversity(map<string,multimap<U,D, greater<double>>>&m)
{
		ofstream fout;
		
		//mkdir(capRound.c_str());
		mkdir(destinationPath.c_str());
		for(auto &it:m)
		{
		unordered_map<int,int> mCollegeCode_Add;
		list<string> lInfo;	
			int index  = 1;
			string  courseListName = "";
			//univeristy outer
			string dirNameWith_ = it.first;
			replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
			string oldName = destinationPath+dirNameWith_;
			cout<<oldName<<endl;
			mkdir(oldName.c_str());
			oldName = oldName + "\\"+"List.csv";
			fout.open(oldName);
			fout<<"Index"<<","<<"Percentile"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
			//inner percnetile
			//cout<<it.first<<endl;
			//string uni = it.second.back();  //chetan debug
			for(auto &itIn:it.second)
			{
				//START chetan debug
				/*
				if(uni.find("pune")!=string::npos)
				{
					percCourse[itIn.second[1]]++;
				}
				 */
				//END chetan debug
				fout<<index<<","<<itIn.first<<",";
				courseListName = itIn.second[1];
				//cout<<itIn.first<<endl;
				for(auto &itv:itIn.second)
				{
					fout<<itv<<",";
					//cout<<itv<<endl;
				}
				index++;
				fout<<endl;
			}
			fout.close();
			string newName = destinationPath+dirNameWith_+"\\"+dirNameWith_+"_"+courseListName+".csv";
			//cout<<newName<<endl;
			rename(oldName.c_str(),newName.c_str());	
		}
		
}

template <class U, class D>
void printMultimapAsPerUniversity_COMP_IT_SEQ_ToFile_LOPENO(map<string,multimap<U,D, greater<double>>>&m)
{
		ofstream fout;
		
		mkdir(capRound.c_str());
		for(auto &it:m)
		{
			
			int index  = 1;
			string  courseListName = "";
			//univeristy outer
			string dirNameWith_ = it.first;
			//cout<<it.first<<endl;
			replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
			string oldName = destinationPath+dirNameWith_;
			//cout<<oldName<<endl;
			mkdir(oldName.c_str());
			oldName = oldName + "\\"+"COMP_IT_List.csv";
			fout.open(oldName);
			fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
			//inner percnetile
			//cout<<it.first<<endl;
			//FIRST create linked list
			list<string> lInfo;	
			unordered_map<int,list<string>::iterator> mCollegeCode_Add;
			
			
			int clgCode = -1;
			for(auto &itIn:it.second)
			{
				ostringstream os;
				string collegeInfo = "";
				os<<index<<","<<itIn.first<<",";
				courseListName = itIn.second[3];
				clgCode = stoi(itIn.second[4]);
				//cout<<itIn.first<<endl;
				for(auto &itv:itIn.second)
				{
					os<<itv<<",";
					//cout<<itv<<endl;
				}
				index++;
				os<<endl;
				string sOs = os.str();
				
				
				#if 1
				if(mCollegeCode_Add.find(clgCode) == mCollegeCode_Add.end())  //if college didn't appear first
				{					
					//cout<<sOs<<endl;
					lInfo.push_back(sOs);    //push entry info in linked list				
					list<string>::iterator itLast = lInfo.end();
					itLast--;
					mCollegeCode_Add[clgCode] = itLast;   //make entry college code -> address in linked list.
				}
				else
				{
					//cout<<sOs<<endl;
					
					unordered_map<int,list<string>::iterator>::iterator itM = mCollegeCode_Add.find(clgCode);
					list<string>::iterator itList = itM->second;
			
					int xTemp = COMP_IT_DIFF;
					while(itList!=lInfo.end() && xTemp--)
					{
						itList++;
					}
	
					lInfo.insert(itList,sOs);
					
					itList--;
					mCollegeCode_Add[clgCode] = itList;   //update address for college code department came again.		
				}
				#endif
				//printList(lInfo);
			}
			
			
			//cout<<"printing Linked list"<<endl;			
			for(auto entry:lInfo)
			{				
				//cout<<entry<<endl;
				fout<<entry;
			}
			
			fout.close();
			string newName = destinationPath+dirNameWith_+"\\"+dirNameWith_+"_"+courseListName+"COMP_IT_SEQ_LOPENO.csv";
			//cout<<newName<<endl;
			rename(oldName.c_str(),newName.c_str());	
			
		}
		
}









template <class U, class D>
void printMultimapAsPerUniversity_COMP_IT_SEQ_ToFile(map<string,multimap<U,D, greater<double>>>&m)
{
		ofstream fout;
		
		mkdir(capRound.c_str());
		for(auto &it:m)
		{
			
			int index  = 1;
			string  courseListName = "";
			//univeristy outer
			string dirNameWith_ = it.first;
			//cout<<it.first<<endl;
			replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
			string oldName = destinationPath+dirNameWith_;
			//cout<<oldName<<endl;
			mkdir(oldName.c_str());
			oldName = oldName + "\\"+"COMP_IT_List.csv";
			fout.open(oldName);
			fout<<"Index"<<","<<"Percentile"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
			//inner percnetile
			//cout<<it.first<<endl;
			//FIRST create linked list
			list<string> lInfo;	
			unordered_map<int,list<string>::iterator> mCollegeCode_Add;
			
			
			int clgCode = -1;
			for(auto &itIn:it.second)
			{
				ostringstream os;
				string collegeInfo = "";
				os<<index<<","<<itIn.first<<",";
				courseListName = itIn.second[1];
				clgCode = stoi(itIn.second[2]);
				//cout<<itIn.first<<endl;
				for(auto &itv:itIn.second)
				{
					os<<itv<<",";
					//cout<<itv<<endl;
				}
				index++;
				os<<endl;
				string sOs = os.str();
				
				
				#if 1
				if(mCollegeCode_Add.find(clgCode) == mCollegeCode_Add.end())  //if college didn't appear first
				{					
					//cout<<sOs<<endl;
					lInfo.push_back(sOs);    //push entry info in linked list				
					list<string>::iterator itLast = lInfo.end();
					itLast--;
					mCollegeCode_Add[clgCode] = itLast;   //make entry college code -> address in linked list.
				}
				else
				{
					//cout<<sOs<<endl;
					
					unordered_map<int,list<string>::iterator>::iterator itM = mCollegeCode_Add.find(clgCode);
					list<string>::iterator itList = itM->second;
			
					int xTemp = COMP_IT_DIFF;
					while(itList!=lInfo.end() && xTemp--)
					{
						itList++;
					}
	
					lInfo.insert(itList,sOs);
					
					itList--;
					mCollegeCode_Add[clgCode] = itList;   //update address for college code department came again.		
				}
				#endif
				//printList(lInfo);
			}
			
			
			//cout<<"printing Linked list"<<endl;			
			for(auto entry:lInfo)
			{				
				//cout<<entry<<endl;
				fout<<entry;
			}
			
			fout.close();
			string newName = destinationPath+dirNameWith_+"\\"+dirNameWith_+"_"+courseListName+"COMP_IT_MIX.csv";
			//cout<<newName<<endl;
			rename(oldName.c_str(),newName.c_str());	
			
		}
		
}



template <class U, class D>
void printMultimapAsPerUniversity_COMP_IT_SEQ_LOPENO_ToFile(map<string,multimap<U,D, greater<double>>>&m)
{
		ofstream fout;
		
		mkdir(capRound.c_str());
		for(auto &it:m)
		{
			
			int index  = 1;
			string  courseListName = "";
			//univeristy outer
			string dirNameWith_ = it.first;			
			replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
			string oldName = destinationPath+dirNameWith_;
			mkdir(oldName.c_str());
			oldName = oldName + "\\"+"COMP_IT_List.csv";
			fout.open(oldName);
			fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
			//inner percnetile
			//cout<<it.first<<endl;
			//FIRST create linked list
			list<string> lInfo;	
			unordered_map<int,list<string>::iterator> mCollegeCode_Add;
			
			
			int clgCode = -1;
			for(auto &itIn:it.second)
			{
				ostringstream os;
				string collegeInfo = "";
				os<<index<<","<<itIn.first<<",";
				courseListName = itIn.second[1];
				clgCode = stoi(itIn.second[4]);
				//cout<<itIn.first<<endl;
				for(auto &itv:itIn.second)
				{
					os<<itv<<",";
					//cout<<itv<<endl;
				}
				index++;
				os<<endl;
				string sOs = os.str();
				
				
				#if 1
				if(mCollegeCode_Add.find(clgCode) == mCollegeCode_Add.end())  //if college didn't appear first
				{					
					//cout<<sOs<<endl;
					lInfo.push_back(sOs);    //push entry info in linked list				
					list<string>::iterator itLast = lInfo.end();
					itLast--;
					mCollegeCode_Add[clgCode] = itLast;   //make entry college code -> address in linked list.
				}
				else
				{
					//cout<<sOs<<endl;
					
					unordered_map<int,list<string>::iterator>::iterator itM = mCollegeCode_Add.find(clgCode);
					list<string>::iterator itList = itM->second;
			
					int xTemp = COMP_IT_DIFF;
					while(itList!=lInfo.end() && xTemp--)
					{
						itList++;
					}
	
					lInfo.insert(itList,sOs);
					
					itList--;
					mCollegeCode_Add[clgCode] = itList;   //update address for college code department came again.		
				}
				#endif
				//printList(lInfo);
			}
			
			
			//cout<<"printing Linked list"<<endl;			
			for(auto entry:lInfo)
			{				
				//cout<<entry<<endl;
				fout<<entry;
			}
			
			fout.close();
			string newName = destinationPath+dirNameWith_+"\\"+dirNameWith_+"_"+courseListName+"COMP_IT_MIX.csv";
			//cout<<newName<<endl;
			rename(oldName.c_str(),newName.c_str());	
			
		}
		
}

template <class U, class D>
void printMultimapToFile(multimap<U,D, greater<double>>&m)
{
		ofstream fout;
		string dirNameWith_ =  capRound;
		replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
		string oldName = destinationPath+dirNameWith_;
		mkdir(oldName.c_str());
		oldName = oldName + "\\"+"List.csv";
		fout.open(oldName);
		
		string  courseListName = "";
		int index  = 1;
		//fout<<"Index"<<"   "<<"Percentile"<<"        "<<" course/department "<<"    "<<"College Name"<<endl;
		fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
		fout<<endl;
		
		for(auto &it:m)
		{
			fout<<index<<","<<it.first<<",";
			courseListName = it.second[3]; //Course
			
			for(auto &itv:it.second)
				{
					fout<<itv<<",";
				}
			index++;
			fout<<endl;
		}
		fout.close();
		string newName = destinationPath + dirNameWith_ +"\\" + courseListName + ".csv";
		rename(oldName.c_str(),newName.c_str());
}


template <class U, class D>
void printMultimapToFile_Total_LOPENO(multimap<U,D, greater<double>>&m)
{
		ofstream fout;
		string dirNameWith_ =  capRound;
		replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
		mkdir(destinationPath.c_str());
		string oldName = destinationPath+dirNameWith_;
		mkdir(oldName.c_str());
		oldName = oldName + "\\"+"List.csv";
		fout.open(oldName);
		
		string  courseListName = "";
		int index  = 1;
		//fout<<"Index"<<"   "<<"Percentile"<<"        "<<" course/department "<<"    "<<"College Name"<<endl;
		fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
		fout<<endl;
		
		for(auto &it:m)
		{
			string uni = it.second.back();
			//START check if expected uni from uniPref list
			
			bool foundUni= false;
			if(FLAG_UNI_PREF)		
			{				
				for(auto ve:UniPref )
				{
					if(uni.find(ve) != string::npos)
					{
						foundUni = true;
						break;
					}					
				}			
				if(foundUni==false)
					continue;
			}
			
			//END check if expected uni from uniPref list
			
			fout<<index<<","<<it.first<<",";
			courseListName = it.second[3]; //Course
			
			for(auto &itv:it.second)
				{
					fout<<itv<<",";
				}
			index++;
			fout<<endl;
		}
		fout<<"";
		fout.close();
		string newName = destinationPath + dirNameWith_ +"\\" + courseListName + "_Maha.csv";
		rename(oldName.c_str(),newName.c_str());
}


template <class U, class D>
void printMultimapToFile_COMP_IT_SEQ_TOTAL_LOPENO(multimap<U,D, greater<double>>&m)
{
		ofstream fout;
		string dirNameWith_ =  capRound;
		replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
		mkdir(destinationPath.c_str());
		string oldName = destinationPath+dirNameWith_;
		mkdir(oldName.c_str());
		oldName = oldName + "\\"+"COMP_IT_List.csv";
		fout.open(oldName);
		
		string  courseListName = "";
		int index  = 1;
		//fout<<"Index"<<"   "<<"Percentile"<<"        "<<" course/department "<<"    "<<"College Name"<<endl;
		fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
		fout<<endl;
		list<string> lInfo;	
		unordered_map<int,list<string>::iterator> mCollegeCode_Add;
		
		int clgCode = -1;
		for(auto &it:m)
		{		
			ostringstream os;
			
			os<<index<<","<<it.first<<",";
			courseListName = it.second[3]; //Course
			clgCode =  stoi(it.second[4]);
			for(auto &itv:it.second)
				{
					os<<itv<<",";
				}
			index++;
			os<<endl;
			
			string sOs = os.str();
						
			if(mCollegeCode_Add.find(clgCode) == mCollegeCode_Add.end())  //if college didn't appear first
			{					
					//cout<<sOs<<endl;
					lInfo.push_back(sOs);    //push entry info in linked list				
					list<string>::iterator itLast = lInfo.end();
					itLast--;
					mCollegeCode_Add[clgCode] = itLast;   //make entry college code -> address in linked list.
			}
			else
			{
					//cout<<sOs<<endl;
					
					unordered_map<int,list<string>::iterator>::iterator itM = mCollegeCode_Add.find(clgCode);
					list<string>::iterator itList = itM->second;
			
					int xTemp = COMP_IT_DIFF;
					while(itList!=lInfo.end() && xTemp--)
					{
						itList++;
					}
	
					lInfo.insert(itList,sOs);
					
					itList--;
					mCollegeCode_Add[clgCode] = itList;   //update address for college code department came again.		
			}
			
		}
		//cout<<"printing Linked list"<<endl;			
		for(auto entry:lInfo)
		{				
				//cout<<entry<<endl;
				fout<<entry;
		}
			
		
		fout.close();
		string newName = destinationPath + dirNameWith_ +"\\" + courseListName + "COMP_IT_SEQ_TOTAL_LOPENO.csv";
		rename(oldName.c_str(),newName.c_str());
}
template <class U, class D>
void printMultimapToFile_COMP_IT_UNIWise(multimap<U,D, greater<double>>&m)
{
		ofstream fout;
		string dirNameWith_ =  capRound;
		replace(dirNameWith_.begin(),dirNameWith_.end(),' ','_');
		string oldName = destinationPath+dirNameWith_;
		mkdir(oldName.c_str());
		oldName = oldName + "\\"+"COMP_IT_List_UniPref.csv";
		fout.open(oldName);
		
		string  courseListName = "";
		int index  = 1;
		//fout<<"Index"<<"   "<<"Percentile"<<"        "<<" course/department "<<"    "<<"College Name"<<endl;
		fout<<"Index"<<","<<"Percentile"<<","<<"LOPENH"<<","<<"LOPENO"<<","<<"codeCourse"<<","<<"Course"<<","<<"codeCollege"<<","<<"College Name"<<","<<"Status"<<","<<"University"<<endl;
		fout<<endl;
		list<string> lInfo;	
		unordered_map<int,list<string>::iterator> mCollegeCode_Add;
		
		int clgCode = -1;
		for(auto &it:m)
		{
			string uni = it.second.back();
			//START check if expected uni from uniPref list
			bool foundUni= false;
			for(auto ve:UniPref)
			{
				if(uni.find(ve) != string::npos)
					{
						foundUni = true;
						break;
					}					
			}
			if(foundUni==false)
				continue;
			//END check if expected uni from uniPref list
			
			ostringstream os;
			
			os<<index<<","<<it.first<<",";
			courseListName = it.second[3]; //Course
			clgCode =  stoi(it.second[4]);
			for(auto &itv:it.second)
				{
					os<<itv<<",";
				}
			index++;
			os<<endl;
			
			string sOs = os.str();
						
			if(mCollegeCode_Add.find(clgCode) == mCollegeCode_Add.end())  //if college didn't appear first
			{					
					//cout<<sOs<<endl;
					lInfo.push_back(sOs);    //push entry info in linked list				
					list<string>::iterator itLast = lInfo.end();
					itLast--;
					mCollegeCode_Add[clgCode] = itLast;   //make entry college code -> address in linked list.
			}
			else
			{
					//cout<<sOs<<endl;
					
					unordered_map<int,list<string>::iterator>::iterator itM = mCollegeCode_Add.find(clgCode);
					list<string>::iterator itList = itM->second;
			
					int xTemp = COMP_IT_DIFF;
					while(itList!=lInfo.end() && xTemp--)
					{
						itList++;
					}
	
					lInfo.insert(itList,sOs);
					
					itList--;
					mCollegeCode_Add[clgCode] = itList;   //update address for college code department came again.		
			}
			
		}
		//cout<<"printing Linked list"<<endl;			
		for(auto entry:lInfo)
		{				
				//cout<<entry<<endl;
				fout<<entry;
		}
			
		
		fout.close();
		string newName = destinationPath + dirNameWith_ +"\\" + courseListName + "COMP_IT.csv";
		rename(oldName.c_str(),newName.c_str());
}


//*******************************************END print functions******************************************************************//


//**********************************************START project main functions***************************************************************//
//
void getListDescending_LOPENO(vector<string> &expectedCourse)
{
		multimap<double, vector<string>, greater<double>>desiredList;
    	ifstream fin; 
		string line;
		set<string>setCollegeNames;
		fin.open(BROUCHERE); 
		string  collegeName = "";
		string course = "";
		string x = "";
		while(fin)
		{
			getline(fin,line);
			//cout<<line<<endl;
			
			//look for college name;
			if(line.find(YEAR)!=string::npos)
			{
				getline(fin,line);
				//cout<<"------------->"<<line<<endl;
				line.erase(remove(line.begin(),line.end(),','),line.end());
				collegeName = line;
				
			}
			//look for department name;
			course = x;
			x = line;
			string statusAndUniversity = "";
		    if(line.find("Status:")!=string::npos)
			{
				string universityName = ""; 
				statusAndUniversity = line;
				removeAllChars(statusAndUniversity,',');
				removeAllChars(statusAndUniversity,'"');
				
				//cout<<"course---------------------->"<<course<<endl;
				removeAllChars(course,',');
				removeAllChars(course,'"');
				//see if current course belong to expectedCourses
				bool courseFound = false;
				for(auto expected:expectedCourse)
				{
					string lowerCaseCourse = getLowerCaseString(course);
					string lowerCaseExpcted = getLowerCaseString(expected);
					if(lowerCaseCourse.find(lowerCaseExpcted)!=string::npos)
					{
						courseFound = true;
						break;
					}
				}
				if(courseFound==false)
					continue;
				//else go ahead and get percentile for that course
				bool LOPENH_found = false;
				int iWord = 0;	
				string value_LOPENH = "";
				string value_LOPENO = "";
				for(int i=0;i<5;i++)
				{
					getline(fin, line);
					
					if(i==1)
					{
						string cast = "LOPENH";
						iWord = getNumberLOPENH(line, cast, LOPENH_found);
					}
					
					if(i==4)
					{
						//percentile here
						//cout<<"here";
						//cout<<line<<endl;
						int x = line.find(')');
						string sub = line.substr(0,(x-0));
						string stringDouble = stripToGetNumber(sub);
						double percentile  = stod(stringDouble);
						//cout<<"percentile -------------------->"<<percentile<<endl;
						
						vector<string> vInfo;
						
						//push course, college, university(optional TODO)
						int i_ = course.find('-');
						string courseCodeTemp = course.substr(0,i_);
						string courseName = "";
						while(!isalpha(course[i_]))
							i_++;
						courseName = course.substr(i_);
						
						removeAllChars(collegeName,'"');
						removeAllChars(collegeName,',');
						int j_ = collegeName.find('-');
						string collegeCodeTemp = collegeName.substr(0,j_);
						string collegeNameFound = "";
						while(!isalpha(collegeName[j_]))
							j_++;
						collegeNameFound = collegeName.substr(j_);
						
						int iUni = statusAndUniversity.find("Home University");
						int iStartStatus = statusAndUniversity.find(":");
						iStartStatus++;
						string statusOfClg = statusAndUniversity.substr(  iStartStatus, iUni - iStartStatus);
						if(statusOfClg=="")
							statusOfClg = "-";
						int iStartUni = statusAndUniversity.find(":",iUni); 
						string university = statusAndUniversity.substr(iStartUni+2);
 
						if(courseName =="")
							courseName = "-";
						if(collegeNameFound =="")
							collegeNameFound = "-";
						if(university =="")
							university = "-";
						
						//
						if(LOPENH_found==true && i==4)
						{
						//find
							int start = 0;
							int count = 0;
							while(line.find('(',start)!=string::npos)
							{
								start = line.find('(', start);
								start++;
								count++;
								if(count==iWord)
								{
									int iEnd = line.find(')', start);
									value_LOPENH = line.substr(start, iEnd-start);
									break;
								}
							}
						}
					//check for LOPENO now; read 4 more lines now.	
					int jWord = 0;
					bool LOPENO_found = false;
					for(int j=0;j<5 && LOPENH_found==true; j++)
					{
						getline(fin, line);
					
						if(j==1)
						{
							string cast = "LOPENO";
							jWord = getNumberLOPENH(line, cast, LOPENO_found);
						}
					
						if(j==4)
						{
							//
							if(LOPENO_found==true && i==4)
							{
							//find
								int start = 0;
								int count = 0;
								while(line.find('(',start)!=string::npos)
								{
									start = line.find('(', start);
									start++;
									count++;
									if(count==jWord)
									{
										int jEnd = line.find(')', start);
										value_LOPENO = line.substr(start, jEnd-start);
										break;
									}
								}
							}
						}	
					}
						//
						if(value_LOPENH=="")
							value_LOPENH ="-";
						if(value_LOPENO=="")
							value_LOPENO ="-";
						
						
						vInfo.pb(value_LOPENH);
						vInfo.pb(value_LOPENO);
						vInfo.pb(courseCodeTemp);
						vInfo.pb(courseName);
						vInfo.pb(collegeCodeTemp);
						vInfo.pb(collegeNameFound);
						vInfo.pb(statusOfClg);
						vInfo.pb(university);
						desiredList.insert(make_pair(percentile,vInfo));
						//cout<<"---------------------------------------------->"<<percentile<<"   "<<course<<"   "<<collegeName<<endl;
					}
				}
			}
			//cout<<line<<endl;
		}
		fin.close();
		//printMultimap(desiredList);
		//printMultimapToFile_COMP_IT_SEQ_TOTAL_LOPENO(desiredList);
		printMultimapToFile_Total_LOPENO(desiredList);
		
		//printMultimapToFile_COMP_IT_UNIWise(desiredList);
		//printMultimapToFile(desiredList);
}

void getListDescending_University(vector<string> &expectedCourse)
{
		map<string, multimap<double, vector<string>, greater<double>>> desiredList;
    	ifstream fin; 
		string line;
		set<string>setCollegeNames;
		fin.open(BROUCHERE); 
		string  collegeName = "";
		string course = "";
		string x = "";
		
		while(fin)
		{
			getline(fin,line);
			//cout<<line<<endl;
			//look for college name;
			if(line.find(YEAR)!=string::npos)
			{
				getline(fin,line);
				//cout<<"------------->"<<line<<endl;
				line.erase(remove(line.begin(),line.end(),','),line.end());
				collegeName = line;
				
			}
			//look for department name;
			course = x;
			x = line;
			string statusAndUniversity = "";
			string universityName = ""; 
		    if(line.find("Status:")!=string::npos)
			{
				
				statusAndUniversity = line;
				removeAllChars(statusAndUniversity,',');
				removeAllChars(statusAndUniversity,'"');
				if(FlagUniversityWise==true)
				{
					int indexUni  = statusAndUniversity.find("University");
					indexUni = statusAndUniversity.find(":",indexUni);
					while(!isalpha(statusAndUniversity[indexUni]))
					{
							indexUni++;
					}
					universityName = statusAndUniversity.substr(indexUni);
				}
				
				
				//cout<<"course---------------------->"<<course<<endl;
				removeAllChars(course,',');
				removeAllChars(course,'"');
				//see if current course belong to expectedCourses
				bool courseFound = false;
				for(auto expected:expectedCourse)
				{
					string lowerCaseCourse = getLowerCaseString(course);
					string lowerCaseExpcted = getLowerCaseString(expected);
					if(lowerCaseCourse.find(lowerCaseExpcted)!=string::npos)
					{
						courseFound = true;
						break;
					}
				}
				if(courseFound==false)
					continue;
				//else go ahead and get percentile for that course	
				for(int i=0;i<5;i++)
				{
					getline(fin, line);
					if(i==1)
					{
						//GOPENH or GOPENS
						//if(line.find(GOPENH))
					}
					if(i==4)
					{
						//percentile here
						//cout<<"here";
						//cout<<line<<endl;
						int x = line.find(')');
						string sub = line.substr(0,(x-0));
						string stringDouble = stripToGetNumber(sub);
						double percentile  = stod(stringDouble);
						//cout<<"percentile -------------------->"<<percentile<<endl;
						
						//push course, college, university(optional TODO)
						int i_ = course.find('-');
						string courseCodeTemp = course.substr(0,i_);
						string courseName = "";
						while(!isalpha(course[i_]))
							i_++;
						courseName = course.substr(i_);
						
						removeAllChars(collegeName,',');
						removeAllChars(collegeName,'"');
						int j_ = collegeName.find('-');
						string collegeCodeTemp = collegeName.substr(0,j_);
						string collegeNameFound = "";
						while(!isalpha(collegeName[j_]))
							j_++;
						collegeNameFound = collegeName.substr(j_);
						
						int iUni = statusAndUniversity.find("Home University");
						int iStartStatus = statusAndUniversity.find(":");
						iStartStatus++;
						string statusOfClg = statusAndUniversity.substr(  iStartStatus, iUni - iStartStatus);
						if(statusOfClg=="")
							statusOfClg = "-";
						int iStartUni = statusAndUniversity.find(":",iUni); 
						string university = statusAndUniversity.substr(iStartUni+2);
 
						if(courseName =="")
							courseName = "-";
						if(collegeNameFound =="")
							collegeNameFound = "-";
						if(university =="")
							university = "-";
						
						vector<string> vInfo;
						//push course, college, university(optional TODO)
						if(desiredList.find(universityName)==desiredList.end())
						{
							multimap<double, vector<string>, greater<double>> desiredUniList;
							vInfo.pb(courseCodeTemp);
							vInfo.pb(courseName);
							vInfo.pb(collegeCodeTemp);
							vInfo.pb(collegeNameFound);
							vInfo.pb(statusOfClg);
							vInfo.pb(university);
							desiredUniList.insert(make_pair(percentile,vInfo));
							desiredList[universityName] = desiredUniList; 
						}
						else
						{
							//university name allready present.
							vInfo.pb(courseCodeTemp);
							vInfo.pb(courseName);
							vInfo.pb(collegeCodeTemp);
							vInfo.pb(collegeNameFound);
							vInfo.pb(statusOfClg);
							vInfo.pb(university);
							desiredList[universityName].insert(make_pair(percentile,vInfo));
						}
						//cout<<"---------------------------------------------->"<<percentile<<"   "<<course<<"   "<<collegeName<<endl;
					}
				}
			}
			//cout<<line<<endl;
		}
		fin.close();
		//printMultimap(desiredList);
		//printCountsOfMap(desiredList);
		//printMultimapAsPerUniversity_COMP_IT_SEQ_ToFile(desiredList);		
		
		printMultimapAsPerUniversity(desiredList);
}

void getListDescending_University_LOPENO(vector<string> &expectedCourse)
{
		map<string, multimap<double, vector<string>, greater<double>>> desiredList;
    	ifstream fin; 
		string line;
		set<string>setCollegeNames;
		fin.open(BROUCHERE); 
		string  collegeName = "";
		string course = "";
		string x = "";
		while(fin)
		{
			getline(fin,line);
			//cout<<line<<endl;
			
			//look for college name;
			if(line.find(YEAR)!=string::npos)
			{
				getline(fin,line);
				//cout<<line<<endl;
				line.erase(remove(line.begin(),line.end(),','),line.end());
				collegeName = line;
				
			}
			//look for department name;
			course = x;
			x = line;
			string statusAndUniversity = "";
			string universityName = ""; 
		    if(line.find("Status:")!=string::npos)
			{				
				statusAndUniversity = line;
				removeAllChars(statusAndUniversity,',');
				removeAllChars(statusAndUniversity,'"');
				
				if(FlagUniversityWise==true)
				{
					int indexUni  = statusAndUniversity.find("University");
					indexUni = statusAndUniversity.find(":",indexUni);
					while(!isalpha(statusAndUniversity[indexUni]))
					{
							indexUni++;
					}
					universityName = statusAndUniversity.substr(indexUni);
				}
				#if 0
				if(universityName.find("SNDT")!=string::npos)
				{
					
					cout<<"----------------------------->"<<course<<endl;
					cout<<"break";
				}
				#endif 
				
				//cout<<"course---------------------->"<<course<<endl;
				removeAllChars(course,',');
				removeAllChars(course,'"');
				//see if current course belong to expectedCourses
				bool courseFound = false;
				for(auto expected:expectedCourse)
				{
					string lowerCaseCourse = getLowerCaseString(course);
					string lowerCaseExpcted = getLowerCaseString(expected);
					if(lowerCaseCourse.find(lowerCaseExpcted)!=string::npos)
					{
						courseFound = true;
						break;
					}
				}

				if(courseFound==false)
					continue;
				//else go ahead and get percentile for that course
				bool LOPENH_found = false;
				int iWord = 0;	
				string value_LOPENH = "";
				string value_LOPENO = "";
				for(int i=0;i<5;i++)
				{
					getline(fin, line);
					//cout<<line<<endl;
					
					if(i==1)
					{
						string cast = "LOPENH";
						iWord = getNumberLOPENH(line, cast, LOPENH_found);
					}
					
					if(i==4)
					{
						//percentile here
						//cout<<"here";
						//cout<<line<<endl;
						int x = line.find(')');
						string sub = line.substr(0,(x-0));
						string stringDouble = stripToGetNumber(sub);
						double percentile  = stod(stringDouble);
						//cout<<"percentile -------------------->"<<percentile<<endl;
																	
						//push course, college, university(optional TODO)
						int i_ = course.find('-');
						string courseCodeTemp = course.substr(0,i_);
						string courseName = "";
						while(!isalpha(course[i_]))
							i_++;
						courseName = course.substr(i_);
						
						removeAllChars(collegeName,',');
						removeAllChars(collegeName,'"');
						int j_ = collegeName.find('-');
						string collegeCodeTemp = collegeName.substr(0,j_);
						string collegeNameFound = "";
						while(!isalpha(collegeName[j_]))
							j_++;
						collegeNameFound = collegeName.substr(j_);
						
						int iUni = statusAndUniversity.find("Home University");
						int iStartStatus = statusAndUniversity.find(":");
						iStartStatus++;
						string statusOfClg = statusAndUniversity.substr(  iStartStatus, iUni - iStartStatus);
						if(statusOfClg=="")
							statusOfClg = "-";
						int iStartUni = statusAndUniversity.find(":",iUni); 
						string university = statusAndUniversity.substr(iStartUni+2);
 
						if(courseName =="")
							courseName = "-";
						if(collegeNameFound =="")
							collegeNameFound = "-";
						if(university =="")
							university = "-";
						
						//
						if(LOPENH_found==true && i==4)
						{
						//find
							int start = 0;
							int count = 0;
							while(line.find('(',start)!=string::npos)
							{
								start = line.find('(', start);
								start++;
								count++;
								if(count==iWord)
								{
									int iEnd = line.find(')', start);
									value_LOPENH = line.substr(start, iEnd-start);
									break;
								}
							}
						}
						//check for LOPENO now; read 4 more lines now.	
						int jWord = 0;
						bool LOPENO_found = false;
						for(int j=0;j<5 && LOPENH_found==true;j++)
						{
							getline(fin, line);
							//cout<<line<<endl;
							if(j==1)
							{
								string cast = "LOPENO";
								jWord = getNumberLOPENH(line, cast, LOPENO_found);
							}
					
							if(j==4)
							{
							//
								if(LOPENO_found==true && i==4)
								{
								//find
									int start = 0;
									int count = 0;
									while(line.find('(',start)!=string::npos)
									{
										start = line.find('(', start);
										start++;
										count++;
										if(count==jWord)
										{
											int jEnd = line.find(')', start);
											value_LOPENO = line.substr(start, jEnd-start);
											break;
										}
									}
								}
							}	
						}
						//
						if(value_LOPENH=="")
							value_LOPENH ="-";
						if(value_LOPENO=="")
							value_LOPENO ="-";
						
						vector<string> vInfo;
						//push course, college, university(optional TODO)
						if(desiredList.find(universityName)==desiredList.end())
						{
							multimap<double, vector<string>, greater<double>> desiredUniList;
							vInfo.pb(value_LOPENH);
							vInfo.pb(value_LOPENO);
							vInfo.pb(courseCodeTemp);
							vInfo.pb(courseName);
							vInfo.pb(collegeCodeTemp);
							vInfo.pb(collegeNameFound);
							vInfo.pb(statusOfClg);
							vInfo.pb(university);
							desiredUniList.insert(make_pair(percentile,vInfo));
							desiredList[universityName] = desiredUniList; 
						}
						else
						{
							//university name allready present.
							vInfo.pb(value_LOPENH);
							vInfo.pb(value_LOPENO);
							vInfo.pb(courseCodeTemp);
							vInfo.pb(courseName);
							vInfo.pb(collegeCodeTemp);
							vInfo.pb(collegeNameFound);
							vInfo.pb(statusOfClg);
							vInfo.pb(university);
							desiredList[universityName].insert(make_pair(percentile,vInfo));
						}						
						//cout<<"---------------------------------------------->"<<percentile<<"   "<<course<<"   "<<collegeName<<endl;
					}
				}
			
			}
			//cout<<line<<endl;
		}
		fin.close();
		//printMultimap(desiredList);

		//printCountsOfMap(desiredList);
		printMultimapAsPerUniversity_COMP_IT_SEQ_ToFile_LOPENO(desiredList);
		
		//printMultimapToFile(desiredList);
}

//**********************************************END project main functions***************************************************************//

int main()
{
	//getCollegeNames();
	vector<vector<string>>bigV;	
	vector<string>computer{"computer","information technology"}; bigV.pb(computer);
	#if 1
	vector<string>mechanical{"mechanical"};bigV.pb(mechanical);
	vector<string>civil{"civil"};bigV.pb(civil);
	vector<string>electrical{"electrical"};bigV.pb(electrical);
	vector<string>Electronics{"Electronics","Electronics and Telecommunication","Electronics and communication"};bigV.pb(Electronics);
	vector<string>chemical{"chemical"};bigV.pb(chemical);
	vector<string>Instrumentation{"Instrumentation"};bigV.pb(Instrumentation);
	vector<string>food{"food"};bigV.pb(food);
	vector<string>bitotechnology{"bio technology","bio medical"};bigV.pb(bitotechnology);
	vector<string>Metallurg{"Metallurg"};bigV.pb(Metallurg);
	vector<string>Production{"Production"};bigV.pb(Production);
	
	#endif
	if(FlagUniversityWise == true)
	{
		for(auto course:bigV)
		{			
			getListDescending_University(course);
			//getListDescending_University_LOPENO(course);
		}
	}
	
	if(FlagTotal == true)
	{
		for(auto course:bigV)
		{					
			getListDescending_LOPENO(course);
		}	
	}
	
	cout<<"end of prog"<<endl;
}