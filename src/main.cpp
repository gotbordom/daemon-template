/// THE OLD WAY
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <systemd/sd-journal.h>

void logMessage(const std::string &message)
{
    std::cout << message << std::endl;
}

void daemonize()
{
    // Fork the first time
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Failed to fork process" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        std::cerr << "First fork exited successfully" << std::endl;
        exit(EXIT_SUCCESS); // Parent process exits
    }

    // Create a new session ID to become the session leader
    if (setsid() < 0)
    {
        std::cerr << "Failed to create new session" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Catch, ignore and handle signals */
    // TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork the second time
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Failed to fork process" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        std::cerr << "Second fork exited successfully" << std::endl;
        exit(EXIT_SUCCESS); // Parent process exits
    }

    // The second child process continues

    // Change the current working directory to root
    if (chdir("/") < 0)
    {
        std::cerr << "Failed to change directory to root" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect standard files to /dev/null
    open("/dev/null", O_RDONLY); // STDIN
    open("/dev/null", O_WRONLY); // STDOUT
    open("/dev/null", O_RDWR);   // STDERR
}

int main()
{
    logMessage("Daemon is starting...");
    daemonize();

    while (true)
    {
        sd_journal_print(LOG_INFO, "Daemon is running...");
        sleep(5);
    }

    return 0;
}
