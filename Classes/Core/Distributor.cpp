#include "Distributor.h"

std::string Distributor::getStoreName(const Store s)
{
    switch(s)
    {
    case iTunes:
        return "itunes";
    case GooglePlay:
        return "google-play";
    case SamsungStore:
        return "samsung-store";
    case AmazonStore:
        return "amazon-store";
    case WindowsStore8:
        return "win8-store";
    case WindowsPhoneStore:
        return "win8-phone-store";
    }
    return "default";
}
