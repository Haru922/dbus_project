#include "bh-sd-gooroom.h"

int main(int argc, char *argv[]) {
    int socket_fd;
    char buf[OUTPUT_BUF_SIZE];
    struct sockaddr_in server_addr;
    int n, len;
    int signal;
    pid_t pid;
    int status;

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    len = sizeof(server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(G_EMU_UDP_PORT);

    for (;;) {
        puts("#########################");
        puts("#         GCli          #");
        puts("#=======================#");
        puts("#   1: Launch AppA      #");
        puts("#   2: Stop AppA        #");
        puts("#   3: Print Log        #");
        puts("#   4: Quit GCli        #");
        puts("#=======================#");
        puts("#   5: Launch Services  #");
        puts("#   6: Print Status     #");
        puts("#   7: Kill Services    #");
        puts("#########################");
        printf("\nCommand: ");
        scanf("%d", &signal);
        puts ("");
        buf[0] = signal + '0';
        buf[1] = '\0';

        switch (signal) {
            case 0: case 1: case 2: case 3:
                sendto(socket_fd,
                       (const char*)buf,
                       strlen(buf),
                       MSG_CONFIRM,
                       (const struct sockaddr*)&server_addr,
                       sizeof(server_addr));
                n = recvfrom(socket_fd,
                             (char*)buf,
                             OUTPUT_BUF_SIZE,
                             MSG_WAITALL,
                             (struct sockaddr*)&server_addr,
                             &len);
                buf[n] = '\0';
                
                printf("[Result]\n%s\n", buf);
                break;
            case 4:
                goto finish;
                break;
            case 5:
                pid = fork();
                if (pid == 0) {
                    execl("/bin/bash", "/bin/bash", "/home/haru/dbus_project/c/sdbus/scripts/bh_sd_launch_services.sh");
                    exit(EXIT_SUCCESS);
                } else 
                    waitpid(pid, &status, 0);
                break;
            case 6:
                pid = fork();
                if (pid == 0) {
                    execl("/bin/bash", "/bin/bash", "/home/haru/dbus_project/c/sdbus/scripts/bh_sd_status_services.sh");
                    exit(EXIT_SUCCESS);
                } else
                    waitpid(pid, &status, 0);
                break;
            case 7:
                pid = fork();
                if (pid == 0) {
                    execl("/bin/bash", "/bin/bash", "/home/haru/dbus_project/c/sdbus/scripts/bh_sd_kill_services.sh");
                    exit(EXIT_SUCCESS);
                } else
                    waitpid(pid, &status, 0);
                break;
            default:
                break;
        }
    }

finish:
    close(socket_fd);
    exit(EXIT_SUCCESS);
}
