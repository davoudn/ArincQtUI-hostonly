#!/usr/bin/python
import numpy as np
import csv
import numpy as np
import json
from pathlib import Path

class ReadLabelData():
    def __init__(self,path,discretesfilename,labelfilename):
        self.data_label = []
        self.data_discretes = []
        self.path =  path
        self.Equipment = {}
        self.f_label = open(path+labelfilename)
        self.f_discretes = open(path+discretesfilename)

        self.BNRBCDPARAMSDETAILS = ["Units", "Scale-", "Scale+", "SigBits", "Resolution", "MinTransit(msec)", "MaxTransmit(msec)"]

        self.BNRBCDMAP = { "Label" : 0,  "Id" : 1, "Name" : 2, "Units":3, "Scale-": 4, "Scale+": 5, "SigBits": 6, "Resolution":8, "MinTransit(msec)":9, 
              "MaxTransmit(msec)": 10, "Type" : 12
            }
    def load_data(self):
        self.data_label = []
        with self.f_label as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in spamreader:
                self.data_label.append(row)

        self.data_discretes = []
        with self.f_discretes as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in spamreader:
                self.data_discretes.append(row)
        #rint(self.data)
         
    
    def make_bnr_bcd_jsons(self):
         
        for l in self.data_label : 
            print(l[2])
            label = {}
            for key, value in self.BNRBCDMAP.items():
                label[key] = l[value]
            if (label["Type"]!="BNR" and label["Type"]!="BCD" and label["Type"]!="Discrete"):
                print (label["Type"])

            if ( label["Scale-"]!=""):
                try :
                  if ( float(label["Scale-"]) > 0.0 ):
                     label["Scale-"] = str(-float(label["Scale-"].strip()))
                except ValueError:
                     print("Cannot convert to float!")  # Output: Cannot convert to 
            #  -----------------------------------------------------------------------------
            newlabel ={}
            newlabel["Id"] = label["Id"]
            newlabel["Label"] = label["Label"]
            newlabel["Type"] = label["Type"]
            newlabel["Name"] = label["Name"]
            details= {}
            for it in self.BNRBCDPARAMSDETAILS:
                details[it] = label[it]
            p0 ={}

            p0["Name"] = label["Name"]
            p0["Type"] = label["Type"]
            p0["IBit"] = ""
            p0["FBit"] = ""
            p0["Details"] = details

            params = {}
            params["0"] = p0

            if newlabel["Type"] == "BCD":
                file1 = open("SSMBCD.json",'r')
                jstr = file1.read()
            #    print(jstr)
                jobj = json.loads(jstr)
                params["1"] = jobj

            if newlabel["Type"] == "BNR":
                file2 = open("SSMBNR.json",'r')
                jstr = file2.read()
            #    print(jstr)
                jobj = json.loads(jstr)
                params["1"] = jobj

            file3 = open("SDI.json",'r')
            jstr = file3.read()

            jobj = json.loads(jstr)
            params["2"] = jobj

            newlabel["Params"] = params

            # --------------------------------------------------------------------------------
            

            jsonstr = json.dumps(newlabel)
            filepath = self.path + label["Label"] +"_"+ label["Id"]+".json"
            if Path(filepath).exists()==False:
                labelfile = open (self.path + label["Label"] +"_"+ label["Id"]+".json",'w')
                labelfile.write(jsonstr)

                if (label["Id"] in self.Equipment):
                    self.Equipment[label["Id"]].append(label["Label"])
                else :
                    self.Equipment[label["Id"]] = []
                    self.Equipment[label["Id"]].append(label["Label"])

    
    def make_discrete_jsons(self):
        for i in range(len(self.data_discretes)-1):
            if (self.data_discretes[i][0] == self.data_discretes[i+1][0]) & (self.data_discretes[i][1] == self.data_discretes[i+1][1]) & (self.data_discretes[i][2] == "HIGH") & (self.data_discretes[i+1][2] == "LOW"):
                tmp = []
                c=9
                label = {}
                label["Label"] = self.data_discretes[i][0]
                label["Id"] = self.data_discretes[i][1]
                label["Type"] = "DISCRETE"
                for j in range(3,len(self.data_discretes[i+1])):
                    tmp.append(self.data_discretes[i+1][j]);

                for j in range(3,len(self.data_discretes[i])):
                    tmp.append(self.data_discretes[i][j])

                params = {}
                for x in tmp:
                   #print (x.split(";"))
                   if len(x.split(";")) == 3:
                     params[c-9] = {"IBit": c, "FBit": c,  "Name": x.split(";")[0] , "Details": {"1":x.split(";")[1], "0":x.split(";")[2]}}
                     c+=1
                label["Params"] = params

                jsonstr = json.dumps(label)

                labelfile = open (self.path + label["Label"] +"_"+ label["Id"]+".json",'w')
                labelfile.write(jsonstr)
                if (label["Id"] in self.Equipment):
                    self.Equipment[label["Id"]].append(label["Label"])
                else :
                    self.Equipment[label["Id"]] = []
                    self.Equipment[label["Id"]].append(label["Label"])

    def make_all(self):
        self.make_bnr_bcd_jsons();
        self.make_discrete_jsons();
        jsonstr = json.dumps (self.Equipment);
        equipmentfile = open (self.path + "equipment.json",'w')
        equipmentfile.write(jsonstr)

    
class EquipmentNames:
  def __init__(self, path, filename):
      self.path = path
      self.f_equipments = open(path+filename) 
 
  def load_data(self):
        self.data = []
        with self.f_equipments as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in spamreader:
                self.data.append(row)


  def make_json(self):
      d = {}
      for x in self.data:
            d[x[0]] = x[1]
     
      jsonstr = json.dumps (d);
      equipment_names_filefile = open (self.path + "equipment_names.json",'w')
      equipment_names_filefile.write(jsonstr)
       

#eq = EquipmentNames("/mnt/d/davoud/ARINC-QT/Source/ArincPlainRasberry/Labels/","equipments_names.csv")
#eq.load_data()
#eq.make_json()
rl = ReadLabelData("/mnt/d/davoud/ARINC-QT-NEW/ArincQtUI/menus/Labels/","discr.csv", "label.csv")

rl.load_data()
#rl.make_bnr_bcd_jsons();

#rl.make_discrete_jsons()
rl.make_all()





   
