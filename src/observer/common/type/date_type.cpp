/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */
#include <iomanip>
#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/type/char_type.h"
#include "event/sql_debug.h"
#include "common/value.h"
using std::setfill;
using std::setw;

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
  ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");
	LOG_INFO("datetype::compare:: left is %d while right is %d",left.get_int(),right.get_int());
    return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
}

RC DateType::add(const Value &left, const Value &right, Value &result) const
{
  return RC::INTERNAL;
}

RC DateType::subtract(const Value &left, const Value &right, Value &result) const
{
  return RC::INTERNAL;
}

RC DateType::multiply(const Value &left, const Value &right, Value &result) const
{
  return RC::INTERNAL;
}

RC DateType::negative(const Value &val, Value &result) const
{
  return RC::INTERNAL;
}

inline bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}
inline bool is_valid_date(int year, int month, int day) {
    if (year < 1900 || year > 9999 ||
        month <= 0 || month > 12 ||
        day <= 0 || day > 31) {
        return false;
    }
    int max_day_in_month[] = {
        31,
        29,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31,
    };
    const int max_day = max_day_in_month[month - 1];
    if (day > max_day) {
        return false;
    }
    if (month == 2 && !is_leap_year(year) && day > 28) {
        return false;
    }
    return true;
}
RC DateType::set_value_from_str(Value &val, const string &data) const
{
	int year = 0;
    int month = 0;
    int day = 0;
    int ret = sscanf(data.c_str(), "%d-%d-%d", &year, &month, &day);
    if (ret != 3) {
        return RC::INVALID_ARGUMENT;
    }
    if (!is_valid_date(year, month, day)) {
        return RC::INVALID_ARGUMENT;
    }
    int date = year * 10000 + month * 100 + day;
		val.set_date(date);
    return RC::SUCCESS;
}
RC DateType::to_string(const Value &val, string &result) const
{
	int date =  val.get_int();
	sql_debug("the date number is %d", date);
	int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;
    if (!is_valid_date(year, month, day)) {
        return RC::INVALID_ARGUMENT;
    }
		std::stringstream ss;
		ss<<setfill('0')<<setw(4)<<year<<"-"<<setw(2)<<month<<"-"<<setw(2)<<day;
		result = ss.str();
		return RC::SUCCESS;
}
RC DateType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
		case AttrType::CHARS: {
														DataType::type_instance(AttrType::CHARS)->set_value_from_str(result,val.get_string());
    LOG_INFO("date value %d turned into char value %s",val.get_int(),result.get_string().c_str());
													} break;
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int DateType::cast_cost(AttrType type)
{
  if (type==AttrType::DATES) {
    return 0;
  }
	if(type == AttrType::CHARS) {
		return 3; // 避免2003-1-09和2003-01-08比较的问题
	}
  return INT32_MAX;
}
