//
//  macos_mm.m
//  aura
//
//  Created by Thomas Borregaard Sørensen on 3/8/16.
//
//

#import <Foundation/Foundation.h>

bool GetImagePixelData(unsigned int * pcr, int cx, int cy, int iScan, CGImageRef inImage)
;


//
//char * str_clip_dup()
//{
// 
//   NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
//   
//   NSArray *classes = [[NSArray alloc] initWithObjects:[NSString class], nil];
//   
//   NSDictionary *options = [NSDictionary dictionary];
//   
//   NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
//   
//   if (copiedItems == nil)
//   {
//      
//      return NULL;
//
//   }
//   
//   if([copiedItems count] <= 0)
//   {
//    
//      return NULL;
//      
//   }
//  
//   NSString * pstr = [copiedItems objectAtIndex:0];
//   
//   return strdup([pstr UTF8String]);
//
//}


char * ns_get_default_browser_path()
{
   
   CFURLRef appURL = LSCopyDefaultApplicationURLForURL((CFURLRef)[NSURL URLWithString: @"http:"], kLSRolesAll, NULL);
   
   CFStringRef str = CFURLGetString(appURL);
   
   char * psz = strdup([(NSString *)str UTF8String]);
   
   CFRelease(appURL);
   
   //CFRelease(str);
   
   return psz;
   
}


bool ns_open_file(const char * psz)
{
   
   NSString * path = [NSString stringWithUTF8String:psz];
   
   if(path == NULL)
   {
      
      return false;
      
   }
   
   if(![[NSWorkspace sharedWorkspace] openFile: path])
   {
      
      return false;
      
   }
   
   return true;
   
}




bool mm2_get_file_image(unsigned int * pcr, int cx, int cy, int iScan, const char * psz)
{
   
   NSString * str = [NSString stringWithUTF8String:psz];
   
   if(str == NULL)
   {
      
      return false;
      
   }
   
   NSImage *image = [[NSWorkspace sharedWorkspace] iconForFile:str];
   if(image == NULL)
   {
      
      return false;
      
   }
   
   NSRect r;
   
   r.origin.x = 0;
   r.origin.y = 0;
   r.size.width = cx;
   r.size.height = cy;
   
   CGImageRef i = [image CGImageForProposedRect: &r context:nil hints:nil];
   
   bool bOk = GetImagePixelData(pcr, cx, cy, iScan,  i);
   
   //CFRelease(i);
   
   return bOk;
   
   
   
}





bool mm1_get_file_image(unsigned int * pcr, int cx, int cy, int iScan, const char * psz)
{
   
   return mm2_get_file_image(pcr, cx, cy, iScan, psz);
}
