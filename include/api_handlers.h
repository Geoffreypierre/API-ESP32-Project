#ifndef API_HANDLERS_H
#define API_HANDLERS_H

#include <WebServer.h>

namespace APIHandlers
{
    void setupRoutes(WebServer &server);

    void handleGetSensors();
    void handleGetLight();
    void handleGetTemperature();

    void handleLedRed();
    void handleLedGreen();
    void handleLedBlue();

    void handleLinkBlueToLight();
    void handleUnlinkBlueFromLight();
    void handleLinkRedToTemp();
    void handleUnlinkRedFromTemp();

    void sendError(int code, const char *message);
}

#endif