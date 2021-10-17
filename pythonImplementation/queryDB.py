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

departments  = (["computer","information","electronics","electrical","civil",
                  "mechanical","bio","chemical","food","textile"])

def printListInFile(fd, list, headMessage):
   #START print uni list in general info file
   line = ""
   for each in list:
      line = line+","+each
   fd.write(headMessage+","+len(list)+"\n")
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
   
   printListInFile(foutGen,uniLst,"university list")
   printListInFile(foutGen,departmentsList,"distinct department list")
   

   #START print uni list in general info file
   uniLine = ""
   for uni in uniLst:
      uniLine = uniLine+","+uni
   foutGen.write("Universities present "+len(uniLst)+"\n")
   foutGen.write(uniLine)
   #END print uni list in general info file

   foutGen.write("How many number of colleges provide single department?\n")
   for depart in departments:   
      foutGen.write("\n"+depart+"\n")
      foutGen.write(str(mydb.myCollection.find( { "$text": { "$search": depart } } ).count()))

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
   
   numOfEntriesInRange = myCollection.find(
            {"$and":
               [
                  {"nameDepartment" : {"$regex":department,'$options': 'i'}},
                  {"firstPercentileEntry":{"$gt":start}},
                  {"firstPercentileEntry":{"$lt":end}}
               ]
            }).count()

   return numOfEntriesInRange      

def query(): 
   
   disticntKeyName = "nameUniversity"
   uniLst = getDistinct(disticntKeyName)
   departmentsList = getDistinct("nameDepartment")
   
   queryGeneralInfo(departments,uniLst,departmentsList)
   createCsvs(departments)
   
   

   createCsvsUniWise(departments, uniLst)      
   
   print("query end")
     