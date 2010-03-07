#!/bin/sh

cdcd close
./serial-robby-client 127.0.0.1 up left exit


./serial-robby-client 127.0.0.1 down open up right exit

cdcd open
./serial-robby-client 127.0.0.1 down close up exit

cdcd close
cdcd open

./serial-robby-client 127.0.0.1 down open up exit
cdcd close

./serial-robby-client 127.0.0.1 down close up left exit
