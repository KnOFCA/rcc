#pragma once

#include "AST.h"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>

namespace rcc::front {

// CRTP-based AST visitor. Derived should inherit as:
//   struct MyVisitor : ASTVisitor<MyVisitor> { ... };
// Derived may override any of the visitXXX methods; the base
// implementations here perform a recursive traversal.
template <class Derived>
class ASTVisitor {
  protected:
  // 访问者实例（静态转换到派生类）
  Derived& derived() { return static_cast<Derived&>(*this); }
  const Derived& derived() const { return static_cast<const Derived&>(*this); }

  public:
  virtual ~ASTVisitor() = default;

  // Convenience template: accept shared_ptr<T>
  template<typename T>
  void visit(const std::shared_ptr<T> &node) { if (!node) return; derived().visitImpl(node); }

  // 默认实现：对AST节点的通用处理（不可识别时作为回退）
  void visitImpl(const std::shared_ptr<ast::ASTNode> &node) { /*throw std::runtime_error("can not deal ASTNode.");*/ std::cout << "no match node." << std::endl; }

  // 翻译单元
  void visitImpl(const std::shared_ptr<ast::TranslationUnit> &tu) {
    if (!tu) return;
    for (auto &ed : tu->externalDecls) visit(ed);
  }

  void visitImpl(const std::shared_ptr<ast::ExternalDecl> &ed) {
    if (!ed) return;
    if (auto decl = std::dynamic_pointer_cast<ast::Declaration>(ed)) {visit(decl); return;}
    if (auto func = std::dynamic_pointer_cast<ast::FunctionDef>(ed)) {visit(func); return;}    
  }

  // 声明 / 定义
  void visitImpl(const std::shared_ptr<ast::Declaration> &decl) {
    if (!decl) return;
    visit(decl->specs);
    for (auto &init : decl->initDeclarators) {
      if (!init) continue;
      visit(init);
    }
  }

  void visitImpl(const std::shared_ptr<ast::FunctionDef> &fdef) {
    if (!fdef) return;
    if (fdef->declarator) visit((fdef->declarator));
    if (fdef->body) visit(fdef->body);
  }

  // 语句层次
  void visitImpl(const std::shared_ptr<ast::Stmt> &stmt) {
    if (!stmt) return;
    if (auto comp = std::dynamic_pointer_cast<ast::CompoundStmt>(stmt)) { visit(comp); return; }
    if (auto exprStmt = std::dynamic_pointer_cast<ast::ExprStmt>(stmt)) { if (exprStmt->expr) visit(std::dynamic_pointer_cast<ast::Expr>(exprStmt->expr)); return; }
    if (auto ifs = std::dynamic_pointer_cast<ast::IfStmt>(stmt)) { if (ifs->cond) visit(ifs->cond); if (ifs->thenStmt) visit(ifs->thenStmt); if (ifs->elseStmt) visit(ifs->elseStmt); return; }
    if (auto ws = std::dynamic_pointer_cast<ast::WhileStmt>(stmt)) { if (ws->cond) visit(ws->cond); if (ws->body) visit(ws->body); return; }
    if (auto rs = std::dynamic_pointer_cast<ast::ReturnStmt>(stmt)) { if (rs->expr) visit(std::dynamic_pointer_cast<ast::Expr>(rs->expr)); return; }
    if (auto forS = std::dynamic_pointer_cast<ast::ForStmt>(stmt)) { if (forS->init) visit(forS->init); if (forS->cond) visit(forS->cond); if (forS->iter) visit(forS->iter); if (forS->body) visit(forS->body); return; }
  }

  void visitImpl(const std::shared_ptr<ast::CompoundStmt> &comp) {
    if (!comp) return;
    for (auto &item : comp->items) {
      if (!item) continue;
      if (auto di = std::dynamic_pointer_cast<ast::DeclItem>(item)) { if (di->decl) visit((di->decl)); }
      else if (auto si = std::dynamic_pointer_cast<ast::StmtItem>(item)) { if (si->stmt) visit(si->stmt); }
    }
  }

