#include <iostream>

#include <version.h>
#include <network/network.h>

using namespace std;

int main() {

    cout << version() << endl;

    int socket = createUDP();
    if (bind(socket, 74638) != 0) return -1;

    return 0;
}