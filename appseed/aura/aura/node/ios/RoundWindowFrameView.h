//
//  RoundWindowFrameView.h
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


#define NS_ROUND_WINDOW_FRAME_PADDING 0

@class RoundWindow;

@interface RoundWindowFrameView : UITextView
{
@public
   
   RoundWindow *     m_roundwindow;
   bool              m_bShift;
   bool              m_bControl;
   bool              m_bAlt;
   CGPoint           m_ptLastTouchBegan;
   
}


-(void)on_text : (NSString *) text;
-(void)on_sel : (UITextRange *) sel;


@end
