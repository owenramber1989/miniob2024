#pragma once
#include <string>
#include <vector>
#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"
#include "common/value.h"
class UpdateStmt;
/**
 * @brief 更新物理算子
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, Value &&value, const char *field_name)
      : table_(table), value_(value), field_name_(field_name)
  {}
  virtual ~UpdatePhysicalOperator() = default;
  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }
  // 真正执行的操作
  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override { return nullptr; }
private:
  Table      *table_ = nullptr;
  Trx        *trx_   = nullptr;
  Value       value_;
  std::string field_name_;
};
