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

/*
 * etch_transportdata.c
 * i_transportdata interface
 */

#include "etch_transportdata.h"
#include "etch_message.h"
#include "etch_global.h"
#include "etchlog.h"

int destroy_transportdata(i_transportdata*);
int etchtransportdata_def_transportdata (void* thisx, etch_who*, etch_flexbuffer*);


/**
 * new_transportdata_interface()
 * i_transportdata constructor
 * @param func i_transportdata::transport_data function override. 
 * @param itransport transport interface virtual function overrides, 
 * caller relinquishes ownership of this memory
 */
i_transportdata* new_transportdata_interface(void* thisx, etch_transport_data func, i_transport* itransport)  
{
    i_transportdata* newi = (i_transportdata*) new_object
        (sizeof(i_transportdata), ETCHTYPEB_TRANSPORTDATA, CLASSID_TRANSPORTDATA);

    newi->clone   = clone_null;
    newi->destroy = destroy_transportdata;  

    newi->transport_data = func? func: etchtransportdata_def_transportdata;

    newi->itransport = itransport? itransport: new_default_transport_interface();
    newi->transport_control = newi->itransport->transport_control; 
    newi->transport_notify  = newi->itransport->transport_notify;
    newi->transport_query   = newi->itransport->transport_query;
    newi->get_session       = newi->itransport->get_session;
    newi->set_session       = newi->itransport->set_session;

    newi->thisx = thisx;

    return newi;
}


/*
 * destroy_transportdata()
 * i_transportdata destructor
 */
int destroy_transportdata(i_transportdata* sm)
{
    if (NULL == sm) return -1;
    if (sm->refcount > 0 && --sm->refcount > 0) return -1;  

    if (!is_etchobj_static_content(sm))
    {   etch_free(sm->itransport);
    }
            
    return destroy_objectex((objmask*)sm);
}


/*
 * etchtransportdata_def_transportdata()
 * default virtual implementation  
 * @param whoto caller retains
 * @param buf caller retains 
 */
int etchtransportdata_def_transportdata (i_transportdata* sm, etch_who* whoto, etch_flexbuffer* buf)
{
      return -1;
}
