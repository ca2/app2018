//
//  RoundViewController.h
//  aura
//
//  Created by Thomas Borregaard Sørensen on 14/05/14.
//
//

@class RoundWindowFrameView;

@interface RoundViewController : UIViewController < UITextViewDelegate >
{
@public
   
   
   RoundWindowFrameView   * childContentView;
   RoundWindow * m_pwindow;
   
}

@end
