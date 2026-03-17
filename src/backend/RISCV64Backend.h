#pragma once

#include "backend/Backend.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

namespace rcc::backend {

class RISCV64Backend final : public Backend {
  public:
    BackendError emit_program(const ir::Program& program,
                              std::ostream& out) override;

  private:
    struct StackSlot {
        std::size_t offset{0};
        std::size_t size{0};
        std::size_t align{1};
    };

    struct FrameLayout {
        std::size_t stack_size{0};
        std::size_t outgoing_arg_size{0};
        std::size_t ra_offset{0};
        bool save_ra{false};
        std::unordered_map<const ir::ValueData*, StackSlot> slots;
    };

    struct FunctionContext {
        const ir::FunctionData* function{nullptr};
        FrameLayout frame;
        std::unordered_map<const ir::BasicBlockData*, std::string> block_labels;
        std::size_t next_block_id{0};
    };

    BackendError emit_global(const ir::Value& global, std::ostream& out);
    BackendError emit_function(const ir::Function& function, std::ostream& out);
    BackendError build_frame_layout(const ir::Function& function,
                                    FrameLayout& frame);
    BackendError emit_function_prologue(const FunctionContext& ctx,
                                        std::ostream& out);
    BackendError emit_function_epilogue(const FunctionContext& ctx,
                                        std::ostream& out);
    BackendError emit_basic_block(const ir::BasicBlock& bb,
                                  FunctionContext& ctx,
                                  std::ostream& out);
    BackendError emit_instruction(const ir::Value& inst,
                                  FunctionContext& ctx,
                                  std::ostream& out);
    BackendError emit_global_initializer(const ir::Value& init,
                                         ir::Type type,
                                         std::ostream& out);

    std::string function_symbol(const ir::Function& function) const;
    std::string value_symbol(const ir::Value& value) const;
    std::string block_label(const ir::BasicBlock& bb, FunctionContext& ctx);

    BackendError emit_value_to_reg(const ir::Value& value,
                                   const std::string& reg,
                                   FunctionContext& ctx,
                                   std::ostream& out);
    BackendError emit_address_to_reg(const ir::Value& value,
                                     const std::string& reg,
                                     FunctionContext& ctx,
                                     std::ostream& out);
    BackendError emit_load_from_address(const std::string& addr_reg,
                                        ir::Type type,
                                        const std::string& dst_reg,
                                        std::ostream& out);
    BackendError emit_store_to_address(const std::string& src_reg,
                                       const std::string& addr_reg,
                                       ir::Type type,
                                       std::ostream& out);
    BackendError emit_store_reg_to_slot(const std::string& src_reg,
                                        const StackSlot& slot,
                                        ir::Type type,
                                        std::ostream& out);
    BackendError emit_load_slot_to_reg(const StackSlot& slot,
                                       ir::Type type,
                                       const std::string& dst_reg,
                                       std::ostream& out);

    std::optional<std::int64_t> extract_integer(const ir::Value& value) const;
    std::size_t type_size(ir::Type type) const;
    std::size_t type_align(ir::Type type) const;
    bool is_zero_initializer(const ir::Value& value) const;
};

BackendError emit_riscv64(const ir::Program& program, std::ostream& out);

} // namespace rcc::backend
