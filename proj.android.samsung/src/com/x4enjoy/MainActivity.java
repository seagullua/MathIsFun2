package com.x4enjoy;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import com.x4enjoy.mathisfun.App;

public class MainActivity {
	public static Activity getActivity()
	{
		return App.getActivity();
	}
	
	public static Context getContext()
	{
		return App.getAppContext();
	}
	
	public static void onActivityResultAction(int _requestCode,
	        int     _resultCode,
	        Intent  _intent)
    {
		if(_listener != null)
		{
			_listener.action(_requestCode, _resultCode, _intent);
		}
    }
	
	public static void setOnActivityResultListener(onActivityResult listener)
	{
		_listener = listener;
	}
	
	private static onActivityResult _listener = null; 
	
	public interface onActivityResult
	{
		void action(int _requestCode,
		        int     _resultCode,
		        Intent  _intent);
	}
}
