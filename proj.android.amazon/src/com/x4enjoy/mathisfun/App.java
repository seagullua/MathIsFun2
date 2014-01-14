package com.x4enjoy.mathisfun;


import android.app.Activity;
import android.app.Application;
import android.content.Context;

public class App extends Application{

    private static Context context;
    private static Activity _main_activity = null;
    
    public void onCreate(){
        super.onCreate();
        App.context = getApplicationContext();
    }

    public static void setActivity(Activity a)
    {
    	_main_activity = a;
    }
    
    public static Activity getActivity()
    {
    	return _main_activity;
    }
    
    public static Context getAppContext() {
        return App.context;
    }
    
    public static String getStringRes(int id)
    {
    	return App.context.getResources().getString(id);
    }
}