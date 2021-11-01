#include "../../serverBackend.cpp"

int main() {
    Resources::Test = 1;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");

    Service robots;
    robots.setAddress(server);
    robots.start();

}