  // 表达式层次
  void visitImpl(const std::shared_ptr<ast::Expr> &expr) {
    if (!expr) return;
    if (auto id = std::dynamic_pointer_cast<ast::IdExpr>(expr)) { derived().visitImpl(id); return; }
    if (auto lit = std::dynamic_pointer_cast<ast::LiteralExpr>(expr)) { derived().visitImpl(lit); return; }
    if (auto un = std::dynamic_pointer_cast<ast::UnaryExpr>(expr)) { derived().visitImpl(un); return; }
    if (auto bin = std::dynamic_pointer_cast<ast::BinaryExpr>(expr)) { derived().visitImpl(bin); return; }
    if (auto cond = std::dynamic_pointer_cast<ast::ConditionalExpr>(expr)) { derived().visitImpl(cond); return; }
    if (auto call = std::dynamic_pointer_cast<ast::CallExpr>(expr)) { derived().visitImpl(call); return; }
    if (auto post = std::dynamic_pointer_cast<ast::PostfixExpr>(expr)) { derived().visitImpl(post); return; }
    if (auto cast = std::dynamic_pointer_cast<ast::CastExpr>(expr)) { derived().visitImpl(cast); return; }
  }

  void visitImpl(const std::shared_ptr<ast::IdExpr> &id) { if (!id) return; std::cout << "IdExpr: " << id->name << std::endl; }
  void visitImpl(const std::shared_ptr<ast::LiteralExpr> &lit) {
    if (!lit) return;
    std::string t;
    switch (lit->type) {
      case ast::LiteralExpr::INTEGER: t = "INTEGER"; break;
      case ast::LiteralExpr::FLOAT: t = "FLOAT"; break;
      case ast::LiteralExpr::CHAR: t = "CHAR"; break;
      case ast::LiteralExpr::STRING: t = "STRING"; break;
      default: t = "UNKNOWN"; break;
    }
    std::cout << "Literal(" << t << "): " << lit->value << std::endl;
  }

  void visitImpl(const std::shared_ptr<ast::UnaryExpr> &un) { 
    if (!un) return; 
    if (auto ope = std::dynamic_pointer_cast<ast::UnaryExpr>(un->operand)) visit(un->operand); 
    
  }
  void visitImpl(const std::shared_ptr<ast::BinaryExpr> &bin) { if (!bin) return; if (bin->lhs) visit(bin->lhs); if (bin->rhs) visit(bin->rhs); }
  void visitImpl(const std::shared_ptr<ast::ConditionalExpr> &ce) { if (!ce) return; if (ce->cond) visit(ce->cond); if (ce->thenExpr) visit(ce->thenExpr); if (ce->elseExpr) visit(ce->elseExpr); }
  void visitImpl(const std::shared_ptr<ast::CallExpr> &call) { if (!call) return; if (call->callee) visit(call->callee); for (auto &a : call->args) if (a) visit(a); }
  void visitImpl(const std::shared_ptr<ast::PostfixExpr> &post) { 
    if (!post) return; 
    if (post->op == ast::opcode::ARRAY) visit(std::dynamic_pointer_cast<ast::Expr>(post->array));
    if (auto expr = std::dynamic_pointer_cast<ast::Expr>(post->expr)) visit(expr); 
  }
  void visitImpl(const std::shared_ptr<ast::CastExpr> &cast) {
    if (!cast) return;
    std::cout << "visit cast expr" << std::endl;
    if (cast->type) visit(cast->type);
    if (auto expr = std::dynamic_pointer_cast<ast::Expr>(cast->expr)) 
      visit(expr);
  }

  void visitImpl(const std::shared_ptr<ast::Declarator> &decl) { if (!decl) return; if (decl->pointer) visit(decl->pointer); if (decl->direct) visit(decl->direct); }
  void visitImpl(const std::shared_ptr<ast::TypeName> &tn) { if (!tn) return; if (tn->specs) visit(tn->specs); if (tn->abstractDecl) visit(tn->abstractDecl); }

