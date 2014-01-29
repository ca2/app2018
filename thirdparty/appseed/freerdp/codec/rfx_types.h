/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * RemoteFX Codec Library
 *
 * Copyright 2011 Vic Lee
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RFX_TYPES_H
#define __RFX_TYPES_H

#include "framework.h"

#include <winpr/crt.h>
#include <winpr/pool.h>
#include <winpr/wlog.h>
#include <winpr/collections.h>

#include <freerdp/utils/debug.h>
#include <freerdp/utils/profiler.h>

#ifdef WITH_DEBUG_RFX
#define DEBUG_RFX(fmt, ...) DEBUG_CLASS(RFX, fmt, ## __VA_ARGS__)
#else
#define DEBUG_RFX(fmt, ...) DEBUG_NULL(fmt, ## __VA_ARGS__)
#endif

struct _RFX_CONTEXT_PRIV
{
	wLog* log;
	wObjectPool* TilePool;

	BOOL UseThreads;
	DWORD MinThreadCount;
	DWORD MaxThreadCount;

	PTP_POOL ThreadPool;
	TP_CALLBACK_ENVIRON ThreadPoolEnv;
 
	wBufferPool* BufferPool;

	/* profilers */
	PROFILER_DEFINE(prof_rfx_decode_rgb);
	PROFILER_DEFINE(prof_rfx_decode_component);
	PROFILER_DEFINE(prof_rfx_rlgr_decode);
	PROFILER_DEFINE(prof_rfx_differential_decode);
	PROFILER_DEFINE(prof_rfx_quantization_decode);
	PROFILER_DEFINE(prof_rfx_dwt_2d_decode);
	PROFILER_DEFINE(prof_rfx_ycbcr_to_rgb);
	PROFILER_DEFINE(prof_rfx_decode_format_rgb);

	PROFILER_DEFINE(prof_rfx_encode_rgb);
	PROFILER_DEFINE(prof_rfx_encode_component);
	PROFILER_DEFINE(prof_rfx_rlgr_encode);
	PROFILER_DEFINE(prof_rfx_differential_encode);
	PROFILER_DEFINE(prof_rfx_quantization_encode);
	PROFILER_DEFINE(prof_rfx_dwt_2d_encode);
	PROFILER_DEFINE(prof_rfx_rgb_to_ycbcr);
	PROFILER_DEFINE(prof_rfx_encode_format_rgb);
};

#endif /* __RFX_TYPES_H */
