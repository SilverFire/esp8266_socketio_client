#include <IPAddress.h>

class WiFi_Client
{
    public:
        WiFi_Client(const char* ssid, const char *passphrase);
        bool connect();
        IPAddress localIP();
    private:
        const char* ssid;
        const char* passphrase;
};
