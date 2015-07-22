// 0 [x] Base visitor class should define visit_* in terms of its leaf nodes
// 1 [.] Break up Analyze into AnalyzeType, AnalyzeUsage, and CheckType
// 2 [.] Move _x_* from AnalyzeType into the scope construct
// 3 [ ] Determine solution for anonymous scopes (scopes not tied to an Item)
// 4 [ ] Block expressions
// 5 [ ] CTA
// 6 [ ] AnalyzeType should add contextual types of usage to its inferrence
// 7 [ ] type(...)
// 8 [ ] Move typing of extern_* into analyze

// let var = 20; // decl(var) + assign(var, type(20))
// {
//   let a = var; // decl(a) + assign(a, type(var)) + use(var, type(a))
// }

// immutable variables are treated as immutable only within the current
// named scope; they become constant once we leave that

// [x] Rewrite code::Scope to code::Block (what scope was) / code::Scope (stack of blocks)
// [ ] AnalyzeUsage: Block-scoping
// [ ] AnalyzeUsage: Select
// [ ] AnalyzeUsage: Loop
// [ ] AnalyzeUsage: Function
// [ ] AnalyzeUsage: _warnings_
// [ ] AnalyzeType: _initial_
// [ ] AnalyzeType: CTA
// [ ] AnalyzeType: type(...)
// [ ] AnalyzeType: extern_*
// [ ] Build: Function
// [ ] Build: Call (finish)
// [ ] 95+% code coverage
// [ ] Switch back to using shared_ptr and friends (remove Ref<T>)
