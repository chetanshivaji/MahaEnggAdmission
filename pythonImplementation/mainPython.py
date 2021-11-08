
'''
1. parsing
2. db
3. UI for input- cross platform
4. output -1. graphs
           2. excel sheets 

parsing(PYTHON) -> DB(FIREBASE)<- UIinput(FLUTTER) ->excel _shutdown_executors
Graph ->

'''
#from os import _FdOrAnyPath
import pymongo
from typing import OrderedDict
from queryDB import *
from graphs import *


def  GetFileNameYear(inFileName):
    year=""
    i = 0
    for ch in inFileName:
        if(ch.isdigit()):
            year = year+ch
            i = i+1
              #for skipping last number which is for cap round not year
            if(i==4):
                break

    return year

def GetRoundNumber(inFileName):    
    for i in range(len(inFileName)-1,-1,-1):
        if(inFileName[i].isdigit()):
            return inFileName[i]
        
    return -1

def initDB(dbName):    
    myclient = pymongo.MongoClient("mongodb://localhost:27017/")
    mydb = myclient[dbName]
     
    return mydb

def addValuesToDB(year, capRound, status,homeUniversity,collegeName,departementName,castsPercentile,myDB,firstPercentileEntry):


    myCollection = myDB["admission"]
    collegeList = collegeName.split("-")
    collegeCode = collegeList[0].strip() 
    collegeList.pop(0)
    college = collegeList[0].strip()
    for i in range(1,len(collegeList)):
        college = college+" "+collegeList[i].strip()   

    
    depList = departementName.split("-")
    depCode = depList[0].strip() 
    depList.pop(0)
    department = depList[0].strip()
    for i in range(1,len(depList)):
        department = department+" "+depList[i].strip()        
    
    #START clean string
    year = year.replace(",","").replace("\"","") .strip()
    capRound =capRound.replace(",","").replace("\"","").strip()
    status=status.replace(",","").replace("\"","").strip()
    homeUniversity=homeUniversity.replace(",","").replace("\"","").strip()
    college=college.replace(",","").replace("\"","").strip()
    collegeCode=collegeCode.replace(",","").replace("\"","").strip()
    department=department.replace(",","").replace("\"","").strip()
    depCode=depCode.replace(",","").replace("\"","").strip()
    firstPercentileEntry = firstPercentileEntry.replace(",","").replace("\"","").strip()
    firstPercentileEntry = float(firstPercentileEntry)
    for key, value in castsPercentile.items():
        castsPercentile[key]=float(value.replace(",","").strip())
    if(not status):
        status="not present"

    #END clean string
    entry= {
        "firstPercentileEntry":firstPercentileEntry,
        "nameYear":year,
        "nameCap":capRound,
        "nameUniversity":homeUniversity,
        "status":status,    
        "nameCollege":college,
        "codeCollege":collegeCode,
        "nameDepartment":department,
        "codeDepartment":depCode,
        "casts":castsPercentile
    }
    myDB.myCollection.insert_one(entry)


def parse2021(year, capRound, inFileName,myDB):
    fIn = open(inFileName,'r')
    collegeCode = '0000'
    oneLineAbove = ""
    secondLineAbove = ""
    lines = fIn.readlines()
    i=0
    while(i < len(lines)):    
        currentLine = lines[i]  
        #print(currentLine)      
        if "Status" in currentLine:
            statusLine= currentLine.replace(",",'')
            statusLine= statusLine.replace("\n",'')
            statusLine= statusLine.replace("Home University",'')
            statusList = statusLine.split(':') 
            status = statusList[1]
            homeUniversity =""
            oneLineAbove = lines[i-1]
            departementName = oneLineAbove.replace(",",'')
            if("200824210" in departementName):
                print("break")
            departementName= departementName.replace("\n",'')
            departementName = departementName.replace("\"","")
            #if("411624210" in departementName):
            #   print("break")
            
            secondLineAbove = lines[i-2]
            secondLineAbove = secondLineAbove.replace("\"","")
            secondLineAbove= secondLineAbove.replace("\n",'')
            if(collegeCode!=departementName[0:4]):
                collegeName = secondLineAbove.replace(",",'')
                collegeCode = collegeName[0:4].replace(",",'')

        elif "Stage" in currentLine: 
            #create and keep adding casts for each stage table
            #loop till we get status
            casts=[]
            castsPercentile = OrderedDict()
            firstPercentileEntry = -1
            while(i < len(lines)):
                if("Status" in lines[i]):
                    i = i-1
                    break #while(1)
                else:
                    if("Stage" in lines[i]):
                        castsLine = lines[i]
                        
                        castsLine = castsLine.replace(" ",'')            
                        castsLine = castsLine.replace('\n','')
                        casts = castsLine.split(",")            
                        casts = [j for j in casts if j]
                        casts.pop(0) 
                        
                        #To handle perticular case; where after line having "stage" there is line with ",,,,,,,,,,,,,,,,,,S".
                        
                        if(",,,,S" in lines[i+1]):
                            i=i+1

                        i  = i+2 #go to percentile of stage I
                        percentileLine = lines[i]
                        percentileLine = percentileLine.replace("\n",'')
                        percentile = percentileLine.split(",")
                        percentile.pop(0)            

                        
                        indx = 0        
                        flagAbs= False    
                        for cast in casts:
                            percent = percentile[indx]
                            
                            if(not percent):
                                #for missing entry in stage 1; look for stage 2; and note down its casts
                                    #go to percentile of stage II
                                if flagAbs==False:
                                    i  = i+2
                                    flagAbs = True
                                stage2PercentileLine = lines[i]
                                stage2PercentileLine = stage2PercentileLine.replace("\n",'')
                                stage2Percentile = stage2PercentileLine.split(",") #create list using delimeter ','
                                stage2Percentile.pop(0)    #remove first empty entry
                                percent = stage2Percentile[indx]                                
                                


                            percent=percent.replace('(','')
                            percent=percent.replace(')','')
                            if(cast not in castsPercentile):
                                castsPercentile[cast]=percent
                                if(firstPercentileEntry==-1):
                                    firstPercentileEntry=percent

                            indx = indx+1
                        
                    i=i+1

            addValuesToDB(year, capRound,status,homeUniversity,collegeName,departementName,castsPercentile,myDB,firstPercentileEntry)
            castsPercentile.clear()
            firstPercentileEntry=-1                        
        i = i+1

