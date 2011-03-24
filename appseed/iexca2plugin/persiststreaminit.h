/*****************************************************************************
 * persiststreaminit.h: Free ActiveX based on ActiveX control for VLC
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 * Copyright (C) 2008 http://unick-soft.xost.ru
 *
 * Authors: Damien Fouilleul <Damien.Fouilleul@laposte.net>
 * Modification: Oleg <soft_support@list.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#pragma once


namespace iexca2
{

   class persist_stream_init : public IPersistStreamInit
   {

   public:

       persist_stream_init(plugin *p_instance);
       virtual ~persist_stream_init();

       // IUnknown methods
       STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
       {
           if( NULL == ppv )
               return E_POINTER;
           if( (IID_IUnknown == riid)
            || (IID_IPersist == riid)
            || (IID_IPersistStreamInit == riid) )
           {
               AddRef();
               *ppv = reinterpret_cast<LPVOID>(this);
               return NOERROR;
           }
           return _p_instance->pUnkOuter->QueryInterface(riid, ppv);
       };

       STDMETHODIMP_(ULONG) AddRef(void) { return _p_instance->pUnkOuter->AddRef(); };
       STDMETHODIMP_(ULONG) Release(void) { return _p_instance->pUnkOuter->Release(); };

       // IPersist methods
       STDMETHODIMP GetClassID(LPCLSID);

       // IPersistStreamInit methods
       STDMETHODIMP IsDirty(void);
       STDMETHODIMP InitNew(void);
       STDMETHODIMP Load(LPSTREAM);
       STDMETHODIMP Save(LPSTREAM, BOOL);
       STDMETHODIMP GetSizeMax(ULARGE_INTEGER *);

   private:

       plugin *_p_instance;
       class VLCPropertyBag *_p_props;
   };

} // namespace iexca2