#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

#include "PKBBuilder.hpp"

std::vector<var_ref> extractVariableFromSubtree(TNode cur_node) {

    if (cur_node.getType() == VARIABLE) {
        return std::vector<var_ref>({cur_node.getValue()});
    }

    std::vector<var_ref> results;
    for (auto child: cur_node.getChildren()) {
        std::vector<var_ref> subtree_results = extractVariableFromSubtree(child);
        results.insert(results.end(), subtree_results.begin(), subtree_results.end());
    }

    return results;
}

std::vector<var_ref> computeModifies(TNode cur_node, PKB &pkb) {
    
    stmt_type type = cur_node.getType();

    std::vector<var_ref> cur_node_modifies;
    std::vector<var_ref> buffer;

    switch (type)
    {
    case ASSIGN:
        // consider the left hand side only
        buffer = extractVariableFromSubtree(cur_node.getChildren()[0]);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        break;
    case IF:
        // check the "then" block
        buffer = computeModifies(cur_node.getChildren()[1], pkb);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        // check the "else" block
        buffer = computeModifies(cur_node.getChildren()[2], pkb);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        break;
    case WHILE:
        buffer = computeModifies(cur_node.getChildren()[1], pkb);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        break;
    case READ:
        buffer = extractVariableFromSubtree(cur_node.getChildren()[0]);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        break;
    case STATEMENTLIST:
        for (auto child: cur_node.getChildren()) {
            buffer = computeModifies(child, pkb);
            cur_node_modifies.insert(cur_node_modifies.end(), 
                    buffer.begin(), buffer.end());
        }
        break;
    case PROCEDURE:
        // check the statement list
        buffer = computeModifies(cur_node.getChildren()[0], pkb);
        cur_node_modifies.insert(cur_node_modifies.end(), buffer.begin(), buffer.end());
        pkb.procedures[cur_node.getValue()].modifies.insert(cur_node_modifies.begin(), 
                cur_node_modifies.end());
        break;
    default:
        break;
    }

    stmt_ref stmt_no = cur_node.getStatementNum();

    if (stmt_no != NULL_STMT_REF) {
        pkb.statements[stmt_no].modifies.insert(cur_node_modifies.begin(), 
                cur_node_modifies.end());
    }

    return cur_node_modifies;
}

std::vector<var_ref> computeUses(TNode cur_node, PKB &pkb) {
    
    stmt_type type = cur_node.getType();

    std::vector<var_ref> cur_node_uses;
    std::vector<var_ref> buffer;

    switch (type)
    {
    case ASSIGN:
        // consider the right hand side only
        buffer = extractVariableFromSubtree(cur_node.getChildren()[1]);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        break;
    case IF:
        // check the conditional block
        buffer = extractVariableFromSubtree(cur_node.getChildren()[0]);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        // check the "then" block
        buffer = computeUses(cur_node.getChildren()[1], pkb);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        // check the "else" block
        buffer = computeUses(cur_node.getChildren()[2], pkb);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        break;
    case WHILE:
        // check the conditionals
        buffer = extractVariableFromSubtree(cur_node.getChildren()[0]);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        // check the nested block
        buffer = computeUses(cur_node.getChildren()[1], pkb);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        break;
    case PRINT:
        buffer = extractVariableFromSubtree(cur_node.getChildren()[0]);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        break;
    case STATEMENTLIST:
        for (auto child: cur_node.getChildren()) {
            buffer = computeUses(child, pkb);
            cur_node_uses.insert(cur_node_uses.end(), 
                    buffer.begin(), buffer.end());
        }
        break;
    case PROCEDURE:
        // check the statement list
        buffer = computeUses(cur_node.getChildren()[0], pkb);
        cur_node_uses.insert(cur_node_uses.end(), buffer.begin(), buffer.end());
        pkb.procedures[cur_node.getValue()].uses.insert(cur_node_uses.begin(), 
                cur_node_uses.end());
        break;
    default:
        break;
    }

    stmt_ref stmt_no = cur_node.getStatementNum();

    if (stmt_no != NULL_STMT_REF) {
        pkb.statements[stmt_no].uses.insert(cur_node_uses.begin(), 
                cur_node_uses.end());
    }

    return cur_node_uses;
}

