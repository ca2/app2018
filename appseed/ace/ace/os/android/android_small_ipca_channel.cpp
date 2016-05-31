

namespace ace
{


   namespace ipc
   {


      base::base(::ace::application * papp) :
         object(papp)
      {
         m_key = 0;
         m_iQueue = -1;
      }

      base::~base()
      {
      }

      tx::tx(::ace::application * papp) :
         object(papp),
         base(papp)
      {


      }

      tx::~tx()
      {


      }

      bool tx::open(const char * pszChannel,launcher * plauncher)
      {

         if(m_iQueue >= 0)
            close();

         /*   m_key = ftok(".", 'c');

            if(m_key == 0)
            return false;

            if((m_iQueue = msgget(m_key, 0660)) == -1)
            {
            return false;
            }

            m_strBaseChannel = pszChannel;*/

         return true;

      }

      bool tx::close()
      {

         if(m_iQueue < 0)
            return true;

         m_iQueue = -1;

         m_strBaseChannel = "";

         return true;

      }


      bool tx::send(const char * pszMessage,DWORD dwTimeout)
      {

         data_struct data;
         data.mtype        = 15111984;
         data.request      = 0;
         data.size         = strlen_dup(pszMessage);
         if(data.size > 512)
            return false;

         /* The length is essentially the size of the structure minus sizeof(mtype) */
         int32_t length = sizeof(data_struct) - sizeof(long);

         int32_t result;
         /*
            if((result = msgsnd(m_iQueue, &data, length, 0)) == -1)
            {
            return false;
            }
            */
         return true;
      }


      bool tx::send(int32_t message,void * pdata,int32_t len,DWORD dwTimeout)
      {

         if(message == (int32_t) 0x80000000)
            return false;


         if(!is_tx_ok())
            return false;

         const char * pszMessage = (const char *)pdata;

         ::count c = len;

         ::count cSend;

         data_struct data;
         data.mtype        = 15111984;
         data.request      = 0x80000000;
         data.size         = (int32_t)strlen_dup(pszMessage);

         ::count cPos = 0;

         while(c > 0)
         {

            cSend = MIN(c,511);

            memcpy(data.data,&pszMessage[cPos],MIN(c,511));

            c -= cSend;

            cPos += cSend;

            if(c > 0)
               data.size = 512;
            else
               data.size = (int32_t)cSend;

            /* The length is essentially the size of the structure minus sizeof(mtype) */
            int32_t length = sizeof(data_struct) - sizeof(long);

            int32_t result;
            /*
            if((result = msgsnd(m_iQueue, &data, length, 0)) == -1)
            {
            return false;
            }
            */
         }

         return true;

      }



      bool tx::is_tx_ok()
      {

         return m_iQueue != -1;

      }


      rx::rx(::ace::application * papp) :
         object(papp),
         base(papp)
      {

         m_preceiver    = NULL;

      }


      rx::~rx()
      {

      }


      bool rx::create(const char * pszChannel)
      {
         /*
         m_key = ftok(".", 'c');

         if(m_key == 0)
         return false;

         if((m_iQueue = msgget(m_key, IPC_CREAT | IPC_EXCL | 0660 )) == -1)
         {
         return false;
         }
         */

         return true;
      }


      bool rx::destroy()
      {

         int32_t iRetry = 23;
         while(m_bRunning && iRetry > 0)
         {
            m_bRun = false;
            sleep(1);
            iRetry--;
         }

         if(m_iQueue < 0)
            return true;
         /*
         if(msgctl( m_iQueue, IPC_RMID, 0) == -1)
         {
         return false;
         }
         */
         m_iQueue = -1;

         return true;

      }

      bool rx::start_receiving()
      {

         m_bRunning = true;

         m_bRun = true;

         if(pthread_create(&m_thread,NULL,&rx::receive_proc,this) != 0)
         {

            m_bRunning = false;

            m_bRun = false;

            return false;

         }

         return true;

      }

      void * rx::receive_proc(void * param)
      {

         rx * pchannel = (rx *)param;

         return pchannel->receive();

      }



      void rx::receiver::on_receive(rx * prx,const char * pszMessage)
      {
      }

      void rx::receiver::on_receive(rx * prx,int32_t message,void * pdata,memory_size_t len)
      {
      }

      void rx::receiver::on_post(rx * prx,int64_t a,int64_t b)
      {
      }



      void * rx::on_receive(rx * prx,const char * pszMessage)
      {

         if(m_preceiver != NULL)
         {
            m_preceiver->on_receive(prx,pszMessage);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }

      void * rx::on_receive(rx * prx,int32_t message,void * pdata,memory_size_t len)
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

         return m_iQueue != -1;
      }






      void * rx::receive()
      {

         while(m_bRun)
         {

            m_bRunning = true;

            ssize_t  result;

            //int32_t length;

            data_struct data;

            /* The length is essentially the size of the structure minus sizeof(mtype) */
            //length = sizeof(data_struct) - sizeof(long);

            memory mem;

            do
            {
               /*
               if((result = msgrcv(m_iQueue, &data, length, 15111984, IPC_NOWAIT)) == -1)
               {

               if(errno == ENOMSG)
               {
               if(!on_idle())
               {
               Sleep(100);
               }
               }
               else
               {
               return (void *) -1;
               }

               }
               */
               mem.assign(data.data,data.size);


               if(data.size < 512)
                  break;

            } while(true);


            if(data.request == 0)
            {

               on_receive(this,mem.to_string());

            }
            else
            {

               on_receive(this,data.request,mem.get_data(),mem.get_size());

            }

         }

         m_bRunning = false;

         return NULL;

      }

      ipc::ipc(::ace::application * papp):
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


} // namespace ace








