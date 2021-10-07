#include <iostream>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

//I amit every exception such as fork's return -1

int main(){
    if(fork()>0){//end father process,return shell to user,cause process group owner can't create sid
        exit(0);
    }

    setsid();//child process use setsid,create a new say-window,run in the back

    if(fork()>0){//the same as note one
        exit(0);
    }

    for(int i = 0;i<3;++i){//close file descriptor 0,1,2,aims stdin,stdout,stderr
        close(i);
    }

    chdir("/");//change to root directory,so use root to run this program

    umask(0);//give all right to change file

    while(1){
        std::fstream fd("daemon.log",std::fstream::out | std::fstream::app);
    
        time_t tm = time(0);
        char* ti = asctime(localtime(&tm));

        fd << ti;

        fd.close();

        sleep(30);
    }

    return 0;
}