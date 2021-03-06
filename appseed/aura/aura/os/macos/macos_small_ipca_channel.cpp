#include "framework.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>




namespace aura
{


   namespace ipc
   {


      base::base(::aura::application * papp) :
         object(papp)
      {
         m_port = NULL;
      }

      base::~base()
      {
      }


      tx::tx(::aura::application * papp) :
         object(papp),
         base(papp)
      {
         
      }
      
      
      tx::~tx()
      {
         
      }


      bool tx::open(const char * pszChannel,launcher * plauncher)
      {

         CFDataRef data;
         
         SInt32 messageID = 0x1111; // Arbitrary
         
         CFTimeInterval timeout = 10.0;
         
         CFStringRef name = CFStringCreateWithCString(NULL,  (string("com.ca2.app.port.server.") + pszChannel), kCFStringEncodingUTF8);
      
         m_port =        CFMessagePortCreateRemote(nil,name);
         
         CFRelease(name);

         return true;
         
      }
      

      bool tx::close()
      {

         if(m_port == NULL)
         {
            
            return true;
            
         }

         CFRelease(m_port);

         m_port = NULL;

         return true;

      }


      bool tx::send(const char * pszMessage,::duration durationTimeout)
      {
         
         if(m_port == NULL)
            return false;

         ::count c = strlen_dup(pszMessage);

         ::count cSend;
         
         memory m;
         
         m.assign(pszMessage, c);
         
         CFDataRef data = m.get_os_cf_data();
         
         CFTimeInterval sendTimeout = (double) durationTimeout.get_total_milliseconds() / 1000.0;
         
         CFTimeInterval rcvimeout = (double) durationTimeout.get_total_milliseconds() / 1000.0;

         SInt32 status =
         CFMessagePortSendRequest(m_port,
                                  0x80000000,
                                  data,
                                  sendTimeout,
                                  rcvimeout,
                                  NULL,
                                  NULL);
         if (status == kCFMessagePortSuccess)
         {
            
            return true;
            
         }
         
         if(status != kCFMessagePortSendTimeout && status != kCFMessagePortReceiveTimeout)
         {
            
            close();
            
         }
         
         return false;

      }


      bool tx::send(int message,void * pdata,int len,duration durationTimeout)
      {

         if(message == 0x80000000)
            return false;


         if(!is_tx_ok())
            return false;

         memory m(pdata, len);

         ::count c = len;

         ::count cSend;


         SInt32 status =
         CFMessagePortSendRequest(m_port,
                                  message,
                                  m.get_os_cf_data(),
                                  (double) durationTimeout.get_total_milliseconds() / 1000.0,
                                  (double) durationTimeout.get_total_milliseconds() / 1000.0,
                                  NULL,
                                  NULL);
         if (status == kCFMessagePortSuccess) {
            // ...
         }
         return true;

      }



      bool tx::is_tx_ok()
      {

         return m_port != NULL;

      }




      rx::rx(::aura::application * papp) :
         object(papp),
         base(papp)
      {
         
         m_id = "::aura::ipc::rx";

         m_preceiver    = NULL;

      }


      rx::~rx()
      {

      }


      CFDataRef Callback(CFMessagePortRef port,
                                SInt32 messageID,
                                CFDataRef data,
                                void *info)
      {
         rx * p = (rx*) info;
         
         if(messageID == 0x80000000)
         {
            
            memory m;
            
            m.set_os_cf_data(data);
         
            p->on_receive(p,m.to_string());
         
         }
         else
         {
         
            memory m;
            
            m.set_os_cf_data(data);
            
            p->on_receive(p,messageID,m.get_data(),(int)m.get_size());
            
         }
         return NULL;
      }

      bool rx::create(const char * pszChannel)
      {
         
         CFMessagePortContext c = {};
         
         c.info = this;

         CFStringRef kungfuck = CFStringCreateWithCString(NULL,  (string("com.ca2.app.port.server.") + pszChannel), kCFStringEncodingUTF8);

         Boolean b = false;

         m_port = CFMessagePortCreateLocal(nil, kungfuck, Callback, &c, &b);
         
         start_receiving();

         return true;

      }


      bool rx::destroy()
      {
         
         if(m_port == NULL)
            return true;
         
         CFRelease(m_port);

         m_port = NULL;

         return true;

      }

      bool rx::start_receiving()
      {

         m_bRunning = true;

         m_bRun = true;

         m_pthread = fork([&]()
                            {
                               
                               receive();
                            });

         return true;

      }

      
      void rx::receiver::on_receive(rx * prx,const char * pszMessage)
      {
         
      }
      

