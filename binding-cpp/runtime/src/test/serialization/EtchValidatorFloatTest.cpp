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

#include <gtest/gtest.h>
#include "serialization/EtchValidatorFloat.h"
#include "capu/util/SmartPointer.h"
#include "common/EtchString.h"
#include "capu/os/NumericLimits.h"

TEST(EtchValidatorFloatTest, createTest) {
  capu::SmartPointer<EtchValidatorFloat> ptr = NULL;

  capu::SmartPointer<EtchValidator> val;
  EXPECT_TRUE(EtchValidatorFloat::Get(0, val) == ETCH_OK);
  ptr = capu::smartpointer_cast<EtchValidatorFloat>(val);

  EXPECT_TRUE(ptr->getExpectedType()->equals(&EtchFloat::TYPE));
  EXPECT_TRUE(ptr->getNDims() == 0);

  EtchObjectType type1(EOTID_FLOAT, NULL);
  EtchObjectType type2(EOTID_NATIVE_ARRAY, &type1);
  EtchObjectType type3(EOTID_NATIVE_ARRAY, &type2);

  EXPECT_TRUE(EtchValidatorFloat::Get(2, val) == ETCH_OK);
  ptr = capu::smartpointer_cast<EtchValidatorFloat>(val);
  EXPECT_TRUE(ptr->getExpectedType()->equals(&type3));
  EXPECT_TRUE(ptr->getNDims() == 2);
}

TEST(EtchValidatorFloatTest, validateTest) {
  EtchObject* byte = NULL;

  EtchObject* floatTmp = new EtchFloat(capu::NumericLimitMin<capu::float_t>());
  EtchObject* floatTmp1 = new EtchFloat(0);
  EtchObject* floatTmp2 = new EtchFloat(capu::NumericLimitMax<capu::float_t>());
  EtchObject* floatTmp3 = new EtchFloat(897.12);

  //incompatible type
  EtchObject* str = new EtchString();

  capu::SmartPointer<EtchValidator> ptr = NULL;
  EXPECT_TRUE(EtchValidatorFloat::Get(0, ptr) == ETCH_OK);
  EXPECT_TRUE((capu::smartpointer_cast<EtchTypeValidator>(ptr))->getNDims() == 0);

  EXPECT_FALSE(ptr->validate(byte));
  EXPECT_FALSE(ptr->validate(str));

  EXPECT_TRUE(ptr->validate(floatTmp));
  EXPECT_TRUE(ptr->validate(floatTmp1));
  EXPECT_TRUE(ptr->validate(floatTmp2));
  EXPECT_TRUE(ptr->validate(floatTmp3));

  delete floatTmp;
  delete floatTmp1;
  delete floatTmp2;
  delete floatTmp3;
  delete str;
}

TEST(EtchValidatorFloatTest, validateValueTest) {
  EtchObject* byte = NULL;
  EtchObject* result;
  EtchObject* floatTmp = new EtchFloat(-128.12);
  EtchObject* floatTmp2 = new EtchFloat(5.13);
  EtchObject* floatTmp3 = new EtchFloat(127.54);
  capu::SmartPointer<EtchValidator> ptr = NULL;
  EXPECT_TRUE(EtchValidatorFloat::Get(0, ptr) == ETCH_OK);
  EXPECT_TRUE(ptr->validateValue(byte, result) == ETCH_ERROR);
  EXPECT_TRUE(ptr->validateValue(floatTmp, result) == ETCH_OK);
  EXPECT_TRUE(((EtchFloat*) result)->get() == ((EtchFloat*) floatTmp)->get());
  EXPECT_TRUE(ptr->validateValue(floatTmp2, result) == ETCH_OK);
  EXPECT_TRUE(((EtchFloat*) result)->get() == ((EtchFloat*) floatTmp2)->get());
  EXPECT_TRUE(ptr->validateValue(floatTmp3, result) == ETCH_OK);
  EXPECT_TRUE(((EtchFloat*) result)->get() == ((EtchFloat*) floatTmp3)->get());
  delete floatTmp;
  delete floatTmp2;
  delete floatTmp3;
}

TEST(EtchValidatorFloatTest, elementValidatorTest) {
  capu::SmartPointer<EtchValidator> ptr = NULL;
  EXPECT_TRUE(EtchValidatorFloat::Get(1, ptr) == ETCH_OK);
  capu::SmartPointer<EtchValidator> elementValidator;
  ptr->getElementValidator(elementValidator);

  EtchObject* floatTmp = new EtchFloat(capu::NumericLimitMin<capu::float_t>());
  EtchObject* floatTmp1 = new EtchFloat(0);
  EtchObject* floatTmp2 = new EtchFloat(capu::NumericLimitMax<capu::float_t>());
  EtchObject* floatTmp3 = new EtchFloat(897.12);

  //incompatible type
  EtchObject* str = new EtchString();

  EXPECT_FALSE(elementValidator->validate(str));
  EXPECT_TRUE(elementValidator->validate(floatTmp));
  EXPECT_TRUE(elementValidator->validate(floatTmp1));
  EXPECT_TRUE(elementValidator->validate(floatTmp2));
  EXPECT_TRUE(elementValidator->validate(floatTmp3));

  delete floatTmp;
  delete floatTmp1;
  delete floatTmp2;
  delete floatTmp3;

  delete str;
}