  // Decl specifiers / types
  void visitImpl(const std::shared_ptr<ast::DeclSpecifier> &ds) { 
    if(auto sc = std::dynamic_pointer_cast<ast::StorageClassSpec>(ds)) {visit(sc); return;}
    if(auto tq = std::dynamic_pointer_cast<ast::TypeQualifierSpec>(ds)) {visit(tq); return;}
    if(auto ts = std::dynamic_pointer_cast<ast::TypeSpecifier>(ds)) {visit(ts); return;} 
  }

  void visitImpl(const std::shared_ptr<ast::StorageClassSpec> &sc) {
    if (!sc) return;
    switch (sc->storageClass) {
      case ast::StorageClassSpec::Class::Auto: std::cout << "StorageClass: auto" << std::endl; break;
      case ast::StorageClassSpec::Class::Register: std::cout << "StorageClass: register" << std::endl; break;
      case ast::StorageClassSpec::Class::Static: std::cout << "StorageClass: static" << std::endl; break;
      case ast::StorageClassSpec::Class::Extern: std::cout << "StorageClass: extern" << std::endl; break;
      case ast::StorageClassSpec::Class::Typedef: std::cout << "StorageClass: typedef" << std::endl; break;
      default: std::cout << "StorageClass: NONE" << std::endl; break;
    }
  }

  void visitImpl(const std::shared_ptr<ast::TypeQualifierSpec> &tq) {
    if (!tq) return;
    switch (tq->qualifier) {
      case ast::TypeQualifierSpec::Qualifier::Const: std::cout << "TypeQualifier: const" << std::endl; break;
      case ast::TypeQualifierSpec::Qualifier::Volatile: std::cout << "TypeQualifier: volatile" << std::endl; break;
      case ast::TypeQualifierSpec::Qualifier::Restrict: std::cout << "TypeQualifier: restrict" << std::endl; break;
      case ast::TypeQualifierSpec::Qualifier::Atomic: std::cout << "TypeQualifier: atomic" << std::endl; break;
      default: std::cout << "TypeQualifier: NONE" << std::endl; break;
    }
  }

  void visitImpl(const std::shared_ptr<ast::TypeSpecifier> &ts) { 
    if(auto bt = std::dynamic_pointer_cast<ast::BuiltinTypeSpec>(ts)) {visit(bt); return;}
    if(auto st = std::dynamic_pointer_cast<ast::StructTypeSpec>(ts)) {visit(st); return;}
    if(auto ut = std::dynamic_pointer_cast<ast::UnionTypeSpec>(ts)) {visit(ut); return;}
    if(auto et = std::dynamic_pointer_cast<ast::EnumTypeSpec>(ts)) {visit(et); return;} 
  }

  void visitImpl(const std::shared_ptr<ast::BuiltinTypeSpec> &bts) {
    if (!bts) return;
    switch (bts->builtin) {
      case ast::BuiltinTypeSpec::Builtin::Void: std::cout << "BuiltinType: void" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Char: std::cout << "BuiltinType: char" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Short: std::cout << "BuiltinType: short" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Int: std::cout << "BuiltinType: int" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Long: std::cout << "BuiltinType: long" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Float: std::cout << "BuiltinType: float" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Double: std::cout << "BuiltinType: double" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Signed: std::cout << "BuiltinType: signed" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Unsigned: std::cout << "BuiltinType: unsigned" << std::endl; break;
      case ast::BuiltinTypeSpec::Builtin::Bool: std::cout << "BuiltinType: bool" << std::endl; break;
      default: std::cout << "BuiltinType: NONE" << std::endl; break;
    }
  }

  void visitImpl(const std::shared_ptr<ast::StructTypeSpec> &sts) { if (!sts) return; for (auto &f : sts->fields) if (f) visit(f); }
  void visitImpl(const std::shared_ptr<ast::UnionTypeSpec> &uts) { if (!uts) return; for (auto &f : uts->fields) if (f) visit(f); }
  void visitImpl(const std::shared_ptr<ast::EnumTypeSpec> &ets) { if (!ets) return; for (auto &e : ets->enumerators) if (e.value) visit(e.value); }
  void visitImpl(const std::shared_ptr<ast::DeclSpec> &ds) { if (!ds) return; for (auto &s : ds->specs) if (s) visit(s); }

