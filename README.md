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
2. There is a depreceated <mem.h> this needs to be replaced with 
3. /* For memmove. */
#ifdef __STDC__
#include <string.h>
#else
#include <mem.h>
#endif  

Following fixes are made to get the easytest.cpp 
1. easytest.cpp 
2. Return functions have been set to Return 0, else it will fail
3. Row 140 and 152 have been added to Return 0; 
4. Had to comment out row 184-196 to not rewrite the files



