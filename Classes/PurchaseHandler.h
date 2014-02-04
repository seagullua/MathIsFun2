#ifndef JUNIOR

#ifndef __EventHandler__
#define __EventHandler__


#include <ADLib/Device/ADInApp.h>



class InAppDelegate : public ADInApp::Delegate {
public:
    void purchaseSuccessful(const ADInApp::ProductID&);
    void purchaseFailed(const ADInApp::ProductID&, const ADInApp::ErrorType);
    void restorePurchasesSuccessfully();
    void restorePurchasesFailed();
};

#endif

#endif
