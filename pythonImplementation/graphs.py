# Importing libraries
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
  
def createGraph():    
    sns.set(style="dark")
    fmri = sns.load_dataset("fmri")
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