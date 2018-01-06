//
//  RoundWindowFrameView.m
//  RoundWindow
//
//  Created by Matt Gallagher on 12/12/08.
//  Copyright 2008 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//
#import "ios_mm.h"


void __ios_do_events();


@implementation RoundWindowFrameView

//
// resizeRect
//
// Returns the bounds of the resize box.
//
- (CGRect)resizeRect
{
   
//	const CGFloat resizeBoxSize = 16.0;
   
//	const CGFloat contentViewPadding = 5.5;
	
//	CGRect contentViewRect = [[self window] contentRectForFrameRect:[[self window] frame]];
    
//    CGRect contentViewRect;
   
	/*CGRect resizeRect = NSMakeRect(
		NSMaxX(contentViewRect) + contentViewPadding,
		NSMinY(contentViewRect) - resizeBoxSize - contentViewPadding,
		resizeBoxSize,
		resizeBoxSize);
	*/
    
    CGRect resizeRect;
    
    resizeRect.origin.x = 0;
    resizeRect.origin.y = 0;
    resizeRect.size.width = 0;
    resizeRect.size.height = 0;
    
	return resizeRect;
   
}

- (void)mouseUp:(UIEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
//   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
    CGPoint point;
    
    point.x = 0;
    point.y = 0;
    
   CGRect e = [[UIScreen mainScreen] bounds];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_up(x, y);
   
   return;
   
}


- (void)mouseMoved:(UIEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
//   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
    
    CGPoint point;
    
    point.x = 0;
    point.y = 0;
   
   CGRect e = [[UIScreen mainScreen] bounds];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_moved(x, y);
   
   return;
   
}


- (void)mouseDragged:(UIEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
//   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
    
    CGPoint point;
    
    point.x = 0;
    
    point.y = 0;
   
   CGRect e = [[UIScreen mainScreen] bounds];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_dragged(x, y);
   
   return;
   
   
}


//
// mouseDown:
//
// Handles mouse clicks in our frame. Two actions:
//	- click in the resize box should resize the window
//	- click anywhere else will drag the window.
//
- (void)mouseDown:(UIEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
//   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
    CGPoint point;
    
    point.x = 0;
    point.y = 0;
   
   CGRect e = [[UIScreen mainScreen] bounds];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_down(x, y);
   
   return;
   
////	CGPoint pointInView = [self convertPoint:[event locationInWindow] fromView:nil];
//    
//	CGPoint pointInView;
//    
//    pointInView.x = 0;
//    pointInView.y = 0;
//    
//	BOOL resize = NO;
////	if (CGPointInRect(pointInView, [self resizeRect]))
////	{
////		resize = YES;
////	}
//	
//	UIWindow *window = [self window];
////	CGPoint originalMouseLocation = [window convertBaseToScreen:[event locationInWindow]];
////    CGPoint point;
//    point.x = 0;
//    point.y = 0;
//	CGRect originalFrame = [window frame];
//	
//    while (YES)
//	{
//		//
//		// Lock focus and take all the dragged and mouse up events until we
//		// receive a mouse up.
//		//
////        UIEvent *newEvent = [window
////			nextEventMatchingMask:(NSLeftMouseDraggedMask | NSLeftMouseUpMask)];
//        
////        UIEvent * newEvent = NULL;
//		
////        if ([newEvent type] == NSLeftMouseUp)
////		{
////			break;
////		}
//		
//		//
//		// Work out how much the mouse has moved
//		//
////		CGPoint newMouseLocation = [window convertBaseToScreen:[newEvent locationInWindow]];
////		CGPoint delta = NSMakePoint(
////			newMouseLocation.x - originalMouseLocation.x,
////			newMouseLocation.y - originalMouseLocation.y);
//        
////        CGPoint newMouseLocation;
//        CGPoint delta;
//		
//		CGRect newFrame = originalFrame;
//		
//		if (!resize)
//		{
//			//
//			// Alter the frame for a drag
//			//
//			newFrame.origin.x += delta.x;
//			newFrame.origin.y += delta.y;
//		}
//		else
//		{
//			//
//			// Alter the frame for a resize
//			//
//			newFrame.size.width += delta.x;
//			newFrame.size.height -= delta.y;
//			newFrame.origin.y += delta.y;
//			
//			//
//			// Constrain to the window's min and max size
//			//
//            /*
//			CGRect newContentRect = [window contentRectForFrameRect:newFrame];
//			NSSize maxSize = [window maxSize];
//			NSSize minSize = [window minSize];
//			if (newContentRect.size.width > maxSize.width)
//			{
//				newFrame.size.width -= newContentRect.size.width - maxSize.width;
//			}
//			else if (newContentRect.size.width < minSize.width)
//			{
//				newFrame.size.width += minSize.width - newContentRect.size.width;
//			}
//			if (newContentRect.size.height > maxSize.height)
//			{
//				newFrame.size.height -= newContentRect.size.height - maxSize.height;
//				newFrame.origin.y += newContentRect.size.height - maxSize.height;
//			}
//			else if (newContentRect.size.height < minSize.height)
//			{
//				newFrame.size.height += minSize.height - newContentRect.size.height;
//				newFrame.origin.y -= minSize.height - newContentRect.size.height;
//			}
//             */
//		}
//		
////		[window setFrame:newFrame display:YES animate:NO];
//	}
}

