//
//  aura_timer.m
//  aura
//
//  Created by Thomas Borregaard Sørensen on 07/07/15.
//
//

#import "framework.h"


void * CreateDispatchQueue()
{
   
   return dispatch_queue_create (NULL, NULL);
   
}

void CreateDispatchSource(void * p)
{
   
   dispatch_source_cancel((dispatch_source_t) p);
   
}


void * CreateDispatchTimer(uint64_t interval, uint64_t leeway, void * queue, void (*pfnTimer)(void * p), void * p)
{
    
    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, (dispatch_queue_t) queue);
    
    if (timer)
    {
        
        dispatch_source_set_timer(timer, dispatch_walltime(NULL, 0), interval * 1000 * 1000, leeway * 1000 * 1000);
        
        dispatch_source_set_event_handler(timer, ^{ pfnTimer(p); });
        
        dispatch_resume(timer);
        
    }
    
    return timer;
    
}


void ReleaseDispatch(void * p)
{
 
   dispatch_release((dispatch_object_t) p);
   
}



