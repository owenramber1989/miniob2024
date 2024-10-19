/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "common/log/log.h"
#include "common/rc.h"
#include "sql/stmt/update_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/index/index_meta.h"
#include "storage/table/table.h"
#include "storage/table/table_meta.h"
#include <cstdint>
#include <string>

UpdateStmt::UpdateStmt(Table *table, const Value *values, FilterStmt *filter_stmt, const char *field_name)
    : table_(table), values_(values), filter_stmt_(filter_stmt), field_name_(field_name)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
	const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name || update.value.length() == 0) {
    LOG_WARN("invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }
  // 检查表是否存在
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  // 检查字段类型
  const TableMeta &table_meta = table->table_meta();
  const FieldMeta *field_meta = table_meta.field(update.attribute_name.c_str());
  if (field_meta == nullptr) {
    return RC::INVALID_ARGUMENT;
  }
  if (field_meta->type() != update.value.attr_type()) {
    LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_meta->type(),update.value.attr_type());
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert({std::string(table_name), table});
  FilterStmt *filter_stmt = nullptr;
  // 创建Filter
  RC rc = FilterStmt::create(
      db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  stmt = new UpdateStmt(table, &update.value, filter_stmt, update.attribute_name.c_str());
  return RC::SUCCESS;
}