- (BOOL) isFlipped
{
   return YES;
}



- (BOOL)acceptsFirstResponder
{
   return TRUE;
}

//- (BOOL) acceptsFirstMouse:(NSEvent *)theEvent
//{
  // [self mouseDown: theEvent];
   //return YES;
//}


-(void)on_text : (NSString *) text
{

   round_window * p = m_roundwindow->m_pwindow;
   
   const char * pszText = [text UTF8String];
   
   if(p->round_window_on_text(pszText))
         return;
   
}


-(void)on_sel : (UITextRange *) sel
{

   round_window * p = m_roundwindow->m_pwindow;
   
   if(sel == NULL)
   {

      p->round_window_on_sel_text(-1, -1);
      
      return;

   }
   
   UITextPosition * beg = [self beginningOfDocument];
   
   UITextPosition * selbeg = [[self selectedTextRange] start];
   
   UITextPosition * selend = [[self selectedTextRange] start];
   
   long iBeg = [self offsetFromPosition: beg toPosition: selbeg];
   
   long iEnd = [self offsetFromPosition: beg toPosition: selend];

   p->round_window_on_sel_text(iBeg, iEnd);
   
}


- (BOOL)shouldChangeTextInRange:(UITextRange *)range
                replacementText:(NSString *)text

{
   
   return YES;
   
}


- (void)deleteBackward
{

//   ::user::e_key ekey = ::user::key_back;
//
//   round_window * p = m_roundwindow->m_pwindow;
//
//   if(p->round_window_key_down(ekey))
//      return;
   
   [super deleteBackward];
   
   round_window * p = m_roundwindow->m_pwindow;
   
   const char * pszText = [[self text] UTF8String];
   
   if(p->round_window_on_text(pszText))
      return;
   
}

- (void)keyDown:(UIEvent *)event {
   

   ::user::e_key ekey = event_key(event);
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p->round_window_key_down(ekey))
      return;
   
//   [super keyDown:event];
   
}

- (void)keyUp:(UIEvent *)event {
   
   
   ::user::e_key ekey = event_key(event);
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p->round_window_key_up(ekey))
      return;
   
//   [super keyUp:event];
   
}


- (BOOL)textViewShouldBeginEditing:(UITextView * ) pview
{
   
   return YES;
   
}



- (void)flagsChanged:(UIEvent *)event
{

   round_window * p = m_roundwindow->m_pwindow;
   
//   if([event modifierFlags] & NSShiftKeyMask)
   {
      if(!m_bShift)
      {
         m_bShift = true;
         if(p->round_window_key_down(::user::key_shift))
            return;
      }
   }
//   else
   {
      if(m_bShift)
      {
         m_bShift = false;
         if(p->round_window_key_up(::user::key_shift))
            return;
      }
   }

//   if([event modifierFlags] & NSControlKeyMask)
   {
      if(!m_bControl)
      {
         m_bControl = true;
         if(p->round_window_key_down(::user::key_control))
            return;
      }
   }
//   else
   {
      if(m_bControl)
      {
         m_bControl = false;
         if(p->round_window_key_up(::user::key_control))
            return;
      }
   }
 
//   if([event modifierFlags] & NSAlternateKeyMask)
   {
      if(!m_bAlt)
      {
         m_bAlt = true;
         if(p->round_window_key_down(::user::key_alt))
            return;
      }
   }
//   else
   {
      if(m_bAlt)
      {
         m_bAlt = false;
         if(p->round_window_key_up(::user::key_alt))
            return;
      }
   }

//   [super flagsChanged:event];
    
}





