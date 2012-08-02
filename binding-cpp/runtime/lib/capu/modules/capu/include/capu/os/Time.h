/* $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIME_H__
#define __TIME_H__

#include "capu/Config.h"

#define TIME_INC_HEADER
#include "arch/Time.inc"
#undef TIME_INC_HEADER

namespace capu
{
    class Time
    {
     #define TIME_INC_MEMBER
         #include "arch/Time.inc"
     #undef TIME_INC_MEMBER
     public:

         inline static uint32_t GetMilliseconds();

    };

#define TIME_INC_IMPL
    #include "arch/Time.inc"
#undef TIME_INC_IMPL

}
#endif

