#!/usr/bin/python3
import numpy as np
import csv
import numpy as np
import json

class ReadLabelData():
    def __init__(self,filename):
        self.data = []
        self.f = open(filename)
        self.BNRBCDMAP = { "Label" : 1,  "Id" : 2, "Name" : 3, "Units":4, "Scale-": 5, "Scale+": 6, "SigBits": 7, "Resolution":9, "MinTransit(msec)":10, 
              "MaxTransmit(msec)": 11, "Type" : 13
            }
    def load_data(self):
        self.data = []
        with self.f as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in spamreader:
                self.data.append(row)
        print(self.data)
         
    
    def make_bnr_bcd_jsons(self):

        for l in self.data :
            print(l)
            label = {}
            for key, value in self.BNRBCDMAP.items():
                label[key] = l[value]

            jsonstr = json.dumps(label)

            labelfile = open (label["Label"] +"_"+ label["Id"]+".json")
            labelfile.write(jsonstr);

            

rl = ReadLabelData("/mnt/d/davoud/ARINC-QT/Source/ArincQt/Labels/label.csv")

rl.load_data();
#rl.make_bnr_bcd_jsons();

   