      void rx::receiver::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
      {
      }

      void rx::receiver::on_post(rx * prx,int64_t a,int64_t b)
      {
      }



      void * rx::on_receive(rx * prx,const char * pszMessage)
      {
         
         if(m_preceiver != NULL)
         {

            string strMessage(pszMessage);
            
            ::fork(get_app(), [=] ()
            {
                   
               m_preceiver->on_receive(prx,strMessage);
               
            });
                   
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }

      void * rx::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
      {

         if(m_preceiver != NULL)
         {
            m_preceiver->on_receive(prx,message,pdata,len);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }


      void * rx::on_post(rx * prx,int64_t a,int64_t b)
      {

         if(m_preceiver != NULL)
         {
            m_preceiver->on_post(prx,a,b);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }



      bool rx::on_idle()
      {

         return false;

      }


      bool rx::is_rx_ok()
      {

         return m_port != NULL;
      }



      void * rx::receive()
      {
         
         if(m_port == NULL)
            return NULL;

         
         CFRunLoopSourceRef runLoopSource =
         CFMessagePortCreateRunLoopSource(nil, m_port, 0);
         
         CFRunLoopRef runloop = CFRunLoopGetCurrent();
         
         ::get_thread()->m_runloop = runloop;
         
         CFRunLoopAddSource(CFRunLoopGetCurrent(),
                            runLoopSource,
                            kCFRunLoopCommonModes);
         
         //while(m_bRun && ::get_thread_run())
         // Set up an autorelease pool here if not using garbage collection.
         bool done = false;
         
         // Add your sources or timers to the run loop and do any other setup.
         
         do
         {
            // Start the run loop but return after each source is handled.
            SInt32    result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.5, TRUE);
            
            // If a source explicitly stopped the run loop, or if there are no
            // sources or timers, go ahead and exit.
            if ((result == kCFRunLoopRunStopped) || (result == kCFRunLoopRunFinished))
               done = true;
            
            // Check for any other exit conditions here and set the
            // done variable as needed.
         }
         while(!done && ::get_thread_run());
         
//         while(m_bRun)
//         {
//
//            m_bRunning = true;
//
//            ssize_t  result;
//
//            int length;
//
//            data_struct data;
//
//            /* The length is essentially the size of the structure minus sizeof(mtype) */
//            length = sizeof(data) - sizeof(data.message);
//
//            memory mem;
//
//            do
//            {
//
//               ZERO(data);
//               if((result = msgrcv(m_iQueue,&data,length,2000,IPC_NOWAIT)) == -1)
//               {
//
//                  if(errno == ENOMSG)
//                  {
////                     if(!on_idle())
//  //                   {
//    //                    Sleep(84 * 1000);
//      //               }
//                     Sleep(100);
//                  }
//                  else
//                  {
//                     return (void *)-1;
//                  }
//
//               }
//
//               mem.append(data.data,data.size);
//
//
//               if(data.size < 512)
//                  break;
//
//            } while(true);
//
//            if(data.size > 0)
//            {
//            if(data.request == 5)
//            {
//
//               on_receive(this,mem.to_string());
//
//            }
//            else
//            {
//
//               on_receive(this,data.request,mem.get_data(),(int)mem.get_size());
//
//            }
//            }
//
//         }
//
//         m_bRunning = false;

         return NULL;

      }
      
      
      ipc::ipc(::aura::application * papp):
      object(papp),
      base(papp),
      tx(papp),
      m_rx(papp)
      {
         
         m_dwTimeout = (5000) * 11;
         
      }
      
      
      ipc::~ipc()
      {
         
         
      }



      bool ipc::open_ab(const char * pszChannel,launcher * plauncher)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel + "-a";
         string strChannelTx = m_strChannel + "-b";


         if(!m_rx.create(strChannelRx))
         {
            return false;
         }

         if(!tx::open(strChannelTx,plauncher))
         {
            return false;
         }

         return true;

      }

      bool ipc::open_ba(const char * pszChannel,launcher * plauncher)
      {

         m_strChannel = pszChannel;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel + "-b";
         string strChannelTx = m_strChannel + "-a";


         if(!m_rx.create(strChannelRx))
         {
            return false;
         }

         if(!tx::open(strChannelTx,plauncher))
         {
            return false;
         }

         return true;

      }


      bool ipc::is_rx_tx_ok()
      {

         return m_rx.is_rx_ok() && is_tx_ok();

      }


   } // namespace ipc


} // namespace aura