stmt_ref computeParentAndFollows(
        TNode cur_node, 
        stmt_ref parent,
        std::vector<stmt_ref> indirectParents, // this vector is passed by value
        std::unordered_map<stmt_ref, statement>& statements
) {
    // returns the statement number for follows computation
    stmt_ref return_value = NULL_STMT_REF;

    // Do parent relationship computation
    if (cur_node.getStatementNum() != NULL_STMT_REF) {
        stmt_ref cur_node_ref = cur_node.getStatementNum();
        statements[cur_node_ref].indirect_parent.insert(indirectParents.begin(), indirectParents.end());
        statements[cur_node_ref].parent = parent;

        indirectParents.push_back(cur_node_ref);
        parent = cur_node_ref;

        return_value = cur_node_ref;
    }

    // Recursively do the computation
    std::vector<stmt_ref> children;
    for (auto child: cur_node.getChildren()) {
        stmt_ref child_ref = computeParentAndFollows(child, parent, indirectParents, statements);
        if (child.getStatementNum() != NULL_STMT_REF) {
            children.push_back(child_ref);
        }
    }

    stmt_ref last = NULL_STMT_REF;
    std::vector<stmt_ref> previous;

    std::reverse(children.begin(), children.end());

    // Do follows relationship computation
    if (cur_node.getType() == STATEMENTLIST) {
        for(auto child: children) {
            statements[child].follows = last;
            statements[child].indirect_follows.insert(previous.begin(), previous.end());

            last = child;
            previous.push_back(child);
        }
    }

    return return_value;
}

// Utility function for converting an expression's tree into text that can be used in pattern matching
std::string makeMatchableStringFromAST(TNode cur_node) {
    std::string buffer;
    switch (cur_node.getType())
    {
    case VARIABLE:
    case CONSTANT:
        return cur_node.getValue();
    case EXPR:
    case OPERATOR:
        buffer = buffer + "(";
        buffer = buffer + makeMatchableStringFromAST(cur_node.getChildren()[0]);
        buffer = buffer + cur_node.getValue();
        buffer = buffer + makeMatchableStringFromAST(cur_node.getChildren()[1]);
        buffer = buffer + ")";
        return buffer;
    default:
        throw "malformed statement tree";
        break;
    }
}

void populateAssignmentArray(PKB &pkb) {
    for (auto statement: pkb.statements) {
        if (statement.second.type == ASSIGN) {
            TNode rvalue = statement.second.ast.getChildren()[1];

            stmt_ref stmt_no = statement.first;
            std::string value = makeMatchableStringFromAST(rvalue);
            
            // note that we do not need the full statement specifications for the assignment table
            pkb.assignments[stmt_no] = {value};
            
        }
    }
}

void extractDesignEntities(TNode cur_node, PKB &pkb) {
    stmt_type type = cur_node.getType();
    procedure p;
    variable v;
    statement s;
    const_value c;

    switch (type)
    {
        case PROCEDURE:
            p = {cur_node.getValue(), cur_node};
            pkb.procedures[cur_node.getValue()] = p;

            for (auto child: cur_node.getChildren()) {
                extractDesignEntities(child, pkb);
            }
            break;
        case CALL:
        case READ:
        case PRINT:
        case IF:
        case ASSIGN:
        case WHILE:
            s = {
                cur_node.getStatementNum(),
                type,
                cur_node,
            };
            pkb.statements[cur_node.getStatementNum()] = s;
            
            for (auto child: cur_node.getChildren()) {
                extractDesignEntities(child, pkb);
            }
            break;
        case CONSTANT:
            c = cur_node.getValue();
            pkb.constants.emplace(c);
            break;
        case VARIABLE:
            v = {cur_node.getValue()};
            pkb.variables[cur_node.getValue()] = v;  
            break;
        default:
            for (auto child: cur_node.getChildren()) {
                extractDesignEntities(child, pkb);
            }
            break;
    }
}

void computePKB(PKB &pkb) {
    extractDesignEntities(pkb.ast_root, pkb);

    computeParentAndFollows(
        pkb.ast_root,
        NULL_STMT_REF,
        std::vector<stmt_ref>(),
        pkb.statements
    );
    
    for (auto proc: pkb.procedures) {
        computeModifies(proc.second.ast, pkb);
        computeUses(proc.second.ast, pkb);
    }

    // populate the extra assignment array used in pattern computation
    populateAssignmentArray(pkb);
}

PKBBuilder::PKBBuilder(TNode root) {
    // Note that the rest of the fields should initialize themselves as empty by default
    PKBBuilder::pkb.ast_root = root;
}

// Traverse through the tree rooted at node pkb.ast_root to generate and return the PKB 
PKB PKBBuilder::build() {
    computePKB(pkb);

    return pkb;
}
