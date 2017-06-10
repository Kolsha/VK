#include <iostream>
#include "src/api.h"

using namespace std;


int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 4){
        cout << "[access_token] login pass" << endl;
        return 1;
    }

    string access_token, login, pass;

    if(argc == 2){
        access_token = argv[1];
    }
    else if(argc == 3){
        login = argv[1];
        pass = argv[2];
    }
    else if(argc == 4){
        access_token = argv[1];
        login = argv[2];
        pass = argv[3];
    }


    VK::Client api;
    if(api.auth(login, pass, access_token)){
        cout << "Auth ok" << endl;
        cout << "Access token: " << api.access_token() << endl << endl;
    }
    else{
        cout << "Auth fail" << endl;
    }

    return 0;
}