//
// resizeRect
//
// Returns the bounds of the resize box.
//
/*- (CGRect)resizeRect
{
   
	const CGFloat resizeBoxSize = 16.0;
   
	const CGFloat contentViewPadding = 5.5;
	
	CGRect contentViewRect = [[self window] contentRectForFrameRect:[[self window] frame]];
   
	CGRect resizeRect = NSMakeRect(
		NSMaxX(contentViewRect) + contentViewPadding,
		NSMinY(contentViewRect) - resizeBoxSize - contentViewPadding,
		resizeBoxSize,
		resizeBoxSize);
	
	return resizeRect;
   
}*/


- (void)addGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
{
   /*[super addGestureRecognizer:gestureRecognizer];
   // Check the new gesture recognizer is the same kind as the one we want to implement
   // Note:
   // This works because `UITextTapRecognizer` is a subclass of `UITapGestureRecognizer`
   // and the text view has some `UITextTapRecognizer` added :)
   if ([gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]]) {
      UITapGestureRecognizer *tgr = (UITapGestureRecognizer *)gestureRecognizer;
      if ([tgr numberOfTapsRequired] == 1 &&
          [tgr numberOfTouchesRequired] == 1) {
         // If found then add self to its targets/actions
         [tgr addTarget:self action:@selector(_handleOneFingerTap:)];
      }
   }*/
}
- (void)removeGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
{
   // Check the new gesture recognizer is the same kind as the one we want to implement
   // Read above note
/*   if ([gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]]) {
      UITapGestureRecognizer *tgr = (UITapGestureRecognizer *)gestureRecognizer;
      if ([tgr numberOfTapsRequired] == 1 &&
          [tgr numberOfTouchesRequired] == 1) {
         // If found then remove self from its targets/actions
         [tgr removeTarget:self action:@selector(_handleOneFingerTap:)];
      }
   }
   [super removeGestureRecognizer:gestureRecognizer];
 
 
 */
   
   
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
   
      [super touchesEnded:touches withEvent:event];
    
    BOOL allTouchesEnded = ([touches count] == [[event touchesForView:self] count]);
   
   round_window * p = m_roundwindow->m_pwindow;
    
    if ([touches count] == 1 && allTouchesEnded) {
        
        UITouch *touch = [touches anyObject];
        
       // --------------------------------------------------------
       //
       //   tap/ClickCount == 0 :
       //   not properly a tap/click, 
       //   (but-a/"->"){eco/green-lang): drag.
       //   So, still send mouse up message, now in the target location.
       //
       if ([touch tapCount] == 0)
       {
          
          CGPoint point = [touch locationInView:self];
          
          int x = point.x;
          
          int y = point.y;
          
          p->round_window_mouse_up(x, y);
          
       }
       else if ([touch tapCount] == 1) 
       {
            
            // if touch is a single tap, store its location so we can average it with the second touch location
            
            CGPoint point = [touch locationInView:self];
            
            //CGRect e = [[UIScreen mainScreen] applicationFrame];
            
            //int H = (int) e.size.height;
            
            int x = point.x;
            
            //int y = H - point.y;
           
           int y = point.y;
            
            p->round_window_mouse_up(x, y);
            
        } else {
            
//            twoFingerTapIsPossible = NO;
            
        }
        
    }
   
//   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   return;
   
}


