//
//  user_notify_icon_mm_bridge.m
//  core
//
//  Created by Camilo Sasuke Tsumanuma on 3/17/16.
//  Copyright © 2016 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "user_notify_icon_mm_bridge.h"


void user_notify_icon_mm_bridge::notify_icon_init(const char * strIconFile)
{
   
   NSString * str = [[NSString alloc] initWithCString:strIconFile encoding:NSUTF8StringEncoding];
   
   m_pnotifyicon = (__bridge_retained void *) [[user_notify_icon alloc] init: str bridge:this];
   
}


void user_notify_icon_mm_bridge::notify_icon_destroy()
{
   [((__bridge user_notify_icon *) m_pnotifyicon) close];
   //NSMenu* rootMenu = [NSApp mainMenu];
   // sender is an NSMenuItem
   //[rootMenu removeItem: menu];
   //[rootMenu removeItemAtIndex: iIndex];
   
}

