from numpy.lib.function_base import percentile
import pymongo
from pymongo import IndexModel, ASCENDING, DESCENDING,TEXT
import os
import shutil

headLine = ("index"+","+             
            "Percentile"+","+    
            'Department'+","+             
            'codeDepartment'+","+         
            'College'+","                 
            +'codeCollege'+","            
            +'University'+","             
            +'status'+"\n" )

departments  = (["computer information artificial cyber robot data IoT Internet Mechatronics",
                 "electronics",
                 "electrical",
                 "civil",
                  "mechanical",
                  "bio",
                  "chemical",
                  "food",
                  "textile",
                  "Aeronautical",
                   "Agricultural",
                   "Automobile",
                   "Fashion",
                    "Food",
                    "Plastic Polymer",
                    "Production" ])

def printListInFile(fd, list, headMessage):
   #START print uni list in general info file
   line = ""
   for each in list:
      line = line+each+"\n"
   fd.write(headMessage+","+str(len(list))+"\n\n")
   fd.write(line)
   #END print uni list in general info file


def  queryGeneralInfo(departments,uniLst,departmentsList):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   outFileName = "infoGeneral.txt"
   
   if os.path.exists(outFileName):
       os.remove(outFileName)

   foutGen = open(outFileName,"w")
   mydb.myCollection.create_index([('nameDepartment', 'text')]) #Telling mongo db that we will search half string for that key
   
   if(len(uniLst)!=1):
      printListInFile(foutGen,uniLst,"university list")
   printListInFile(foutGen,departmentsList,"\n\ndistinct department list")
   

   #START print uni list in general info file
   if(len(uniLst)!=1):
      uniLine = ""
      for uni in uniLst:
         uniLine = uniLine+","+uni
      foutGen.write("Universities present "+str(len(uniLst))+"\n")
      foutGen.write(uniLine)
   #END print uni list in general info file

   foutGen.write("\n\n\nHow many number of colleges provide single department?\n")
   for depart in departmentsList:       
      foutGen.write("\n"+depart+"\n")
      #foutGen.write(str(mydb.myCollection.find( { "nameDepartment": depart } ).count())) #commented only for 2022
      foutGen.write(str(len(list(mydb.myCollection.find( { "nameDepartment": depart } ).clone())))) #commented only for 2022

   foutGen.close()


def createCsvs(deparments):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   #mydb.myCollection.create_index([('nameDepartment', 'text')])
   mydb.myCollection.create_index([('nameDepartment', pymongo.TEXT)])
   

   dirName = "outputCSVsTotal"
   if(os.path.exists(dirName)):
      shutil.rmtree(dirName)
   os.mkdir(dirName)
   for depart in deparments:
      fileCsvPath = os.path.join(dirName,depart)
      if os.path.exists(fileCsvPath):
          os.remove(fileCsvPath)
      fOutCsv = open( fileCsvPath +".csv",'w' )



      fOutCsv.write(headLine)
      index = 1
      for dicto in myCollection.find({"$text":{"$search":depart}}).sort("firstPercentileEntry",-1):
      #for dicto in myCollection.find({"$text":{"$search":"computer"}}):      
         line = str(index)+","+                           \
                str(dicto["firstPercentileEntry"])+","+    \
                dicto['nameDepartment']+","+                \
                dicto['codeDepartment']+","+             \
                dicto['nameCollege']+","                 \
                +dicto['codeCollege']+","                \
                +dicto['nameUniversity']+","             \
                +dicto['status']+"\n"        
         fOutCsv.write(line)
         index = index+1

      fOutCsv.close()

def createCsvsUniWise(deparments,uniList):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   mydb.myCollection.drop_indexes()

   dirName = "outputCSVsUniWise"

   if(os.path.isdir(dirName)):
      shutil.rmtree(dirName)
   os.mkdir(dirName)
   for depart in deparments:
      for uni in uniList:
         path  = os.path.join(dirName,uni)
         path = path.replace(" ","")
         if(os.path.isdir(path)==False):
            os.mkdir(path)
         fileCsvPath = os.path.join(path,depart)         
         if os.path.isfile(fileCsvPath):
            os.remove(fileCsvPath)

         fOutCsv = open( fileCsvPath +".csv",'w' )
         fOutCsv.write(headLine)
         index=1
         for dicto in myCollection.find(
            {"$and":
               [
                  {"nameDepartment" : {"$regex":depart,'$options': 'i'}},
                  {"nameUniversity":{"$regex":uni,'$options': 'i'}}
               ]
            }).sort("firstPercentileEntry",-1):                          
            line =   (str(index)+","+
                     str(dicto["firstPercentileEntry"])+","+
                     dicto['nameDepartment']+","+
                     dicto['codeDepartment']+","+
                     dicto['nameCollege']+","+
                     dicto['codeCollege']+","+
                     dicto['nameUniversity']+","+
                     dicto['status']+"\n")
            fOutCsv.write(line)

         fOutCsv.close()

      print("end of university wise list")

def getDistinct(disticntKeyName):   
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   lst = mydb.myCollection.distinct(disticntKeyName)
   return lst

from collections import defaultdict
def getWordPercentilesList(department,start, end):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   numOfEntriesInRange = myCollection.find(
            {"$and":
               [
                  {"nameDepartment" : {"$regex":department,'$options': 'i'}},
                  {"firstPercentileEntry":{"$gt":start}},
                  {"firstPercentileEntry":{"$lt":end}}                   
               ]
            }).sort("firstPercentileEntry",-1)      
   return numOfEntriesInRange

def getDepartmentCastList(department, cast,start, end):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   if(cast == "general"):
      cast = "firstPercentileEntry"   

   numOfEntriesInRange = myCollection.find(
            {"$and":
               [
                  {"nameDepartment" : {"$regex":department,'$options': 'i'}},
                  {cast:{"$gt":start}},
                  {cast:{"$lt":end}}                                    
               ]
            }).sort(cast,-1)      
   return numOfEntriesInRange

def getNumberInRange(department,start,end):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   numOfEntriesInRange = (myCollection.find(
            {"$and":
               [
                  {"nameDepartment" : {"$regex":department,'$options': 'i'}},
                  {"firstPercentileEntry":{"$gt":start}},
                  {"firstPercentileEntry":{"$lt":end}}
               ]
            }))#.count()
   numOfEntriesInRange = len(list(numOfEntriesInRange.clone()))
   return numOfEntriesInRange      

def query(): 
   
   disticntKeyName = "nameUniversity"
   uniLst = getDistinct(disticntKeyName)
   departmentsList = getDistinct("nameDepartment")
   
   queryGeneralInfo(departments,uniLst,departmentsList)
   createCsvs(departments)
   
   

   #createCsvsUniWise(departments, uniLst)      
   
   print("query end")
     