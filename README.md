# EP_ECG

Trying to do a compile from beginning - 2022-10-09 
Using Linux and G++ 

With the gcc -c this files get success:
1. analbeat.cpp
2. bdac.cpp
3. match.cpp
4. noisechk.cpp
5. postclas.cpp
6. qrsfilt.cpp
7. rythmchk.cpp

Following are not working out of the box 
1. classify.cpp - have a fix 
2. qrsdet.cpp - have a fix
3. qrsdet2.cpp - have a fix
4. easytest.cpp
5. bxb.cpp

Followin fixes are made to get it compiled fro classify.cpp
1. classify.cpp 
2. Go to line 775 
3. add an int before GetRunCount()

Following fixes are made to get the QRSDET and QRSDET2.cpp
1. qrsdet.cpp and qrsdet2.cpp
2. There is a depreceated <mem.h> this needs to be replaced with stdio.h 
3. Add also #include <string.h> 




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
14. Changes in inputs.h from #define ECG_DB_PATH to char ECG_DB_PATH[]= and ended after the row with a ; 
15. On Row 140 in easytest.cpp I added a 0 to the Return function - as it not returning anything
16. Same on Row 152 in easytest.cpp I added a 0 to the Return function - as its not returgnin anything
17. Renamed bxbep.cpp to bxp.cpp in Makefile as this throwing an error at compile
18. When running make now it´s throwing a lot of error from bxb.cpp 
19. Need to add int instead of void in the main function in bxb.cpp
20. 
21. 


