#!/bin/bash

start(){
        stop
        ./supervise.rstree ./status/rstree        nohup ./bin/rstree &> /dev/null & 
}

stop(){
        killall -9 -g supervise.rstree
}

case C"$1" in
        C)
        start
                echo "Done!"
                ;;
        Cstart)
                stop
                start
                echo "Done!"
                ;;
        Cstop)
                stop 
                echo "Done!"
                ;;
        C*)
                echo "Usage: $0 {start|stop}"
                ;;
esac
