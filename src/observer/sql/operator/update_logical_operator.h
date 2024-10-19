#pragma once
#include "sql/operator/logical_operator.h"
#include "common/value.h"
#include "storage/table/table.h"
#include <string>
/**
 * @brief 逻辑算子，用于执行update语句
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, Value value, const char *field_name);
  virtual ~UpdateLogicalOperator() = default;
  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }
  Table              *table() const { return table_; }
  const Value        &value() const { return value_; }
  Value              &value() { return value_; }
  const char         *field() const { return field_name_.c_str(); }
private:
  Table      *table_ = nullptr;
  Value       value_;
  std::string field_name_;
};