  // Pointer / declarators
  void visitImpl(const std::shared_ptr<ast::Pointer> &ptr) { if (!ptr) return; for (auto &q : ptr->qualifiers) if (q) visit(q); if (ptr->next) visit(ptr->next); }

  void visitImpl(const std::shared_ptr<ast::DirectDeclarator> &dd) {
    if (!dd) return;
    if (auto id = std::dynamic_pointer_cast<ast::DDIdentifier>(dd)) { derived().visitImpl(id); return; }
    if (auto p = std::dynamic_pointer_cast<ast::DDParen>(dd)) { derived().visitImpl(p); return; }
    if (auto a = std::dynamic_pointer_cast<ast::DDArray>(dd)) { derived().visitImpl(a); return; }
    if (auto c = std::dynamic_pointer_cast<ast::DDCall>(dd)) { derived().visitImpl(c); return; }
  }

  void visitImpl(const std::shared_ptr<ast::DDIdentifier> &id) { if (!id) return; std::cout << "DDIdentifier: " << id->name << std::endl; }
  //TODO: specify inner visit type
  void visitImpl(const std::shared_ptr<ast::DDParen> &dp) { if (!dp) return; if (dp->inner) visit(dp->inner); }
  //TODO: specify size visit type
  void visitImpl(const std::shared_ptr<ast::DDArray> &da) { if (!da) return; if (da->base) visit(da->base); if (auto size = std::dynamic_pointer_cast<ast::Expr>(da->size)) visit(size); }
  //TODO: specify base visit type
  void visitImpl(const std::shared_ptr<ast::DDCall> &dc) { if (!dc) return; if (dc->base) visit(dc->base); for (auto &p : dc->params) if (p) visit(p); }
  //TODO: specify declarator visit type
  void visitImpl(const std::shared_ptr<ast::ParameterDecl> &pd) { if (!pd) return; if (pd->specs) visit(pd->specs); if (pd->declarator) visit(pd->declarator); }

  void visitImpl(const std::shared_ptr<ast::AbstractDeclarator> &ad) { if (!ad) return; if (ad->pointer) visit(ad->pointer); if (ad->direct) visit(ad->direct); }

  void visitImpl(const std::shared_ptr<ast::DirectAbstractDeclarator> &dad) {
    if (!dad) return;
    if (auto p = std::dynamic_pointer_cast<ast::DAParen>(dad)) { derived().visitImpl(p); return; }
    if (auto a = std::dynamic_pointer_cast<ast::DAArray>(dad)) { derived().visitImpl(a); return; }
    if (auto c = std::dynamic_pointer_cast<ast::DACall>(dad)) { derived().visitImpl(c); return; }
  }

  void visitImpl(const std::shared_ptr<ast::DAParen> &dap) { if (!dap) return; if (dap->inner) visit(dap->inner); }
  void visitImpl(const std::shared_ptr<ast::DAArray> &daa) { if (!daa) return; if (daa->base) visit(daa->base); if (daa->size) visit(daa->size); }
  void visitImpl(const std::shared_ptr<ast::DACall> &dac) { if (!dac) return; if (dac->base) visit(dac->base); for (auto &p : dac->params) if (p) visit(p); }

  // Initializers / init-declarator
  void visitImpl(const std::shared_ptr<ast::Initializer> &init) { 
    if (!init) return;
    if (auto exprInit = std::dynamic_pointer_cast<ast::ExprInitializer>(init)) {visit(exprInit); return;}
    if (auto initList = std::dynamic_pointer_cast<ast::InitList>(init)) {visit(initList); return;}
  }
  //TODO: specify expr visit type
  void visitImpl(const std::shared_ptr<ast::ExprInitializer> &ei) { if (!ei) return; if (auto exp = std::dynamic_pointer_cast<ast::Expr>(ei->expr)) visit(exp); }
  void visitImpl(const std::shared_ptr<ast::InitList> &il) { if (!il) return; for (auto &e : il->elements) if (e) visit((e)); }
  void visitImpl(const std::shared_ptr<ast::InitDeclarator> &id) { if (!id) return; if (id->declarator) visit(id->declarator); if (id->initializer) visit(id->initializer); }

