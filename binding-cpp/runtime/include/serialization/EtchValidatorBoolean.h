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

#ifndef __ETCHVALIDATORBOOLEAN_H__
#define __ETCHVALIDATORBOOLEAN_H__

#include "serialization/EtchTypeValidator.h"
#include "common/EtchBool.h"
#include "common/EtchNativeArray.h"

class EtchRuntime;

class EtchValidatorBoolean : public EtchTypeValidator {
public:

  /**
   * EtchObjectType for EtchValidatorBoolean.
   */
  static const EtchObjectType* TYPE();

  /**
   * Copy Constructor
   */
  EtchValidatorBoolean(const EtchValidatorBoolean& other);

  /**
   * Destructor
   */
  virtual ~EtchValidatorBoolean();

   /**
   * @see EtchValidator
   */
  virtual status_t getElementValidator(capu::SmartPointer<EtchValidator> &val);

   /**
   * @see EtchValidator
   */
  virtual capu::bool_t validate(capu::SmartPointer<EtchObject> value);

  /**
   * @see EtchValidator
   */
  virtual status_t validateValue(capu::SmartPointer<EtchObject> value, capu::SmartPointer<EtchObject>& result);

  VALIDATOR_GET_DEF()

protected:
  EtchValidatorBoolean(EtchRuntime* runtime, capu::uint32_t ndim);

private:
  EtchRuntime* mRuntime;
  VALIDATOR_CACHE_DEF()
};


#endif /* ETCHVALIDATORBOOLEAN_H */

