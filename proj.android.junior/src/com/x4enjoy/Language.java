package com.x4enjoy;

import java.util.Locale;

public class Language {
	
	public static final String getLanguage()
	{
		String lang = Locale.getDefault().getLanguage();
		return lang;
	}
}
