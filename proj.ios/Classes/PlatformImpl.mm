//
//  PlatformImpl.m
//  TestCpp
//
//  Created by admin on 14.06.13.
//
//
#import <Foundation/Foundation.h>
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"
#import "Flurry.h"
void Browser::openURL(const URLId id)
{
    if(id == 300)
    {
        
        NSString *url = [NSString stringWithString: @"mailto:feedback@4enjoy.com?subject=Math%20Is%20Fun"];
        [[UIApplication sharedApplication] openURL: [NSURL URLWithString: url]];
    }
    else
    {

        NSString* url= [NSString stringWithFormat:@"http://4enjoy.com/r/ios/%i/", id];

        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
        
        
    }
    //cocos2d::CCLog("Open URL: %d", id);
    //Notification::notify(Notification::RestorePurchasesOk);
}

void Statistics::statisticEvent(const StatisticsEvent id)
{
    NSString* action=@"";
    if (id==100)
        action=@"OnMoreLevelsClick";
    else if (id==200)
        action=@"OnDevelopersClick";
    else if (id==300)
        action=@"UseHint";
    else if (id==400)
        action=@"SkipLevel";
    else if (id==500)
        action=@"OnLevelComplete";
    
    [Flurry logEvent:action];
}

void Statistics::detailStatisticEvent(const StatisticsEvent id, const unsigned int level_id, const unsigned int collection_id, const unsigned int stamps_num)
{
    NSString* action=@"Undifined Action";
    if (id==400)
        action=@"OnSkipLevel";
    else if (id==500)
        action=@"OnLevelComplete";
    
    NSDictionary *params =
    [NSDictionary dictionaryWithObjectsAndKeys:
     
     [NSString stringWithFormat:@"%i", level_id],
     @"levelID",
     
     [NSString stringWithFormat:@"%i", collection_id],
     @"collectionID",
     
     [NSString stringWithFormat:@"%i", stamps_num],
     @"stampsNum",
     
     nil];
    [Flurry logEvent:action withParameters:params];
    
}

Language::Languages Language::getDeviceLanguage()
{
    NSString* lang = [[NSLocale preferredLanguages] objectAtIndex:0];
    
    if([lang isEqualToString:@"ru"] || [lang isEqualToString:@"uk"])
        return Language::Russian;
    
    return Language::English;
}

void Notification::notify(const Type t)
{
    
    NSString* title = @"Restore Purchases";
    if(Language::getLanguage() == Language::Russian)
        title = @"Восстановление покупок";

    NSString* text;
    if(t == Notification::RestorePurchasesOk)
    {
        if(Language::getLanguage() == Language::Russian)
            text = @"Покупки восстановлены успешно!";
        else
            text = @"Purchases were restored successfully.";
    }
    else
    {
        if(Language::getLanguage() == Language::Russian)
            text = @"Во время восстановления покупок произошла ошибка. Попробуйте позже.";
        else
            text = @"Restoring purchases failed. Please try again later.";
    }
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:text
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];    //cocos2d::CCLog("Notification ID %d", t);
    
}