# Importing libraries
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
from queryDB import * 


departments  = (["computer","information","electronics","electrical","civil",
                  "mechanical","bio","chemical","food","textile"])

def pieChartDepartments(start, end):        
    # Creating dataset
    departmentEntriesCount=[]
    for  department in departments:
        numOfEntriesInRange = getNumberInRange(department,start, end)
        departmentEntriesCount.append(numOfEntriesInRange)

    fig = plt.figure(figsize=(10, 7))
    plt.pie(departmentEntriesCount, labels=departments)
    plt.title(("pi showing number of colleges choosen for a\
                department in a range "+str(start)+" and "+str(end)))
    # Show plot
    plt.show() 
    print("pi chart done for"+str(start)+"and"+str(end)) 

    '''
    cars = ['AUDI', 'BMW']
    data = [5,5]
    
    # Creating plot
    fig = plt.figure(figsize=(10, 7))
    plt.pie(data, labels=cars)
    '''
# Creating autocpt arguments
def func(pct, allvalues):
    absolute = int(pct / 100.*np.sum(allvalues))
    return "{:.1f}%\n({:d} g)".format(pct, absolute)

def pieSmartChartDepartments(start, end):
    # Creating dataset
    departmentEntriesCount=[]
    for  department in departments:
        numOfEntriesInRange = getNumberInRange(department,start, end)
        departmentEntriesCount.append(numOfEntriesInRange)
     
    # Creating explode departmentEntriesCount
    explode = (0.1, 0.0, 0.2, 0.3, 0.0, 0.0)
   
    # Wedge properties
    wp = { 'linewidth' : 1, 'edgecolor' : "green" }
    
    # Creating autocpt arguments
    def func(pct, allvalues):
        absolute = int(pct / 100.*np.sum(allvalues))
        return "{:.1f}%\n({:d} g)".format(pct, absolute)
    
    # Creating plot
    fig, ax = plt.subplots(figsize =(10, 7))
    wedges, texts, autotexts = ax.pie(departmentEntriesCount, 
                                    autopct = lambda pct: func(pct, departmentEntriesCount),
#                                    explode = explode, 
                                    labels = departments,
                                    shadow = True,                                    
                                    startangle = 90,
                                    wedgeprops = wp,
                                    textprops = dict(color ="blue"))
    
    # Adding legend
    ax.legend(wedges, departments,
            title ="departments",
            loc ="center left",
            bbox_to_anchor =(1, 0, 0.5, 1))
    
    plt.setp(autotexts, size = 8, weight ="bold")
    ax.set_title("Customizing pie chart")
    plt.title(("pi showing number of colleges choosen for a\
                department in a range "+str(start)+" and "+str(end)))
    # show plot
    plt.show()
    print("pi chart done for"+str(start)+"and"+str(end)) 



def showTwoLinesDifference(word1,word2,start,end):
    
    fileName = word1+word2+str(start)+str(end)+".csv"
    if(os.path.isfile(fileName)):
        os.remove(fileName)

    fOut = open(fileName,"w")    
    headLine = "index"+","+"college"+ ","+"codeDepartment" +"," + "department"+","+"percentile"+"\n"
    fOut.write(headLine)
    
    
    words = [word1,word2]
    overwriteDict = {}
    index = 1    
    for word in words:
        numOfEntriesInRange = getWordPercentilesList(word,start, end)    
        for doc in numOfEntriesInRange:                     
            if(doc["nameCollege"] in overwriteDict):
                line = str(overwriteDict[doc["nameCollege"]])+","+doc["nameCollege"]+","+doc["codeDepartment"]+","+word+","+str(doc["firstPercentileEntry"])+"\n"
            else:
                line = str(index)+","+doc["nameCollege"]+","+doc["codeDepartment"]+","+word+","+str(doc["firstPercentileEntry"])+"\n"
                overwriteDict[doc["nameCollege"]] = index
                index = index+1
            fOut.write(line)

    fOut.close()

    #START plot

    
    data = pd.read_csv(fileName)  
    plt.ylim(start, end)

    sns.scatterplot(data=data, x="index", y="percentile", hue="department", style="department",size = "percentile")     
    #sns.lmplot(data=data, x="index", y="percentile", hue="department")     
    plt.title(("Graph showing "+word1+" & "+ word2 + "in a range "+str(start)+" and "+str(end)))
    plt.grid(True)   
    plt.show() 
    
    '''

    data = pd.read_csv(fileName)  
    plt.ylim(start, end)
    sns.pointplot(data = data
        ,x = 'index'
        ,y = "percentile"  
        ,hue = "department"                 
        ,linestyles=''       
        )
   
    plt.grid()    
    plt.show() 
    '''
    #END plot  
    print("There")

