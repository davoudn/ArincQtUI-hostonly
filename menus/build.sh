#1
#!/usr/bin/bash
g++ -std=c++17 main.cpp DEI1016.cpp  Handler.cpp DataSourceBase.cpp Equipment.cpp json.cpp  Label.cpp  DiscreteParameter.cpp BCDParameter.cpp  BNRParameter.cpp  BaseParameter.cpp ArincData.cpp arinc_functions.cpp -lpigpio