def parse(year, capRound, inFileName,myDB):
    fIn = open(inFileName,'r')
    collegeCode = '0000'
    oneLineAbove = ""
    secondLineAbove = ""
    lines = fIn.readlines()
    i=0
    while(i < len(lines)):    
        currentLine = lines[i]
        if "Status" in currentLine:
            statusLine= currentLine.replace(",",'')
            statusLine= statusLine.replace("\n",'')
            statusLine= statusLine.replace("Home University",'')
            statusList = statusLine.split(':') 
            status = statusList[1]
            homeUniversity = statusList[len(statusList)-1]
            oneLineAbove = lines[i-1]
            departementName = oneLineAbove.replace(",",'')
            departementName = departementName.replace("\"","")
            #if("411624210" in departementName):
            #   print("break")
            
            secondLineAbove = lines[i-2]
            secondLineAbove = secondLineAbove.replace("\"","")
            if(collegeCode!=departementName[0:4]):
                collegeName = secondLineAbove.replace(",",'')
                collegeCode = collegeName[0:4].replace(",",'')

        elif "Stage" in currentLine: 
            #create and keep adding casts for each stage table
            #loop till we get status
            casts=[]
            castsPercentile = OrderedDict()
            firstPercentileEntry = -1
            while(i < len(lines)):
                if("Status" in lines[i]):
                    i = i-1
                    break #while(1)
                else:
                    if("Stage" in lines[i]):
                        oneLineAbove = lines[i-1]
                        if("University" in oneLineAbove or "State" in oneLineAbove):
                            castsLine = lines[i+1]
                            i = i+1    #above line was not as expected as casts so increment
                        else:                       
                            castsLine = oneLineAbove
                        castsLine = castsLine.replace(" ",'')            
                        castsLine = castsLine.replace('\n','')
                        casts = castsLine.split(",")            
                        casts = [j for j in casts if j]


                        i  = i+2 #go to percentile of stage I
                        percentileLine = lines[i]
                        percentileLine = percentileLine.replace("\n",'')
                        percentile = percentileLine.split(",")
                        percentile.pop(0)            

                        
                        indx = 0        
                        flagAbs= False    
                        for cast in casts:
                            percent = percentile[indx]
                            
                            if(not percent):
                                #for missing entry in stage 1; look for stage 2; and note down its casts
                                    #go to percentile of stage II
                                if flagAbs==False:
                                    i  = i+2
                                    flagAbs = True
                                stage2PercentileLine = lines[i]
                                stage2PercentileLine = stage2PercentileLine.replace("\n",'')
                                stage2Percentile = stage2PercentileLine.split(",") #create list using delimeter ','
                                stage2Percentile.pop(0)    #remove first empty entry
                                percent = stage2Percentile[indx]                                
                                


                            percent=percent.replace('(','')
                            percent=percent.replace(')','')
                            if(cast not in castsPercentile):
                                castsPercentile[cast]=percent
                                if(firstPercentileEntry==-1):
                                    firstPercentileEntry=percent

                            indx = indx+1
                        
                    i=i+1

            addValuesToDB(year, capRound,status,homeUniversity,collegeName,departementName,castsPercentile,myDB,firstPercentileEntry)
            castsPercentile.clear()
            firstPercentileEntry=-1                        
        i = i+1


def trial():
    data = [23, 17, 35, 29, 12, 41]
    sum =0
    for i in data:
        sum =sum+i
    print(sum)
    
def main(inFile):
    #trial()
    
    parsingOn = True
    queryOn = True
    graphsOn = False
    
    
    if(parsingOn):
        dbName = "Engineering"
        myDb = initDB(dbName)    
        #get year
        year = GetFileNameYear(inFile)
        capRound= GetRoundNumber(inFile)        
        #start parsing now;
        #parse2021(year, capRound,inFile,myDb)
        parse(year, capRound,inFile,myDb)
    if(queryOn):
        query()
    if(graphsOn):
        createGraph()
    
    print("end main")

if __name__ == "__main__":
    #inFile = "MHT-CET-2019-Cutoff-Round-1-Maharashtra.csv"
    inFile = "MHT-CET-2020-Cutoff-Round-1-Maharashtra.csv"
    main(inFile)


#START parsing
'''
1. look for status or stage
    if status
    {
        uni name - status line 
        1 line above department  //get first 4 digit,
        2 line above college name
        create  documents uni->college->department
            if first 4 digits match to old college 4 digits use same college
            {
                
            }
            else
            {
                //2 line above get college name                
            }
        
    }   
    if stage
    {        
        line above csv get all casts  //gets list of casts   
        2 line below gets all percentile ////gets list of percentile
        -> insert in db; using uni->college->department
    }    

'''

