#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H
#include <string>
class Distributor
{
public:
    enum Store
    {
        iTunes=1,
        GooglePlay=2,
        SamsungStore=3,
        AmazonStore=4,
        WindowsStore8=5,
        WindowsPhoneStore=6
    };
    static std::string getStoreName(const Store);

    static Store getCurrentStore();
};

#endif // DISTRIBUTOR_H
