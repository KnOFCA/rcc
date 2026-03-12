#include "IRVisitor.h"

namespace rcc::ir::detail {

namespace {

bool item_matches_kind(const Slice::ItemsPtr& item, SliceItemKind kind) {
    switch (kind) {
        case SliceItemKind::TYPE:
            return std::holds_alternative<Type>(item);
        case SliceItemKind::FUNCTION:
            return std::holds_alternative<Function>(item);
        case SliceItemKind::BASIC_BLOCK:
            return std::holds_alternative<BasicBlock>(item);
        case SliceItemKind::VALUE:
            return std::holds_alternative<Value>(item);
        case SliceItemKind::UNKNOWN:
            return true;
    }
    return false;
}

} // namespace

bool visit_slice_items(const Slice& slice, SliceItemKind expectedKind,
                       const SliceVisitor& visitor) {
    for (const auto& item : slice.buffer) {
        if (slice.kind != SliceItemKind::UNKNOWN && slice.kind != expectedKind) {
            continue;
        }
        if (!item_matches_kind(item, expectedKind)) {
            continue;
        }
        if (!visitor(item)) {
            return false;
        }
    }
    return true;
}

} // namespace rcc::ir::detail
