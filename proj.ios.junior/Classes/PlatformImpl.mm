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

void Browser::openURL(const URLId id)
{
}

void Statistics::statisticEvent(const StatisticsEvent id)
{
    
}

void Statistics::detailStatisticEvent(const StatisticsEvent id, const unsigned int level_id, const unsigned int collection_id, const unsigned int stamps_num)
{
    
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
    
    
}