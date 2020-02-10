#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 13 12:19:37 2019
functions to save data from Howland Charge Pump boards
and calculate linear coefficient (A/V)
@author: hake
"""
import csv
import os
from pathlib import Path
import numpy
import pandas as pd
from sklearn import datasets, linear_model
defaultDir = os.path.expanduser('~/Documents/Lifetime_System/HCP_Testing/')
masterListName = defaultDir + 'Coeffs_Master_List.csv'
#defaultName = 'Default.csv'
#defaultPath = defaultDir + defaultName

def SaveListToCsv(list, dir=defaultDir, name = 'Default'):
    if not Path(dir).exists():
        os.makedirs(dir)
    fname = defaultDir + name + '.csv'
    with open(fname,'w') as csvfile:
        ivwriter = csv.writer(csvfile, delimiter =',',
                              quotechar='|',quoting=csv.QUOTE_MINIMAL)
        ivwriter.writerow(('Bias(V)','Current(A)'))
        for item in list:
            ivwriter.writerow([item[0],item[1]])
            print('V: '+str(item[0])+' I: '+str(item[1]))
            
def FitLineFromCsv(dir=defaultDir, name = 'Default'):
    fname = defaultDir + name + '.csv'
    df = pd.read_csv(fname)
    V = df['Bias(V)']
    I = df['Current(A)']
    V = V.values.reshape(len(V),1)
    I = I.values.reshape(len(I),1)
    regr = linear_model.LinearRegression()
    regr.fit(V, I)    
    with open(masterListName,'a') as csvfile:
        ivwriter = csv.writer(csvfile, delimiter =',',
                                      quotechar='|',quoting=csv.QUOTE_MINIMAL)     
        coeff = str(regr.coef_).replace('[','').replace(']','')
        inter = str(regr.intercept_).replace('[','').replace(']','')
        ivwriter.writerow([name,coeff,inter,V.size])
    print('coef: ' + str(regr.coef_) + 
          ' intercept: ' + str(regr.intercept_))
    
#FitLineFromCsv()