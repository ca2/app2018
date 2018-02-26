//
//  user_notify_icon.m
//  core
//
//  Created by Camilo Sasuke Tsumanuma on 3/17/16.
//  Copyright © 2016 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//


#import "user_notify_icon_mm.h"
#import "user_notify_icon_mm_bridge.h"


@implementation user_notify_icon


//
// initWithContentRect:styleMask:backing:defer:screen:
//
// Init method for the object.
//
- (id)init:(NSString*)strIconFile bridge:(user_notify_icon_mm_bridge *)pbridge
{
   m_pbridge = pbridge;
   // http://stackoverflow.com/questions/3409985/how-to-create-a-menubar-application-for-mac
   statusItem = [[NSStatusBar systemStatusBar]
                  statusItemWithLength:NSVariableStatusItemLength];
   [statusItem setHighlightMode:YES];
   
   [statusItem setImage:[[NSImage alloc ]initByReferencingFile:strIconFile]];
   jcMenu = [[NSMenu alloc] initWithTitle:@"menubar_menu"];
   
   closeItem = [[NSMenuItem alloc] initWithTitle:@"Close"                                             action: @selector(play:) keyEquivalent:@""];
   
   [closeItem setTarget:self];
   
   quitItem = [[NSMenuItem alloc] initWithTitle:@"Quit"
                                                   action: @selector(play:) keyEquivalent:@""];
   [quitItem setTarget:self];
   
   [jcMenu addItem:closeItem];
   
   [jcMenu addItem:quitItem];
   
   [jcMenu setDelegate:self];
   
   [statusItem setMenu:jcMenu];
   [statusItem setEnabled:YES];
   
   return self;
   
}

- (void) menuWillOpen:(NSMenu *)menu
{
   
   if(m_pbridge->notify_icon_frame_is_opened())
   {
    
      [closeItem setTitle:@"Close"];
      
   }
   else
   {

      [closeItem setTitle:@"Open"];

   
   }
   
}

//
// dealloc
//
// Releases instance memory.
//
- (void)dealloc
{
   
}
- (void)play:(id)sender
{
   NSMenuItem * pitem = (NSMenuItem *) sender;
   if(pitem == closeItem)
   {
      if(m_pbridge->notify_icon_frame_is_opened())
      {
         m_pbridge->notify_icon_play("close");
      }
      else
      {
         m_pbridge->notify_icon_play("open");
      }
   }
   else if(pitem == quitItem)
   {
      //[NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
      m_pbridge->notify_icon_play("quit");
   }
}

- (void)close
{
 
   
   [[NSStatusBar systemStatusBar] removeStatusItem: statusItem];
   
}

@end
