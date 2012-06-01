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

#include "serialization/EtchType.h"
#include "util/EtchHash.h"

const EtchObjectType EtchType::TYPE(EOTID_TYPE, NULL);

EtchType::EtchType()
: EtchObject(&EtchType::TYPE), mId(0), mTimeout(0), mName(""), mSuperType(NULL),
mResultType(NULL), mDirection(BOTH), mAsyncMode(NONE), mLocked(false), mComponentType(NULL), mHelper(NULL) {
}

EtchType::EtchType(capu::uint32_t id, EtchString &name)
: EtchObject(&EtchType::TYPE), mId(id), mTimeout(0), mName(name), mSuperType(NULL),
mResultType(NULL), mDirection(BOTH), mAsyncMode(NONE), mLocked(false), mComponentType(NULL), mHelper(NULL) {
}

EtchType::EtchType(EtchString &name)
: EtchObject(&EtchType::TYPE), mTimeout(0), mName(name), mSuperType(NULL),
mResultType(NULL), mDirection(BOTH), mAsyncMode(NONE), mLocked(false), mComponentType(NULL), mHelper(NULL) {
  mId = EtchHashEx::Digest(mName);;
}

EtchType::~EtchType() {
  if (mHelper != NULL) {
    //TODO: Check memory management
    delete mHelper;
  }
}

capu::uint64_t EtchType::getHashCode() {
  return mId.getHashCode() ^ mName.getHashCode();
}

status_t EtchType::addField(EtchField &field) {
  return mFieldMap.add(field);
}

status_t EtchType::getField(capu::uint32_t id, EtchField *field) {
  return mFieldMap.get(id, field);
}

status_t EtchType::getField(EtchString &name, EtchField *field) {
  return mFieldMap.get(name, field);
}

void EtchType::setAsyncMode(EtchAsyncMode mode) {
  if (!mLocked)
    mAsyncMode = mode;
}

EtchAsyncMode EtchType::getAsyncMode() {
  return mAsyncMode;
}

void EtchType::setDirection(EtchDirection direction) {
  if (!mLocked)
    mDirection = direction;
}

EtchDirection EtchType::getDirection() {
  return mDirection;
}

void EtchType::setResult(EtchType* rType) {
  if (!mLocked)
    mResultType = rType;
}

EtchType* EtchType::getResult() {
  return mResultType;
}

void EtchType::setTimeout(capu::uint32_t timeout) {
  if (!mLocked)
    mTimeout = timeout;
}

capu::uint32_t EtchType::getTimeout() {
  return mTimeout;
}

void EtchType::setSuperType(EtchType* superType) {
  if (!mLocked)
    mSuperType = superType;
}

EtchType* EtchType::getSuperType() {
  return mSuperType;
}

void EtchType::setResponseField(EtchField responseField) {
  if (!mLocked)
    mResponseField = responseField;
}

EtchField EtchType::getResponseField() {
  return mResponseField;
}

status_t EtchType::putValidator(EtchField key, capu::SmartPointer<EtchValidator> validator) {
  if (mLocked)
    return ETCH_EINVAL;
  if (validator.get() == NULL)
    return ETCH_EINVAL;
  EtchField field;
  if (mFieldMap.get(key.getId(), &field) == ETCH_ENOT_EXIST) {
    addField(key);
  }
  capu::SmartPointer<EtchValidator> val;
  capu::SmartPointer<EtchValidator> old;
  if (mValidatorMap.get(key, &val) == ETCH_OK) {
    capu::SmartPointer<EtchValidator> comboValidator = new EtchComboValidator(val, validator);
    return mValidatorMap.put(key, comboValidator, &old);
  } else {
    return mValidatorMap.put(key, validator);
  }
}

status_t EtchType::getValidator(EtchField& key, capu::SmartPointer<EtchValidator> &validator) {
  return mValidatorMap.get(key, &validator);
}

status_t EtchType::clearValidator(EtchField key) {
  capu::SmartPointer<EtchValidator> val;
  return mValidatorMap.remove(key, &val);
}

void EtchType::setImportExportHelper(EtchImportExportHelper* helper) {
  if (!mLocked)
    mHelper = helper;
}

EtchImportExportHelper* EtchType::getImportExportHelper() {
  return mHelper;
}

void EtchType::lock() {
  mLocked = true;
  mFieldMap.lock();
}

capu::bool_t EtchType::equals(EtchObject *other) {
  if (other == NULL)
    return false;
  if (other->getObjectType() != &EtchType::TYPE)
    return false;
  EtchType *type = (EtchType*) other;
  return ((type->mId.equals(&(mId))) && (type->mName.equals(&mName)));
}

capu::uint32_t EtchType::getId() {
  return mId.get();
}

EtchString EtchType::getName() {
  return mName;
}

void EtchType::setComponentType(const EtchObjectType* type) {
  mComponentType = type;
}

const EtchObjectType* EtchType::getComponentType() {
  return mComponentType;
}