  // Block items
  void visitImpl(const std::shared_ptr<ast::BlockItem> &bi) { if (!bi) return; if (auto di = std::dynamic_pointer_cast<ast::DeclItem>(bi)) { derived().visitImpl(di); return; } if (auto si = std::dynamic_pointer_cast<ast::StmtItem>(bi)) { derived().visitImpl(si); return; } }
  void visitImpl(const std::shared_ptr<ast::DeclItem> &di) { if (!di) return; if (di->decl) visit((di->decl)); }
  void visitImpl(const std::shared_ptr<ast::StmtItem> &si) { if (!si) return; if (si->stmt) visit(si->stmt); }

  // Labeled
  void visitImpl(const std::shared_ptr<ast::LabeledStmt> &ls) { (void)ls; }
  void visitImpl(const std::shared_ptr<ast::LabelStmt> &ls) { if (!ls) return; std::cout << "Label: " << ls->label << std::endl; if (ls->stmt) visit(ls->stmt); }
  void visitImpl(const std::shared_ptr<ast::CaseStmt> &cs) { if (!cs) return; if (cs->expr) visit(std::dynamic_pointer_cast<ast::Expr>(cs->expr)); if (cs->stmt) visit(cs->stmt); }
  void visitImpl(const std::shared_ptr<ast::DefaultStmt> &ds) { if (!ds) return; if (ds->stmt) visit(ds->stmt); }

  // Additional statements
  void visitImpl(const std::shared_ptr<ast::SwitchStmt> &ss) { if (!ss) return; if (ss->expr) visit(std::dynamic_pointer_cast<ast::Expr>(ss->expr)); if (ss->stmt) visit(ss->stmt); }
  void visitImpl(const std::shared_ptr<ast::DoWhileStmt> &dws) { if (!dws) return; if (dws->body) visit(dws->body); if (dws->cond) visit(dws->cond); }
  void visitImpl(const std::shared_ptr<ast::GotoStmt> &gs) { if (!gs) return; std::cout << "Goto: " << gs->label << std::endl; }
  void visitImpl(const std::shared_ptr<ast::ContinueStmt> &cs2) { (void)cs2; std::cout << "Continue" << std::endl; }
  void visitImpl(const std::shared_ptr<ast::BreakStmt> &bs) { (void)bs; std::cout << "Break" << std::endl; }

  // Struct/Union/Enum
  void visitImpl(const std::shared_ptr<ast::StructDecl> &sd) { if (!sd) return; if (sd->specs) visit(sd->specs); for (auto &d : sd->declarators) if (d) visit(d); }
  void visitImpl(const std::shared_ptr<ast::StructSpec> &ss) { if (!ss) return; if (!ss->name.empty()) std::cout << "StructSpec: " << ss->name << std::endl; for (auto &m : ss->members) if (m) visit(m); }
  void visitImpl(const std::shared_ptr<ast::UnionSpec> &us) { if (!us) return; if (!us->name.empty()) std::cout << "UnionSpec: " << us->name << std::endl; for (auto &m : us->members) if (m) visit(m); }
  void visitImpl(const std::shared_ptr<ast::Enumerator> &e) { if (!e) return; std::cout << "Enumerator: " << e->name << std::endl; if (e->value) visit(e->value); }
  void visitImpl(const std::shared_ptr<ast::EnumSpec> &es) { if (!es) return; if (!es->name.empty()) std::cout << "EnumSpec: " << es->name << std::endl; for (auto &en : es->enumerators) if (en) visit(en); }
};

} // namespace rcc::front