- (void)touchesCanceled:(NSSet *)touches withEvent:(UIEvent *)event
{
   
   [super touchesEnded:touches withEvent:event];
   
   BOOL allTouchesEnded = ([touches count] == [[event touchesForView:self] count]);
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if ([touches count] == 1 && allTouchesEnded) {
      
      UITouch *touch = [touches anyObject];
      
      if ([touch tapCount] == 1) {
         
         // if touch is a single tap, store its location so we can average it with the second touch location
         
         CGPoint point = [touch locationInView:self];
         
         //CGRect e = [[UIScreen mainScreen] applicationFrame];
         
         //int H = (int) e.size.height;
         
         int x = point.x;
         
         //            int y = H - point.y;
         
         int y = point.y;
         
         p->round_window_mouse_up(x, y);
         
      } else {
         
         //            twoFingerTapIsPossible = NO;
         
      }
      
   }
   
   //   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   return;
   
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
   
   [super touchesMoved:touches withEvent:event];
   
   round_window * p = m_roundwindow->m_pwindow;
   
    if ([touches count] == 1) {
        
        UITouch *touch = [touches anyObject];
        
        if ([touch tapCount] == 1) {
            
            // if touch is a single tap, store its location so we can average it with the second touch location
            
            CGPoint point = [touch locationInView:self];
            
            //CGRect e = [[UIScreen mainScreen] applicationFrame];
            
            //int H = (int) e.size.height;
            
            int x = point.x;
            
           //            int y = H - point.y;
           
           int y = point.y;
           
            p->round_window_mouse_moved(x, y);
            
        } else {
            
            //            twoFingerTapIsPossible = NO;
            
        }
        
    }
   
   return;
   
}


/*- (void)mouseDragged:(NSEvent *)event
{
   
   boot_window * p = m_roundwindow->m_pwindow;
   
   CGPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   CGRect e = [[NSScreen mainScreen] frame];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->boot_window_mouse_dragged(x, y);
   
   return;
   
   
}
 */


//
// mouseDown:
//
// Handles mouse clicks in our frame. Two actions:
//	- click in the resize box should resize the window
//	- click anywhere else will drag the window.
//
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
   
    [super touchesBegan:touches withEvent:event];
    
    round_window * p = m_roundwindow->m_pwindow;
 
    BOOL allTouchesEnded = ([touches count] == [[event touchesForView:self] count]);

//    if ([touches count] == 1 && allTouchesEnded) {
    if ([touches count] == 1) {
        
        UITouch *touch = [touches anyObject];
        
        if ([touch tapCount] == 1) {
            
            // if touch is a single tap, store its location so we can average it with the second touch location
            
            CGPoint point = [touch locationInView:self];
            
            //CGRect e = [[UIScreen mainScreen] applicationFrame];
            
            //int H = (int) e.size.height;
            
            int x = point.x;
            
//            int y = H - point.y;
           
            int y = point.y;
           
            p->round_window_mouse_down(x, y);
           
           if(allTouchesEnded)
           {
              
              p->round_window_mouse_up(x, y);
              
           }
            
        } else {
            
            //            twoFingerTapIsPossible = NO;
            
        }
        
    }
   
   return;
   
/*	CGPoint pointInView = [self convertPoint:[event locationInWindow] fromView:nil];
	
	BOOL resize = NO;
	if (CGPointInRect(pointInView, [self resizeRect]))
	{
		resize = YES;
	}
	
	NSWindow *window = [self window];
	CGPoint originalMouseLocation = [window convertBaseToScreen:[event locationInWindow]];
	CGRect originalFrame = [window frame];
	
    while (YES)
	{
		//
		// Lock focus and take all the dragged and mouse up events until we
		// receive a mouse up.
		//
        NSEvent *newEvent = [window
			nextEventMatchingMask:(NSLeftMouseDraggedMask | NSLeftMouseUpMask)];
		
        if ([newEvent type] == NSLeftMouseUp)
		{
			break;
		}
		
		//
		// Work out how much the mouse has moved
		//
		CGPoint newMouseLocation = [window convertBaseToScreen:[newEvent locationInWindow]];
		CGPoint delta = NSMakePoint(
			newMouseLocation.x - originalMouseLocation.x,
			newMouseLocation.y - originalMouseLocation.y);
		
		CGRect newFrame = originalFrame;
		
		if (!resize)
		{
			//
			// Alter the frame for a drag
			//
			newFrame.origin.x += delta.x;
			newFrame.origin.y += delta.y;
		}
		else
		{
			//
			// Alter the frame for a resize
			//
			newFrame.size.width += delta.x;
			newFrame.size.height -= delta.y;
			newFrame.origin.y += delta.y;
			
			//
			// Constrain to the window's min and max size
			//
			CGRect newContentRect = [window contentRectForFrameRect:newFrame];
			NSSize maxSize = [window maxSize];
			NSSize minSize = [window minSize];
			if (newContentRect.size.width > maxSize.width)
			{
				newFrame.size.width -= newContentRect.size.width - maxSize.width;
			}
			else if (newContentRect.size.width < minSize.width)
			{
				newFrame.size.width += minSize.width - newContentRect.size.width;
			}
			if (newContentRect.size.height > maxSize.height)
			{
				newFrame.size.height -= newContentRect.size.height - maxSize.height;
				newFrame.origin.y += newContentRect.size.height - maxSize.height;
			}
			else if (newContentRect.size.height < minSize.height)
			{
				newFrame.size.height += minSize.height - newContentRect.size.height;
				newFrame.origin.y -= minSize.height - newContentRect.size.height;
			}
		}
		
		[window setFrame:newFrame display:YES animate:NO];
	}*/
}

