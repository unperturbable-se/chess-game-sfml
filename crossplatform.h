#ifdef _WIN32
    #include <windows.h>
    void run_backend()
    {
        WinExec("cmd.exe /c backend\\backend.bat", SW_HIDE);
    }
#endif

#ifdef __linux__
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <cstdlib>
    #define MAKE_FIFO(X) mkfifo(X,0666)
    #define UNLINK_FIFO(X) unlink(X)
    #define MAKE_PROCESS fork()
    void run_backend()
    {
        system("cd backend && ./backend");
        return;
    }

    void run_frontend()
    {
        system("../my_game");
        return;  
    }
#endif
