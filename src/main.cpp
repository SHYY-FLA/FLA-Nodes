#include <iostream>

#include <version.h>
#include <network/network.h>

using namespace std;

int main() {

    cout << version() << endl;

    int socket = create();

    return 0;
}