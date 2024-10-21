/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "common/type/text_type.h"
#include "common/type/date_type.h"
#include "common/value.h"

int TextType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::TEXTS && right.attr_type() == AttrType::TEXTS, "invalid type");
	LOG_INFO("chartype::compare:: left is %s, right is %s",left.get_string().c_str(),right.get_string().c_str());
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC TextType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC TextType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    default: return RC::UNIMPLEMENTED;
  }

  return RC::SUCCESS;
}

int TextType::cast_cost(AttrType type)
{
  return INT32_MAX;
}

RC TextType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}

