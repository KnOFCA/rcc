#include "FrontSymtab.h"

namespace rcc {

/* ============================================================ */
/* Scope                                                       */
/* ============================================================ */

Scope::Scope(Scope *parent)
    : parent_(parent) {}

bool Scope::insert(const std::shared_ptr<Symbol> &sym) {
    return objects_.emplace(sym->name, sym).second;
}

bool Scope::insertTag(const std::shared_ptr<TagSymbol> &sym) {
    return tags_.emplace(sym->name, sym).second;
}

std::shared_ptr<Symbol> Scope::lookupLocal(const std::string &name) {
    auto it = objects_.find(name);
    if (it == objects_.end())
        return nullptr;
    return it->second;
}

std::shared_ptr<TagSymbol> Scope::lookupTagLocal(const std::string &name) {
    auto it = tags_.find(name);
    if (it == tags_.end())
        return nullptr;
    return it->second;
}

std::shared_ptr<Symbol> Scope::lookup(const std::string &name) {
    for (Scope *s = this; s; s = s->parent_) {
        auto it = s->objects_.find(name);
        if (it != s->objects_.end())
            return it->second;
    }
    return nullptr;
}

std::shared_ptr<TagSymbol> Scope::lookupTag(const std::string &name) {
    for (Scope *s = this; s; s = s->parent_) {
        auto it = s->tags_.find(name);
        if (it != s->tags_.end())
            return it->second;
    }
    return nullptr;
}

/* ============================================================ */
/* SymbolTable                                                 */
/* ============================================================ */

SymbolTable::SymbolTable() {
    enterScope(); // file scope
}

void SymbolTable::enterScope() {
    Scope *parent = current_;
    scopes_.push_back(std::make_unique<Scope>(parent));
    current_ = scopes_.back().get();
}

void SymbolTable::leaveScope() {
    if (!current_)
        return;
    current_ = current_->parent();
}

Scope *SymbolTable::currentScope() {
    return current_;
}

bool SymbolTable::define(const std::shared_ptr<Symbol> &sym) {
    return current_->insert(sym);
}

bool SymbolTable::defineTag(const std::shared_ptr<TagSymbol> &sym) {
    return current_->insertTag(sym);
}

std::shared_ptr<Symbol> SymbolTable::lookup(const std::string &name) {
    if (!current_)
        return nullptr;
    return current_->lookup(name);
}

std::shared_ptr<TagSymbol> SymbolTable::lookupTag(const std::string &name) {
    if (!current_)
        return nullptr;
    return current_->lookupTag(name);
}

} // namespace rcc
