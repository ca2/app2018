#include "stdafx.h"



namespace i2com
{


	namespace rtp
	{


      mixer::mixer(::ca::application * papp) :
         ca(papp)
      {
      }

	   void mixer::start_mixing(room * proom)
	   {
		   m_proom = proom;
		   AfxBeginThread(get_app(), &thread_proc_mix, (LPVOID) this);
	   }

	   UINT mixer::thread_proc_mix(LPVOID lpparam)
		{
			mixer * pmixer = (mixer *) lpparam;
			pmixer->mix();
			return 0;
		}

	   void mixer::mix()
		{
			primitive::memory memMix;
			memMix.allocate(1024);
			primitive::memory mem;
			mem.allocate(1024);
			memset(memMix, 0, memMix.get_size());
			UINT uiRead;
			UINT uiPosition = 0;
         m_proom->m_memfileRaw.m_bSynch = true;
			while(true)
			{
				for(int i = 0; i < m_proom->m_receivera.get_count(); i++)
				{
					if(uiPosition == m_uiPosition)
					{
						uiRead = m_proom->m_receivera[i].read(mem, mem.get_size());
						short * pshSrc = (short *) mem.get_data();
						short * pshDst = (short *) memMix.get_data();
						while(uiRead > 0)
						{
							*pshDst += *pshSrc;
							pshDst++;
							pshSrc++;
							uiRead -= sizeof(short);
						}
					}
				}
				m_proom->m_memfileRaw.write(memMix, memMix.get_size());
			}
		}



	} // 	namespace rtp


} // namespace i2com

