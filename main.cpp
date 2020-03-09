#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

namespace {
    const char *write_tmp_str = "[Desktop Entry]\n"
        "Encoding=UTF-8\n"
        "Type=Application\n"
        "Terminal= false\n"
        "StartupNotify=false\n";
}

int main(int argc, char **argv) {

    char dir[600];
    getcwd(dir, sizeof(dir));
    std::string dir_str = dir;
    std::cout << "Cur work directory: " << dir << std::endl;
    
    if (argc <= 2) {
        std::cout << "Usage: run [Program_name] "
                  << "[Program_path(absolute)] [Icon_path(absolute)] "
                  << "[If move to application(1 means true)]"
                  << std::endl;
        return 1;
    }
    
    /*
    FILE *tmp_file = fopen(dir_str + "/template.desktop", "r");
    if (tmp_file == nullptr) {
        std::cout << "Cannot open template file.\n";
        return 1;
    }*/
    
    FILE *tar_file = fopen((dir_str + "/" + argv[1] + ".desktop").c_str(), "w");
    if (tar_file == nullptr) {
        std::cout << "Cannot create shortcut file.\n";
        return 1;
    }
    
    std::string write_str = write_tmp_str;
    write_str += "Name=";
    write_str += argv[1];
    write_str += "\n";
    write_str += "Exec=";
    write_str += argv[2];
    write_str += "\n";
    if (argc >= 4) {
        write_str += "Icon=";
        write_str += argv[3];
        write_str += "\n";
    }
    
    if (fputs(write_str.c_str(), tar_file) < 0) {
        std::cout << "Cannot write to shortcut file.\n";
        fclose(tar_file);
        return 1;
    }
    
    fclose(tar_file);
    
    std::string exec_perm = "sudo chmod +x ";
    exec_perm += dir_str + "/" + argv[1] + ".desktop";
    int ret = system(exec_perm.c_str());
    if (ret == 127 || ret == -1) {
        std::cout << "Set permission failed.\n";
        return 1;
    }
    
    if (argc >= 5) {
        if (strcmp(argv[4], "1") == 0) {
            std::string exec_mv = "sudo mv ";
            exec_mv += (dir_str + "/" + argv[1] + ".desktop ");
            exec_mv += "/usr/share/applications";
            ret = system(exec_mv.c_str());
            if (ret == 127 || ret == -1) {
                std::cout << "Move to applications fold failed.\n";
                return 1;
            }
        }
    }
    
    return 0;

}
