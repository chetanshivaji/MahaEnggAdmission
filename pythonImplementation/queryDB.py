import pymongo
import os
import shutil

def queryGeneralInfo(deparments):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   outFileName = "infoGeneral.txt"
   
   if os.path.exists(outFileName):
       os.remove(outFileName)

   foutGen = open(outFileName,"w")
   mydb.myCollection.create_index([('nameDepartment', 'text')]) #Telling mongo db that we will search half string for that key
   
   foutGen.write("How many number of colleges provide single department?\n")
   for depart in deparments:   
      foutGen.write("\n"+depart+"\n")
      foutGen.write(str(mydb.myCollection.find( { "$text": { "$search": depart } } ).count()))

   foutGen.close()


def createCsvs(deparments):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   mydb.myCollection.create_index([('nameDepartment', 'text')])

   dirName = "outputCSVsTotal"
   if(os.path.exists(dirName)):
      shutil.rmtree(dirName)
   os.mkdir(dirName)
   for depart in deparments:
      fileCsvPath = os.path.join(dirName,depart)
      if os.path.exists(fileCsvPath):
          os.remove(fileCsvPath)
      fOutCsv = open( fileCsvPath +".csv",'w' )

      for dicto in myCollection.find({"$text":{"$search":depart}}).sort("firstPercentileEntry",-1):
      #for dicto in myCollection.find({"$text":{"$search":"computer"}}):      
         line = str(dicto["firstPercentileEntry"])+","+dicto['nameDepartment']+","+dicto['codeDepartment']+","+dicto['nameCollege']+","+dicto['codeCollege']+","+dicto['nameUniversity']+","+dicto['status']+"\n"
         fOutCsv.write(line)

      fOutCsv.close()

def createCsvsUniWise(deparments,uniList):
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   mydb.myCollection.create_index([("nameUniversity", "text"),("nameDepartment","text")])
   #mydb.myCollection.create_index( {"nameUniversity": "text","nameDepartment": "text"})
   dirName = "outputCSVsUniWise"

   if(os.path.exists(dirName)):
      shutil.rmtree(dirName)
   os.mkdir(dirName)
   for depart in deparments:
      for uni in uniList:
         fileCsvPath = os.path.join(dirName,uni+"_"+depart)         
         if os.path.exists(fileCsvPath):
            os.remove(fileCsvPath)
         fOutCsv = open( fileCsvPath +".csv",'w' )
         for dicto in myCollection.find({"$and": [{"nameUniversity":uni}, {"nameDepartment": depart}]}).sort("firstPercentileEntry",-1):
         #for dicto in myCollection.find({$and:[{"nameUniversity":uni},{"nameDepartment": depart}]}).sort("firstPercentileEntry",-1):
         #for dicto in myCollection.find({"$text":{"$search":"computer"}}):      
            line = str(dicto["firstPercentileEntry"])+","+dicto['nameDepartment']+","+dicto['codeDepartment']+","+dicto['nameCollege']+","+dicto['codeCollege']+","+dicto['nameUniversity']+","+dicto['status']+"\n"
            fOutCsv.write(line)

         fOutCsv.close()

      print("end of university wise list")

def getDistinct(disticntKeyName):   
   myclient = pymongo.MongoClient("mongodb://localhost:27017/")
   mydb = myclient["Engineering"]
   myCollection = mydb["myCollection"]
   
   lst = mydb.myCollection.distinct(disticntKeyName)
   return lst

def query(): 
   
   departments  = ["computer","information","electronics","electrical","civil","mechanical","bio","chemical","food","textile"] 
   '''
   queryGeneralInfo(departments)
   createCsvs(departments)
   '''
   disticntKeyName = "nameUniversity"
   uniLst = getDistinct(disticntKeyName)
   print(uniLst)
   #createCsvsUniWise(departments, uniLst)      
   
   print("query end")
     