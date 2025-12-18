#ifndef PORTABILITY_H
#define PORTABILITY_H

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR_SCREEN() system("cls")
    #define INIT_CONSOLE() system("chcp 65001 > nul")
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <fcntl.h>
    
    #define SLEEP(ms) usleep((ms) * 1000)
    #define CLEAR_SCREEN() system("clear")
    #define INIT_CONSOLE() // Pas nécessaire sur Linux
    
    // Fonction getch() pour Linux
    static inline int getch(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    
    // Fonction kbhit() pour Linux
    static inline int kbhit(void) {
        struct termios oldt, newt;
        int ch;
        int oldf;
        
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        
        ch = getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        
        if(ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        
        return 0;
    }
#endif

#endif // PORTABILITY_H
