//
//  macos_mm.m
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 3/8/16.
//
//

#import <Foundation/Foundation.h>

size_t ns_string(char * psz, size_t s, NSString * pstr);

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
   
   CFURLRef appURL = LSCopyDefaultApplicationURLForURL((__bridge CFURLRef)[NSURL URLWithString: @"http:"], kLSRolesAll, NULL);
   
   CFStringRef str = CFURLGetString(appURL);
   
   char * psz = strdup([(__bridge NSString *)str UTF8String]);
   
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







char * mm_ca2_command_line()
{
   
   NSString *ca2_command_line = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"ca2_command_line"];
   
   if(ca2_command_line == NULL)
   {
      
      return strdup("");
      
   }
   
   return strdup([ca2_command_line UTF8String]);
   
}





bool GetImagePixelData(unsigned int * pcr, int cx, int cy, int iScan, CGImageRef inImage)
;




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




void ns_app_terminate()
{
   
   [NSApp terminate:nil];
   
}


size_t ns_executable_path(char * psz, size_t s)
{
   
   return ns_string(psz, s, [[NSBundle mainBundle] executablePath]);
   
}

size_t ns_string(char * psz, size_t s, NSString * pstr)
{
   
   size_t len = strlen([pstr UTF8String]);
   
   if(psz == NULL || s == 0)
   {
   
      return len;
      
   }
   
   strncpy(psz, [pstr UTF8String], s);
   
   if(len < s)
   {
    
      return len;
      
   }

   return s;
   
}


size_t ns_bundle_identifier(char * psz, size_t iSize)
{
   
   return ns_string(psz, iSize, [[NSBundle mainBundle] bundleIdentifier]);
   
}



void ns_Sleep(unsigned int uiMillis)
{

   [NSThread sleepForTimeInterval: ((double) uiMillis / 1000.0) ];

}
