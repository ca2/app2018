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


@interface RoundWindowFrameView : NSView
{
   @public
   RoundWindow * m_roundwindow;
   bool m_bLShift;
   bool m_bRShift;
   bool m_bLControl;
   bool m_bRControl;
   bool m_bLAlt;
   bool m_bRAlt;
   bool m_bLCommand; // In Windows, the Windows Key
   bool m_bRCommand; // In Windows, the Windows Key
   
   
}

- (id)initWithFrame:(NSRect)frame andRoundWindow:(RoundWindow*)roundwindow;

@end
