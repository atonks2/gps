# GPS Testing
This repository is for testing individual functions that will go into the GPS Class
# split.cpp
- Splits a NMEA sentence into a vector of strings.
- Sample output:
- Test case: $GPGGA,210658.000,3706.7984,N,11332.4191,W,1,10,0.9,844.3,M,-20.5,M,,0000*60
-Result:
-\---------------------
-$GPGGA
-210658.000
-3706.7984
-N
-11332.4191
-W
-1
-10
-0.9
-844.3
-M
-\-20.5
-M
-
-0000*60
