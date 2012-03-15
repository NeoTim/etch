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

#ifndef __ETCHCONNECTION_H__
#define __ETCHCONNECTION_H__
#include "transport/EtchTransport.h"
#include "common/EtchString.h"
#include "common/EtchObject.h"
#include "capu/os/Thread.h"
#include "capu/util/SmartPointer.h"
#include "common/EtchError.h"
#include "common/EtchInt32.h"
#include "capu/os/Mutex.h"

template <class S>
class EtchConnection : public virtual EtchTransport<S> {
public:

  /** Source query to get the local address. */
  static const EtchString LOCAL_ADDRESS;

  /** Source query to get the remote address. */
  static const EtchString REMOTE_ADDRESS;

  /**
   * Host name to specify to select listening on all interfaces.
   * The value is "0.0.0.0".
   */
  static const EtchString ALL_INTFS;

  /**
   * Check that the receiving thread is working or not
   * @return true if it is started
   *         false otherwise
   */
  virtual capu::bool_t isStarted() = 0;

  /**
   * Main function that will be executed by thread
   * @param param parameter that will be passed to the thread
   */
  virtual void operator()(void * param) = 0;

  /**
   * Terminates operations on the socket.
   */
  virtual status_t close() = 0;

  /**
   * Sets up a newly opened socket. This may involve setting socket
   * options and opening input and output streams.
   */
  virtual status_t setupSocket() = 0;
  
protected:

  /**
   * Opens the socket of a connection.
   * @param reconnect true if we are trying to reconnect, false if this
   * is the first time.
   * @param true if we should reconnect, false if we should stop.
   * @return ETCH_OK if socket is successfully opened
   *         ETCH_ERROR otherwise
   */
  virtual status_t openSocket(capu::bool_t reconnect) = 0;


  /**
   * Performs the usual and customary operations on a socket, such
   * as read or accept.
   */
  virtual status_t readSocket() = 0;

  capu::Thread *mThread;

  capu::bool_t mIsStarted;

  static capu::Mutex mMutex;

  static capu::Mutex mMutexConnection;
};

template <class S>
const EtchString EtchConnection<S>::LOCAL_ADDRESS("LOCAL_ADDRESS");

template <class S>
const EtchString EtchConnection<S>::REMOTE_ADDRESS("REMOTE_ADDRESS");

template <class S>
const EtchString EtchConnection<S>::ALL_INTFS("0.0.0.0");

template <class S>
capu::Mutex EtchConnection<S>::mMutex;

template <class S>
capu::Mutex EtchConnection<S>::mMutexConnection;


#endif /* ETCHCONNECTION_H */