//- (void)drawLayer:(CALayer *)layer
//        inContext:(CGContextRef)ctx;
//{
//   if(m_roundwindow == nil)
//      return;
//
//   round_window * p = m_roundwindow->m_pwindow;
//
//   CGContextRef context = ctx;
//
//   CGContextSaveGState(context);
//
//   CGContextTranslateCTM(context, p->round_window_get_x(), p->round_window_get_y());
//
//   p->round_window_draw(context);
//
//   CGContextRestoreGState(context);
//
//
//}

- (void)drawRect:(CGRect)rect
{
   
   [[UIColor whiteColor] set];
   
   UIRectFill(rect);
   
   if(m_roundwindow == nil)
      return;
  
   round_window * p = m_roundwindow->m_pwindow;
   
   CGContextRef context = UIGraphicsGetCurrentContext();
   
   CGContextSaveGState(context);
   
   int x = p->round_window_get_x();
   
   int y = p->round_window_get_y();
   
   CGContextTranslateCTM(context, x, y);
   
   p->round_window_draw(context);
   
   CGContextRestoreGState(context);
   
}



//- (BOOL) acceptsFirstMouse:(NSEvent *)theEvent//
//{
  // [self mouseDown: theEvent];
   //return YES;
//}





/*
- (void)keyDown:(UIEvent *)event {
   

   ::user::e_key ekey = event_key(event);
   
   boot_window * p = m_roundwindow->m_pwindow;
   
   if(p->boot_window_key_down(ekey, event.characters.UTF8String))
      return;
   
   [super keyDown:event];
   
}

- (void)keyUp:(NSEvent *)event {
   
   
   ::user::e_key ekey = event_key(event);
   
   boot_window * p = m_roundwindow->m_pwindow;
   
   if(p->boot_window_key_up(ekey, event.characters.UTF8String))
      return;
   
   [super keyUp:event];
   
}

- (void)flagsChanged:(NSEvent *)event
{

   boot_window * p = m_roundwindow->m_pwindow;
   
   if([event modifierFlags] & NSShiftKeyMask)
   {
      if(!m_bShift)
      {
         m_bShift = true;
         if(p->boot_window_key_down(::user::key_shift, ""))
            return;
      }
   }
   else
   {
      if(m_bShift)
      {
         m_bShift = false;
         if(p->boot_window_key_up(::user::key_shift, ""))
            return;
      }
   }

   if([event modifierFlags] & NSControlKeyMask)
   {
      if(!m_bControl)
      {
         m_bControl = true;
         if(p->boot_window_key_down(::user::key_control, ""))
            return;
      }
   }
   else
   {
      if(m_bControl)
      {
         m_bControl = false;
         if(p->boot_window_key_up(::user::key_control, ""))
            return;
      }
   }
 
   if([event modifierFlags] & NSAlternateKeyMask)
   {
      if(!m_bAlt)
      {
         m_bAlt = true;
         if(p->boot_window_key_down(::user::key_alt, ""))
            return;
      }
   }
   else
   {
      if(m_bAlt)
      {
         m_bAlt = false;
         if(p->boot_window_key_up(::user::key_alt, ""))
            return;
      }
   }

   [super flagsChanged:event];
}
*/


-(void) redraw_view
{
   
   [self setNeedsDisplay];
   
   __ios_do_events();
   
}

//- (BOOL)becomeFirstResponder
//{
//   
//   return FALSE;
//   
//}


- (BOOL)roundBecomeFirstResponder
{
   
   [[self dd_invokeOnMainThread] becomeFirstResponder ];
//   [super ];
   
   return TRUE;
   
}

