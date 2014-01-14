package com.sec.android.iap;

import java.util.HashMap;
import java.util.Map;

public class SamsungStoreInfo {
	
	public static String getGroupID()
	{
		return "100000100936";
	}
	
	public static Map<String, String> getItemsIDs()
	{
		HashMap<String, String> res = new HashMap<String, String>();
		res.put("unlock_all", 	"000001004911");
		res.put("hint_10", 		"000001004956");
		res.put("hint_50", 		"000001004967");
		res.put("hint_1000", 	"000001004968");
		res.put("buy_4000", 	"000001004969");
		res.put("buy_3000", 	"000001004970"); 
		res.put("buy_2000", 	"000001004971"); 
		res.put("buy_1000", 	"000001004972"); 
		res.put("buy_all", 		"000001004973"); 
		res.put("kill_ads", 		"000001006056"); 
		return res;
	}
}
