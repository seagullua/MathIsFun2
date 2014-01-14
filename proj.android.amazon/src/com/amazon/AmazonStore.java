package com.amazon;


import android.os.AsyncTask;
import android.util.Log;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.GetUserIdResponse.GetUserIdRequestStatus;
import com.amazon.inapp.purchasing.Item.ItemType;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;
import com.amazon.inapp.purchasing.PurchasingManager;
import com.amazon.inapp.purchasing.Receipt;
import com.x4enjoy.MainActivity;
import com.x4enjoy.Notification;

public class AmazonStore extends BasePurchasingObserver {

	final static private String TAG = "AmazonStore";
	
	private String _user_id = null; 
	private String _last_buy_item = "";
	private boolean _manual_restore = false;
	private void showMessage(final String message)
	{
		Notification.threadSafeToast(message);
		//Toast.makeText(MainActivity.getContext(), message, Toast.LENGTH_LONG).show();
		Log.d(TAG, message);
	}
	private void do_storeOpening() 
	{
		//PurchasingManager.initiateGetUserIdRequest();
		
		
	}
	
	private void do_storeClose()
	{
		
	}

	private void do_buyMarketItem(final String item)
	{
		_last_buy_item = item;
		PurchasingManager.initiatePurchaseRequest(item);
	}
	
	private void do_restoreTransactions()
	{
		_manual_restore = true;
		PurchasingManager.initiatePurchaseUpdatesRequest(Offset.BEGINNING);
	}
	
	
	public static void storeOpening()
	{
		Log.d(TAG, "storeOpening()");
		getInstance().do_storeOpening();
		
	}
	public static void storeClosing()
	{
		Log.d(TAG, "storeClosing()");
		getInstance().do_storeClose();
		
	}
	public static void initialize()
	{
		Log.d(TAG, "initialize()");
		PurchasingManager.registerObserver(getInstance());
		
	}
	public static void buyMarketItem(final String item)
	{
		Log.d(TAG, "buyMarketItem()");
		
		getInstance().do_buyMarketItem(item);
	}
	public static void restoreTransactions()
	{
		Log.d(TAG, "restoreTransactions()");
		getInstance().do_restoreTransactions();
	}
	
	@Override
    public void onSdkAvailable(final boolean isSandboxMode) {
        Log.v(TAG, "onSdkAvailable recieved: Response -" + isSandboxMode);
        PurchasingManager.initiateGetUserIdRequest();
    }
	
	@Override
    public void onGetUserIdResponse(final GetUserIdResponse getUserIdResponse) {
        Log.v(TAG, "onGetUserIdResponse recieved: Response -" + getUserIdResponse);
        Log.v(TAG, "RequestId:" + getUserIdResponse.getRequestId());
        Log.v(TAG, "IdRequestStatus:" + getUserIdResponse.getUserIdRequestStatus());
        new GetUserIdAsyncTask().execute(getUserIdResponse);
    }
	
	@Override
    public void onItemDataResponse(final ItemDataResponse itemDataResponse) {
        Log.v(TAG, "onItemDataResponse recieved");
        Log.v(TAG, "ItemDataRequestStatus" + itemDataResponse.getItemDataRequestStatus());
        Log.v(TAG, "ItemDataRequestId" + itemDataResponse.getRequestId());
        //new ItemDataAsyncTask().execute(itemDataResponse);
    }
	
	@Override
    public void onPurchaseResponse(final PurchaseResponse purchaseResponse) {
        Log.v(TAG, "onPurchaseResponse recieved");
        Log.v(TAG, "PurchaseRequestStatus:" + purchaseResponse.getPurchaseRequestStatus());
        new PurchaseAsyncTask().execute(purchaseResponse);
    }
	
	@Override
    public void onPurchaseUpdatesResponse(final PurchaseUpdatesResponse purchaseUpdatesResponse) {
        Log.v(TAG, "onPurchaseUpdatesRecived recieved: Response -" + purchaseUpdatesResponse);
        Log.v(TAG, "PurchaseUpdatesRequestStatus:" + purchaseUpdatesResponse.getPurchaseUpdatesRequestStatus());
        Log.v(TAG, "RequestID:" + purchaseUpdatesResponse.getRequestId());
        new PurchaseUpdatesAsyncTask().execute(purchaseUpdatesResponse);
    }
	
	private class GetUserIdAsyncTask extends AsyncTask<GetUserIdResponse, Void, Boolean> {

        @Override
        protected Boolean doInBackground(final GetUserIdResponse... params) {
            GetUserIdResponse getUserIdResponse = params[0];

            if (getUserIdResponse.getUserIdRequestStatus() == GetUserIdRequestStatus.SUCCESSFUL) {
                final String userId = getUserIdResponse.getUserId();

                getInstance()._user_id = userId;
                return true;
            } else {
                Log.v(TAG, "onGetUserIdResponse: Unable to get user ID.");
                return false;
            }
        }