@end


::user::e_key event_key(UIEvent * event)
{
   /*
   if([event modifierFlags] & NSNumericPadKeyMask) // arrow keys have this mask
   {
      
      NSString * arrow = [event charactersIgnoringModifiers];
      
      unichar key = 0;
      
      if([arrow length] == 0)
         return ::user::key_none;            // reject dead keys
      
      if([arrow length] == 1)
      {
         
         key = [arrow characterAtIndex:0];
         
         if(key == NSLeftArrowFunctionKey)
         {
            
            return ::user::key_left;
            
         }
         
         if(key == NSRightArrowFunctionKey)
         {
            
            return ::user::key_right;
            
         }
         
         if(key == NSUpArrowFunctionKey)
         {
            
            return ::user::key_up;
            
         }
         
         if(key == NSDownArrowFunctionKey)
         {
            
            return ::user::key_down;
            
         }
         
      }
      
   }
   else
   {
      
      unsigned short ush = [event keyCode];
      
      if(ush == 12)
      {
         return ::user::key_q;
      }
      else if(ush == 13)
      {
         return ::user::key_w;
      }
      else if(ush == 14)
      {
         return ::user::key_e;
      }
      else if(ush == 15)
      {
         return ::user::key_r;
      }
      else if(ush == 17)
      {
         return ::user::key_t;
      }
      else if(ush == 16)
      {
         return ::user::key_y;
      }
      else if(ush == 32)
      {
         return ::user::key_u;
      }
      else if(ush == 34)
      {
         return ::user::key_i;
      }
      else if(ush == 31)
      {
         return ::user::key_o;
      }
      else if(ush == 35)
      {
         return ::user::key_p;
      }
      else if(ush == 0)
      {
         return ::user::key_a;
      }
      else if(ush == 1)
      {
         return ::user::key_2;
      }
      else if(ush == 2)
      {
         return ::user::key_d;
      }
      else if(ush == 3)
      {
         return ::user::key_f;
      }
      else if(ush == 5)
      {
         return ::user::key_g;
      }
      else if(ush == 4)
      {
         return ::user::key_h;
      }
      else if(ush == 38)
      {
         return ::user::key_j;
      }
      else if(ush == 40)
      {
         return ::user::key_k;
      }
      else if(ush == 37)
      {
         return ::user::key_l;
      }
      else if(ush == 41)
      {
         return ::user::key_semicolon;
      }
      else if(ush == 6)
      {
         return ::user::key_z;
      }
      else if(ush == 7)
      {
         return ::user::key_x;
      }
      else if(ush == 8)
      {
         return ::user::key_c;
      }
      else if(ush == 9)
      {
         return ::user::key_v;
      }
      else if(ush == 11)
      {
         return ::user::key_b;
      }
      else if(ush == 45)
      {
         return ::user::key_n;
      }
      else if(ush == 46)
      {
         return ::user::key_m;
      }
      else if(ush == 43)
      {
         return ::user::key_comma;
      }
      else if(ush == 47)
      {
         return ::user::key_dot;
      }
      else if(ush == 44)
      {
         //         return ::user::key_;
      }
      else if(ush == 18)
      {
         return ::user::key_1;
      }
      else if(ush == 19)
      {
         return ::user::key_2;
      }
      else if(ush == 20)
      {
         return ::user::key_3;
      }
      else if(ush == 21)
      {
         return ::user::key_4;
      }
      else if(ush == 23)
      {
         return ::user::key_5;
      }
      else if(ush == 22)
      {
         return ::user::key_6;
      }
      else if(ush == 26)
      {
         return ::user::key_7;
      }
      else if(ush == 28)
      {
         return ::user::key_8;
      }
      else if(ush == 25)
      {
         return ::user::key_9;
      }
      else if(ush == 29)
      {
         return ::user::key_0;
      }
      else if(ush == 36)
      {
         return ::user::key_return;
      }
      else if(ush == 48)
      {
         return ::user::key_tab;
      }
      else if(ush == 51)
      {
         return ::user::key_back;
      }
      else if(ush == 117)
      {
         return ::user::key_delete;
      }
      
   }*/
   
   return ::user::key_none;;
   
   
}


void __ios_do_events()

{

   
   [[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode beforeDate: [NSDate date]];

}





