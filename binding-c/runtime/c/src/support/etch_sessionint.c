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
 * etch_sessionint.c
 * session interface
 */

#include "etch_sessionint.h"
#include "etch_defvalufact.h"
#include "etch_message.h"
#include "etch_global.h"
#include "etchlog.h"

int etchsession_def_session_control (void*, void*, void*);
int etchsession_def_session_notify  (void*, void*);
void* etchsession_def_session_query (void*, void*);

char* ETCHDSIF = "DSIF";


/**
 * new_default_sessson_interface
 * return a session interface populated with defaults for virtuals.
 * caller owns returned object, not an etch object, use etch_free() to destroy.
 */
i_session* new_default_session_interface(void* thisx)
{
    i_session* newi = etch_malloc(sizeof(i_session), ETCHTYPEB_RAWOBJECT);
    newi->session_control = etchsession_def_session_control;
    newi->session_notify  = etchsession_def_session_notify;
    newi->session_query   = etchsession_def_session_query;
    newi->thisx = thisx;
    return newi;    
}


/**
 * new_session_interface
 * return a session interface populated with specified virtuals.
 * caller owns returned object, not an etch object, use etch_free() to destroy.
 */
i_session* new_session_interface(void* thisx,
    etch_session_control sc, etch_session_notify sn, etch_session_query sq)
{
    i_session* newi = new_default_session_interface(thisx);
    if (sc) newi->session_control = sc;
    if (sn) newi->session_notify  = sn;
    if (sq) newi->session_query   = sq;
    return newi;    
}


/**
 * clone_session() 
 */
i_session* clone_session(void* thisx, const i_session* thatsession) 
{
    i_session* newsession = thatsession? new_default_session_interface(thisx): NULL;

    if (newsession)
        memcpy(newsession, thatsession, sizeof(i_session));
     
    return newsession;
}


/**
 * new_default_objsessson_interface
 * return an objsession interface populated with defaults for virtuals.
 * caller owns returned object, not an etch object, use etch_free() to destroy.
 */
i_objsession* new_default_objsession_interface (void* thisx)
{
    i_objsession* newi = etch_malloc(sizeof(i_objsession), ETCHTYPEB_RAWOBJECT);
    newi->_session_control = etchsession_def_session_control;
    newi->_session_notify  = etchsession_def_session_notify;
    newi->_session_query   = etchsession_def_session_query;
    newi->thisx = thisx;
    return newi;    
}


/**
 * new_objsession_interface
 * return an objsession interface populated with specified virtuals.
 * caller owns returned object, not an etch object, use etch_free() to destroy.
 */
i_objsession* new_objsession_interface (void* thisx,
    etch_session_control sc, etch_session_notify sn, etch_session_query sq)
{
    return (i_objsession*) new_session_interface(thisx, sc, sn, sq);
}


/**
 * etchsession_get_objinfo()
 * extract object info from the object passed to session methods.
 */
void etchsession_get_objinfo (etch_objsession_objinfo* p, void* evt)
{
    memset(p, 0, sizeof(etch_objsession_objinfo));
    if (NULL == evt) return;
    p->obj = (objmask*) evt;
    p->obj_type = ((objmask*) evt)->obj_type;
    p->class_id = ((objmask*) evt)->class_id;

    if (is_etch_unwantedmsg (p->obj))
    {   p->msg = (objmask*) ((etch_unwanted_message*) p->obj)->message;
        p->whofrom = ((etch_unwanted_message*) p->obj)->whofrom;  
    }
    else
    if (is_etch_message (p->obj))
        p->msg = p->obj;

    if (is_etch_message (p->msg))
    {   
        p->is_message = TRUE; 
        p->msg_aname  = message_aname ((etch_message*) p->msg);

        if (is_exception(p->msg))
        {   p->is_exception = TRUE;
            p->exception = get_exception_from ((objmask*) p->msg);
        }
        else
        {   p->resobj = message_get ((etch_message*) p->msg, builtins._mf_result);
            if (is_exception(p->resobj))
            {   p->is_exception = TRUE;
                p->exception = get_exception_from ((objmask*) p->resobj);
            }
        }
    }     
}


/**
 * etchsession_destroy_objparam()
 * identify, log and destroy the specified session parameter object.
 */
void etchsession_destroy_objparam (void* evt, char* caller) 
{
    char *msgmask = 0, *descrip = 0, *SESSION_ = "session_";
    etch_objsession_objinfo objinfo;
    if (NULL == evt) return;

    etchsession_get_objinfo (&objinfo, evt);

    if (objinfo.is_message)
    {  
        if (objinfo.is_exception)
        {   msgmask = "%s%s disposing '%s'\n";
            descrip = objinfo.exception? ((etchexception*) objinfo.exception)->ansitext: 0;
        }
        else
        {   msgmask = "%s%s disposing message '%s'\n";
            descrip = objinfo.msg_aname;
        }

        if (!descrip) descrip = "?";
        etchlog(ETCHDSIF, ETCHLOG_DEBUG, msgmask, SESSION_, caller, descrip);
    }
    else
    {   msgmask = "%s%s disposing type %x class %x\n";
        etchlog(ETCHDSIF, ETCHLOG_XDEBUG, msgmask, SESSION_, caller, 
                objinfo.obj_type, objinfo.class_id);
    }

    ETCHOBJ_DESTROY(objinfo.obj);
}


/**
 * etchsession_def_session_control()
 * @param obj caller this.
 * @param evt some etch object or null.
 * @param v some etch object or null.
 */
int etchsession_def_session_control (void* obj, void* evt, void* v)
{
    etchsession_destroy_objparam (evt, "control");
    ETCHOBJ_DESTROY(((objmask*)v));
    return 0;
}


/**
 * etchsession_def_session_notify()
 * @param obj caller this.
 * @param evt some etch object or null.
 */
int etchsession_def_session_notify  (void* obj, void* evt)
{
    etchsession_destroy_objparam (evt, "notify");
    return 0;
}


/**
 * etchsession_def_session_query()
 * @param obj caller this.
 * @param query some etch object or null.
 */
void* etchsession_def_session_query (void* obj, void* query) 
{
    etchsession_destroy_objparam (obj, "query");
    return NULL;
}