        /*
         * Call initiatePurchaseUpdatesRequest for the returned user to sync purchases that are not yet fulfilled.
         */
        @Override
        protected void onPostExecute(final Boolean result) {
            super.onPostExecute(result);
            if (result) {
                PurchasingManager.initiatePurchaseUpdatesRequest(
                		Offset.fromString(Offset.BEGINNING.toString()));
                EventbillingSupported(true);
            }
            else
            {
            	EventbillingSupported(false);
            }
        }
    }
	
	private void printReceipt(final Receipt receipt) {
        Log.v(
            TAG,
            String.format("Receipt: ItemType: %s Sku: %s SubscriptionPeriod: %s", receipt.getItemType(),
                receipt.getSku(), receipt.getSubscriptionPeriod()));
    }
	
	private class PurchaseAsyncTask extends AsyncTask<PurchaseResponse, Void, Boolean> {

        @Override
        protected Boolean doInBackground(final PurchaseResponse... params) {
            final PurchaseResponse purchaseResponse = params[0];            
            final String userId = getInstance()._user_id;
            
            if (!purchaseResponse.getUserId().equals(userId)) {
            	Log.w(TAG, "Users differ "+userId+" "+purchaseResponse.getUserId());
                // currently logged in user is different than what we have so update the state
            	getInstance()._user_id = purchaseResponse.getUserId();                
                PurchasingManager.initiatePurchaseUpdatesRequest(Offset.fromString(Offset.BEGINNING.toString()));                
            }
            
            switch (purchaseResponse.getPurchaseRequestStatus()) {
            case SUCCESSFUL:
                /*
                 * You can verify the receipt and fulfill the purchase on successful responses.
                 */
                final Receipt receipt = purchaseResponse.getReceipt();
                String key = "";
                Log.d(TAG, "Success purchase");
                EventmarketPurchase(receipt.getSku());
                
                
                printReceipt(purchaseResponse.getReceipt());
                return true;
            case ALREADY_ENTITLED:
                /*
                 * If the customer has already been entitled to the item, a receipt is not returned.
                 * Fulfillment is done unconditionally, we determine which item should be fulfilled by matching the
                 * request id returned from the initial request with the request id stored in the response.
                 */
            	Log.d(TAG, "Entiteled");
            	EventmarketPurchase(_last_buy_item);
                
                return true;
            case FAILED:
                /*
                 * If the purchase failed for some reason, (The customer canceled the order, or some other
                 * extraneous circumstance happens) the application ignores the request and logs the failure.
                 */
            	//showMessage("Purchase failed");
            	
            	EventmarketPurchaseCancelled(_last_buy_item);
                //Log.v(TAG, "Failed purchase for request" + baseActivity.requestIds.get(purchaseResponse.getRequestId()));
                return false;
            case INVALID_SKU:
                /*
                 * If the sku that was purchased was invalid, the application ignores the request and logs the failure.
                 * This can happen when there is a sku mismatch between what is sent from the application and what
                 * currently exists on the dev portal.
                 */
            	showMessage("No item in the store");
            	EventmarketPurchaseCancelled(_last_buy_item);
                return false;
            }
            return false;
        }

        
    }
	
	private class PurchaseUpdatesAsyncTask extends AsyncTask<PurchaseUpdatesResponse, Void, Boolean> {

        @Override
        protected Boolean doInBackground(final PurchaseUpdatesResponse... params) {
            final PurchaseUpdatesResponse purchaseUpdatesResponse = params[0];
            
            final String userId = getInstance()._user_id;
            if (!purchaseUpdatesResponse.getUserId().equals(userId)) {
                return false;
            }
            /*
             * If the customer for some reason had items revoked, the skus for these items will be contained in the
             * revoked skus set.
             */
            

            switch (purchaseUpdatesResponse.getPurchaseUpdatesRequestStatus()) {
            case SUCCESSFUL:
                boolean one_restored = false;
            	for (final Receipt receipt : purchaseUpdatesResponse.getReceipts()) {
                    final String sku = receipt.getSku();
                    //final String key = getKey(sku);
                    if (receipt.getItemType() == ItemType.ENTITLED) 
                    {

                        EventmarketPurchase(sku);
                        one_restored = true;

                    }
                    printReceipt(receipt);
                }
                if(one_restored && _manual_restore)
                {
                	_manual_restore = false;
                	EventrestoreTransactions(true);
                }
                return true;
            case FAILED:
                /*
                 * On failed responses the application will ignore the request.
                 */
            	if(_manual_restore)
            	{
            		_manual_restore = false;
            		EventrestoreTransactions(false);
            	}
                return false;
            }
            return false;
        }

        
    }
	
	
	private static native void EventbillingSupported(boolean support);
	private static native void EventrestoreTransactions(boolean success);
	private static native void EventmarketPurchase(final String item);
	private static native void EventmarketPurchaseCancelled(final String item);
	
	private static AmazonStore _instance = new AmazonStore();
	
	public AmazonStore()
	{
		super(MainActivity.getActivity());
	}
	
	private static AmazonStore getInstance()
	{
		return _instance;
	}
}
