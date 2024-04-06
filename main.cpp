#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// work package structure
typedef struct
{
    size_t entries;
    size_t hours;
    double balance;
} WorkPackage;

void menu(WorkPackage *ptr_package, struct stat &sb)
{
    size_t hours;
    size_t balance;
    size_t selection;

    do
    {
        std::cout << "WORK PACKAGE\n";
        std::cout << "Entries: " << ptr_package->entries << std::endl;
        std::cout << "Hours: " << ptr_package->hours << std::endl;
        std::cout << "Balance: " << ptr_package->balance << std::endl;
        puts("");

        std::cout << "Make a selection:\n";
        std::cout << "1. Log hours\n";
        std::cout << "2. Add expense\n";
        std::cout << "3. Add deposit\n";
        std::cout << "4. Refresh\n";
        std::cout << "5. Clear data\n";
        std::cout << "0. Quit\n";
        puts("");

        std::cout << "Selection: ";
        std::cin >> selection;
        puts("");

        switch(selection)
        {
            case 1:
                std::cout << "Your new hours to log is -> ";
                std::cin >> hours;
                ptr_package->hours += hours;
                ptr_package->entries++;
                puts("");
            break;

            case 2:
                std::cout << "Your new expense to log is -> ";
                std::cin >> balance;
                ptr_package->balance -= balance;
                puts("");
            break;

            case 3:
                std::cout << "Your new deposit to log is -> ";
                std::cin >> balance;
                ptr_package->balance += balance;
                puts("");
            break;

            case 4:
                std::cout << "Refreshed database...\n";
                puts("");
            break;

            case 5:
                std::cout << "Resetting data...\n";
                memset(ptr_package, 0, sizeof(WorkPackage));
                puts("");
            break;

            case 0:
                std::cout << "Quitting...\n";
                puts("");
            break;

            default:
                std::cout << "Wrong entry... try again...\n";
                puts("");
            break;
        }
    } 
    while(selection != 0);
}

int main(int argc, char *argv[])
{
    const char work_data_addr[] = "WorkData.bin";
    
    struct stat sb;
    const int MEM_SIZE = sizeof(WorkPackage);
    bool file_existed = false;

    int open_flag = O_RDWR | O_CREAT;
    mode_t file_perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; // rw- rw- ---

    int fd = open( work_data_addr, open_flag, file_perm );
    if(fd < 0)
    {
        std::cerr << "Error: file open...\n";
        exit(EXIT_FAILURE);
    }

    if( !file_existed )
    {
        char buffer[MEM_SIZE];
        memset(buffer, 0, MEM_SIZE);
        write(fd, buffer, MEM_SIZE);
    }

    WorkPackage *ptr_work_package;

    ptr_work_package = 
                    (WorkPackage *) mmap 
                                        (
                                            NULL, 
                                            MEM_SIZE, 
                                            PROT_READ | PROT_WRITE, 
                                            MAP_SHARED, 
                                            fd, 
                                            0
                                        );
    
    if (ptr_work_package == MAP_FAILED)
    {
        std::cerr << "mmap failed...\n";
    }

    // user menu
    menu(ptr_work_package, sb);

    // shuts down gracefully
    munmap(ptr_work_package, sb.st_size);
    close(fd);
}