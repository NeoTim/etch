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
 * etch_transportpkt.c
 * i_transportpacket interface
 */

#include "etch_transportpkt.h"
#include "etch_message.h"
#include "etch_global.h"
#include "etchlog.h"
#define ETCHPACKET_DEFHEADERSIZE 8

int destroy_transportpacket(i_transportpacket*);
int etchtransportpkt_def_headersize (i_transportpacket*);
int etchtransportpkt_def_transportpacket (i_transportpacket*, etch_who*, etch_flexbuffer*);


/**
 * new_transportpkt_interface()
 * i_transportpacket constructor
 * @param tp transport_packet virtual function overrides, 
 * @param itransport transport interface virtual function overrides, 
 * caller relinquishes ownership of this memory
 */
i_transportpacket* new_transportpkt_interface(void* thisx, etch_transport_packet tp, i_transport* itransport)  
{
    i_transportpacket* newi = (i_transportpacket*) new_object
        (sizeof(i_transportpacket), ETCHTYPEB_TRANSPORTPKT, CLASSID_TRANSPORTPKT);

    newi->clone   = clone_null;
    newi->destroy = destroy_transportpacket;  
    newi->thisx   = thisx;
    newi->header_size = ETCHPACKET_DEFHEADERSIZE;

    newi->transport_packet = tp? tp: etchtransportpkt_def_transportpacket;

    newi->itransport = itransport? itransport: new_default_transport_interface(thisx);

    newi->transport_control = newi->itransport->transport_control; 
    newi->transport_notify  = newi->itransport->transport_notify;
    newi->transport_query   = newi->itransport->transport_query;
    newi->get_session       = newi->itransport->get_session;
    newi->set_session       = newi->itransport->set_session;

    newi->get_headersize    = etchtransportpkt_def_headersize;  

    return newi;
}


/*
 * destroy_transportpacket()
 * i_transportpacket destructor
 */
int destroy_transportpacket(i_transportpacket* itp)
{
    ETCH_ASSERT(is_etch_transportpkt(itp));
    if (itp->refcount > 0 && --itp->refcount > 0) return -1;  

    if (!is_etchobj_static_content(itp))
    {   etch_free(itp->itransport);
    }
            
    return destroy_objectex((objmask*) itp);
}


/*
 * etchtransportpkt_def_transportpacket()
 * default implementation of transport_packet() 
 * @param sender caller retains
 * @param buf caller retains
 */
int etchtransportpkt_def_transportpacket (i_transportpacket* tp, etch_who* sender, etch_flexbuffer* buf)
{
      return -1;
}


/*
 * etchtransportpkt_def_headersize()
 * default implementation of get_header_size() 
 * @return size of packet header in bytes
 */
int etchtransportpkt_def_headersize (i_transportpacket* tp)
{
    return tp->header_size;
}