def showCompareTwoCasts(department, start,end, referenceCast, compCast):        
    skipCollegesDontOffereCompCast = True
    fileName = department+"_"+referenceCast+"_"+compCast+"_"+str(start)+str(end)+".csv"
    if(os.path.isfile(fileName)):
        os.remove(fileName)

    fOut = open(fileName,"w")    
    headLine = "index"+","+"college"+ ","+"codeDepartment" +","\
                + "department"+","+"casts"+","+"percentile"+"\n"
    fOut.write(headLine)
    
    if(referenceCast == "general"):
        referenceCast = "firstPercentileEntry"

    overwriteDict = {}
    index = 1        
    numOfEntriesInRange = getDepartmentCastList(department, "firstPercentileEntry",start, end)    
    for doc in numOfEntriesInRange:            
        if(skipCollegesDontOffereCompCast and compCast not in (doc["casts"])):
            #skipping colleges to come in output csv and graph which dont have entry for that comparign cast
            continue

        line = str(index)+","+doc["nameCollege"]    \
            +","+doc["codeDepartment"]+","+department+","+referenceCast+","+str(doc[referenceCast])+"\n"
        
        fOut.write(line)        

        #check if compare cast compCast is present
        if(compCast in (doc["casts"])):            
            line = str(index)+","+doc["nameCollege"]    \
                +","+doc["codeDepartment"]+","+department+","+compCast+","+str(doc["casts"][compCast])+"\n"
        else:
           line = str(index)+","+doc["nameCollege"]    \
                +","+doc["codeDepartment"]+","+department+","+compCast+","+str(0)+"\n"
        fOut.write(line)
        index = index+1

    fOut.close()

    #START plot

    
    data = pd.read_csv(fileName)  
    plt.ylim(start, end)

    sns.scatterplot(data=data, x="index", y="percentile", hue="casts", style="casts",size = "percentile")         
    #sns.lmplot(data=data, x="index", y="percentile", hue="department")     
    plt.title(("Graph showing "+department+" "+referenceCast+" & "+ compCast + "in a range "+str(start)+" and "+str(end)))
    plt.grid(True)   
    plt.show() 
    
    #END plot  
    print("There")

def createGraph():   
    piChart = True
    compareTwoLine = True
    compareTwoCasts = False
    if(piChart):
        start = 85
        end = 100
        pieSmartChartDepartments(start, end) 
        #pieChartDepartments(start, end) 
    if(compareTwoLine):
        word1 = "computer"
        word2 = "information"
        start = 85
        end = 100
        showTwoLinesDifference(word1,word2,start,end)
    if(compareTwoCasts):
        department = "Electronics"        
        start = 85
        end = 100
        referenceCast  = "general"
        compCast = "LOPENO"
        showCompareTwoCasts(department, start,end, referenceCast, compCast)

    
    sns.set(style="dark")
    fmri = sns.load_departmentEntriesCountset("fmri")
    print(fmri.head())

    # Plot the responses for different\
    # events and regions
    sns.lineplot(x="timepoint",
                y="signal",
                hue="region",
                style="event",
                data=fmri)

    plt.show()
    print("end")
    