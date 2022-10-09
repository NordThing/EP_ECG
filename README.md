# EP_ECG

Trying to do a compile from beginning - 2022-10-09 
Using Linux and G++ 

Renaming all CASE LETTERS to small letter.h

What´s happening and what have I changed 
1. g++ -c ANALBEAT.CPP 
1a. complaining about bdac.h is missing 
1b. rename from BDAC.H to bdac.h 
1c. g++ -c ANALBEAT.CPP 
1d. SUCCESS and file ANALBEAT.o is created
2. g++ -c BDAC.CPP
2a. SUCCESS and file BDAC.o is created
3. g++ -c BXB.CPP
3a. complaining about wfdb.h is missing
3b. rename WFDB.H to wfdb.h 
3c. run the g++ -c BXB.CPP
3d. complaining that ecgmap.h is missing
3e. rename the ECGMAP.H to ecgmap.h
3f. run the g++ -c BXB.CPP
3g. copmlaining that ecgcodes.h is missing
3h. renaming ECGCODES.h ecgcodes.h
3i. run the g++ -c BXB.CPP
3j. Compiles but complain a lot - waiting

4. run the g++ -c CLASSIFY.CPP
4a. Complains about ISO C++ forbids declaration of "GetRunCount" with no type [-fpermissive]
4b. add that the GetRunCount should be an integer, so an Int before GetRunCount


5.run the QRSDET.CPP and QRSDET2.CPP 
5a. Needs to change <mem.h> into <stdio.h> and <string.h>
5b. compile without problem

6a. run the g++ -c MATCH.CPP 
6b. SUCCESS
7a. run the g++ -c NOISECHK.CPP
7b SUCCESS
8. run the g++ -c POSTCLAS.CPP
8a SUCCESS
9a run the g++ -c QRSFILT.CPP 
9b SUCCESS
10a. run the g++ -c RYTHMCHK.CPP
10b. SUCCESS

11. Trying to compile easytest.cpp 
12. Have included inputs.h in the include to get some data from internet
13. Gotting some problem - have replaced void with int in the main function
14. 


