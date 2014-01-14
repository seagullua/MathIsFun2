package com.sec.android.iap;


import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;
import java.util.Map;

import com.sec.android.iap.sample.helper.SamsungIapHelper;
import com.sec.android.iap.sample.helper.SamsungIapHelper.OnGetInboxListListener;
import com.sec.android.iap.sample.helper.SamsungIapHelper.OnInitIapListener;
import com.sec.android.iap.sample.vo.InBoxVO;

import com.x4enjoy.MainActivity;
import com.x4enjoy.Notification;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class SamsungStore implements OnInitIapListener, OnGetInboxListListener {

	private int mIapMode = SamsungIapHelper.IAP_MODE_COMMERCIAL;
	private String mIapGroupID = "";
	private Map<String, String> mItemsCodes = null;
	private boolean _billing_loaded = false;
	private boolean _billing_supported = false;
	private String _waiting_purchase = null;
	private boolean _waiting_restore_transaction = false;
	
	
	private void showMessage(final String message)
	{
		Notification.threadSafeToast(message);
		//Toast.makeText(MainActivity.getContext(), message, Toast.LENGTH_LONG).show();
		Log.d("SamsungIAP", message);
	}
	private void do_storeOpening() 
	{
		if(!_billing_supported)
		{
			MainActivity.setOnActivityResultListener(new MainActivity.onActivityResult() {
				public void action(int _requestCode, int _resultCode, Intent _intent) {
					SamsungStore.this.onActivityResult(_requestCode, _resultCode, _intent);
				}
			});
			
			mSamsungIapHelper = SamsungIapHelper.getInstance( MainActivity.getActivity() , mIapMode );
			mSamsungIapHelper.setOnInitIapListener( this );
			mSamsungIapHelper.setOnGetInboxListListener( this );
			
			if( true == mSamsungIapHelper.isInstalledIapPackage( MainActivity.getContext() ) )
	        {
	
	            if( true == mSamsungIapHelper.isValidIapPackage( MainActivity.getContext() ) )
	            {
	                mSamsungIapHelper.startAccountActivity( MainActivity.getActivity() );
	            }          
	            else
	            {
	            	showMessage("Invalid IAP package");
	            	EventbillingSupported(false);
	            	_billing_loaded = true;
	            }
	        } 
	        else
	        {
	            mSamsungIapHelper.installIapPackage( MainActivity.getActivity() );
	        }
		}
	}
	
	private void do_storeClose()
	{
		//mSamsungIapHelper = null;
	}
	private void do_buyItem(final String item)
	{
		String code = mItemsCodes.get(item);
		if(code != null)
		{
			mSamsungIapHelper.startPurchase( 
		            MainActivity.getActivity(), 
		            SamsungIapHelper.REQUEST_CODE_IS_IAP_PAYMENT, 
		            mIapGroupID,
		            code );
		}
		else
		{
			showMessage("Wrong item");
		}
		
	}
	private void do_buyMarketItem(final String item)
	{
		if(_billing_loaded)
		{
			if(_billing_supported)
			{
				do_buyItem(item);
			}
			else
			{
				showMessage("IAP is not supported");
				EventmarketPurchaseCancelled(item);
			}
		}
		else
		{
			_waiting_purchase = item;
			if(!_billing_supported)
				do_storeOpening();
			
		}
		
	}
	
	private void request_restoreTransaction()
	{
		Date d = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat( "yyyyMMdd",
                                                     Locale.getDefault() );
        String today = sdf.format( d );
        
        mSamsungIapHelper.safeGetItemInboxTask( 
        		MainActivity.getActivity(), 
        		mIapGroupID,
                1,
                100,
                "20100101",
                today); 
	}
	
	private void do_restoreTransactions()
	{
		if(_billing_loaded)
		{
			if(_billing_supported)
			{
				request_restoreTransaction();
			}
			else
			{
				showMessage("IAP is not supported");
				EventrestoreTransactions(false);
			}
		}
		else
		{
			_waiting_restore_transaction = true;
			if(!_billing_supported)
				do_storeOpening();
			
		}
		
	}
	
	
	@Override
    public void OnSucceedGetInboxList( ArrayList<InBoxVO>  _inboxList )
    {
        Log.i( "SamsungRestore", "getInboxList has finished successfully" );
        String non_consumable = "01";
        
        for(InBoxVO i : _inboxList)
        {
        	if(non_consumable.equals(i.getType()))
        	{
        		String item_id = getApplicationItemID(i.getItemId());
        		Log.i("SamsungRestore", item_id);
        		
        		EventmarketPurchase(item_id);
        		
        	}
        }
        EventrestoreTransactions(true);
    }
	
	private SamsungIapHelper  mSamsungIapHelper  = null;
	
	
	public static void storeOpening()
	{
		Log.d("SamsungStore", "storeOpening()");
		getInstance().do_storeOpening();
		
	}
	public static void storeClosing()
	{
		Log.d("SamsungStore", "storeClosing()");
		getInstance().do_storeClose();
		
	}
	public static void initialize()
	{
		Log.d("SamsungStore", "initialize()");
		getInstance().mIapGroupID = SamsungStoreInfo.getGroupID();
		getInstance().mItemsCodes = SamsungStoreInfo.getItemsIDs();
		//getInstance().do_storeOpening();
	}
	public static void buyMarketItem(final String item)
	{
		Log.d("SamsungStore", "buyMarketItem()");
		
		getInstance().do_buyMarketItem(item);
	}
	public static void restoreTransactions()
	{
		Log.d("SamsungStore", "restoreTransactions()");
		getInstance().do_restoreTransactions();
	}
	
	private String getApplicationItemID(String samsung_id)
	{
		for (Map.Entry<String, String> entry : mItemsCodes.entrySet()) 
		{
		    if(samsung_id.equals(entry.getValue()))
		    	return entry.getKey();
		}
		return samsung_id;
	}
	
	
	
    private void onActivityResult
    (   
        int     _requestCode,
        int     _resultCode,
        Intent  _intent
    )
    {
        switch( _requestCode )
        {

            case SamsungIapHelper.REQUEST_CODE_IS_IAP_PAYMENT:
            {
                if( null == _intent )
                {
                    break;
                }
                
                Bundle extras         = _intent.getExtras();
                
                String itemId         = "";
                String thirdPartyName = "";
                
                // payment success   : 0
                // payment cancelled : 1
                // ============================================================
                int statusCode        = 1;
                // ============================================================
                
                String errorString    = "";
                //PurchaseVO purchaseVO = null;
                
                //    If there is bundle passed from IAP
                // ------------------------------------------------------------
                if( null != extras )
                {
                    thirdPartyName = extras.getString(
                                  SamsungIapHelper.KEY_NAME_THIRD_PARTY_NAME );
                    
                    statusCode = extras.getInt( 
                                       SamsungIapHelper.KEY_NAME_STATUS_CODE );
                    
                    errorString = extras.getString( 
                                      SamsungIapHelper.KEY_NAME_ERROR_STRING );
                    
                    itemId = extras.getString(
                                           SamsungIapHelper.KEY_NAME_ITEM_ID );
                    
                    // print log : Please remove before release
                    // --------------------------------------------------------
                    Log.i( "SamsungStore", "3rdParty Name : " + thirdPartyName + "\n" +
                                "ItemId        : " + itemId + "\n" +
                                "StatusCode    : " + statusCode + "\n" +
                                "errorString   : " + errorString );
                    // --------------------------------------------------------
                }
                // ------------------------------------------------------------
                //    If there is no bundle passed from IAP
                // ------------------------------------------------------------
                else
                {
                    showMessage("No bundle from IAP");
                }
                // ------------------------------------------------------------
                
                // 3) 결제가 성공했을 경우
                //    If payment was not cancelled
                // ------------------------------------------------------------
                if( Activity.RESULT_OK == _resultCode )
                {
                    // a. IAP 에서 넘어온 결제 결과가 성공인 경우 verifyurl 과 
                    //    purchaseId 값으로 서버에 해당 결제가 유효한 지 확인한다.
                    //    if Payment succeed
                    // --------------------------------------------------------
                    if( statusCode == SamsungIapHelper.IAP_ERROR_NONE )
                    {
                        // 정상적으로 결제가 되었으므로 PurchaseVO를 생성한다.
                        // make PurcahseVO
                        // ----------------------------------------------------
//                        purchaseVO = new PurchaseVO( extras.getString(
//                                   SamsungIapHelper.KEY_NAME_RESULT_OBJECT ) );
                        // ----------------------------------------------------
                        
                        // 결제 유효성을 확인한다.
                        // verify payment result
                        // ----------------------------------------------------
                        /*mSamsungIapHelper.verifyPurchaseResult( 
                                                          MainActivity.getActivity(),
                                                          purchaseVO );*/
                        EventmarketPurchase(getApplicationItemID(itemId));
                        // ----------------------------------------------------
                    }
                    // --------------------------------------------------------
                    // b. IAP 에서 넘어온 결제 결과가 실패인 경우 에러메시지를 출력
                    //    Payment failed 
                    // --------------------------------------------------------
                    else
                    {
                    	showMessage("Error: " + errorString);
                    	EventmarketPurchaseCancelled(getApplicationItemID(itemId));
                    }
                    // --------------------------------------------------------
                }
                // ------------------------------------------------------------
                // 4) 결제를 취소했을 경우
                //    If payment was cancelled
                // ------------------------------------------------------------
                else if( Activity.RESULT_CANCELED == _resultCode )
                {
                	EventmarketPurchaseCancelled(getApplicationItemID(itemId));
                }
                // ------------------------------------------------------------
                
                break;
            }
            // ================================================================
            
            // 2. 삼성 어카운트 계정 인증 결과 처리
            //    treat result of SamsungAccount authentication
            // ================================================================
            case SamsungIapHelper.REQUEST_CODE_IS_ACCOUNT_CERTIFICATION :
            {
                // 2) 삼성 계정 인증결과 성공인 경우
                //    If SamsungAccount authentication is succeed 
                // ------------------------------------------------------------
                if( Activity.RESULT_OK == _resultCode )
                {
                    // IAP Service 바인드 및 초기화 진행
                    // start binding and initialization for IAPService 
                    // --------------------------------------------------------
                    bindIapService();
                    // --------------------------------------------------------
                }
                // ------------------------------------------------------------
                // 3) 삼성 계정 인증을 취소했을 경우
                //    If SamsungAccount authentication is cancelled
                // ------------------------------------------------------------
                else if( Activity.RESULT_CANCELED == _resultCode )
                {
                    
                }
                // ------------------------------------------------------------
                
                break;
            }
            // ================================================================
        }
    }
	
	

    public void bindIapService()
    {

        mSamsungIapHelper.bindIapService(new SamsungIapHelper.OnIapBindListener()
        {
            @Override
            public void onBindIapFinished( int result )
            {
                
                if ( result == SamsungIapHelper.IAP_RESPONSE_RESULT_OK )
                {
                    mSamsungIapHelper.safeInitIap(MainActivity.getActivity());
                }
                else
                {
                    showMessage("IAP Service bind failed");
                }
            }
        });
    }
	
	
	private static native void EventbillingSupported(boolean support);
	private static native void EventrestoreTransactions(boolean success);
	private static native void EventmarketPurchase(final String item);
	private static native void EventmarketPurchaseCancelled(final String item);
	
	private static SamsungStore _instance = new SamsungStore();
	private static SamsungStore getInstance()
	{
		return _instance;
	}
	
	@Override
    public void onSucceedInitIap()
    {
		EventbillingSupported(true);
		_billing_loaded = true;
		_billing_supported = true;
		
		if(_waiting_purchase != null)
		{
			String item = _waiting_purchase;
			_waiting_purchase = null;
			do_buyItem(item);
		}
		
		if(_waiting_restore_transaction == true)
		{
			_waiting_restore_transaction = false;
			request_restoreTransaction();
		}
		
    }
	
	static {
        System.loadLibrary("hellocpp");
    }
}
