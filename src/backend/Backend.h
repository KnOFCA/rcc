// Author: KnOFCA

#pragma once

#include "ir/koopa.h"

#include <ostream>
#include <string>

namespace rcc::backend {

enum class BackendErrorCode {
    SUCCESS = 0,
    INVALID_PROGRAM,
    INVALID_OUTPUT,
    UNSUPPORTED_IR,
    UNSUPPORTED_TYPE,
    INTERNAL_ERROR,
};

struct BackendError {
    BackendErrorCode code{BackendErrorCode::SUCCESS};
    std::string message;

    [[nodiscard]] bool ok() const noexcept {
        return code == BackendErrorCode::SUCCESS;
    }

    static BackendError success() {
        return {};
    }
};

class Backend {
  public:
    virtual ~Backend() = default;

    virtual BackendError emit_program(const ir::Program& program,
                                      std::ostream& out) = 0;
};

} // namespace rcc::backend