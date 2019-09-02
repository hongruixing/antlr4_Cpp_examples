#include <cmath>

#include "antlr4-runtime.h"
#include "SimpleExpr1Lexer.h"
#include "SimpleExpr1Parser.h"
#include "SimpleExpr1Visitor.h"

using namespace antlr4;
using namespace std;


string indent_sp(int size)
{ 
    string result = "";
    for(int i = 0; i < size; i++)
      result += " ";
    return result;
}


string beautify_lisp_string(string in_string)
{
    int indent_size = 3;
    int curr_indent = 0;
    string out_string = string(1,in_string[0]);
    string indent = "";
    for (int i = 1; i < in_string.size(); i++)
        if(in_string[i] == '(' and  in_string[i+1] != ' '){
           curr_indent += indent_size;
           out_string += '\n' + indent_sp(curr_indent) + '(';
        }
        else if(in_string[i] == ')'){
          out_string += ')';
          if(curr_indent > indent_size)
            curr_indent -= indent_size;
        }
        else
          out_string += in_string[i];
     
return out_string;
}


class Calc : public SimpleExpr1Visitor{
public:
 
    antlrcpp::Any visitStat(SimpleExpr1Parser::StatContext *ctx) override {
        return visit(ctx->expr());
    }


    antlrcpp::Any visitAdd(SimpleExpr1Parser::AddContext *ctx) override {
        int left  = visit(ctx->expr(0));
        int right = visit(ctx->expr(1));
        return left + right;
    }


    antlrcpp::Any visitExpo(SimpleExpr1Parser::ExpoContext *ctx) override {
        int left  = visit(ctx->expr(0));
        int right = visit(ctx->expr(1));
        return (int) pow(left, right);
    }


    antlrcpp::Any visitMult(SimpleExpr1Parser::MultContext *ctx) override {
        int right = visit(ctx->expr(0));
        int left  = visit(ctx->expr(1));
        return left * right; 
    }


    antlrcpp::Any visitInt(SimpleExpr1Parser::IntContext *ctx) override {
        return stoi(ctx->INT()->getText());
    }

};



int main(int argc, const char *args[])
{
    ifstream ifs;

    ifs.open(args[1]);
    ANTLRInputStream input(ifs);

    SimpleExpr1Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    cout << "Tokens:" << endl;
    tokens.fill();
    for (Token *token : tokens.getTokens())
    cout << token->toString() << endl;

    SimpleExpr1Parser parser(&tokens);
    tree::ParseTree *tree = parser.stat();
    cout << endl << "Parse tree (Lisp format):" << endl;
    string lisp_tree_str = tree->toStringTree(&parser);
    //cout <<lisp_tree_str  << endl;
    cout <<beautify_lisp_string(lisp_tree_str) << endl;

    Calc *calc = new Calc();
    int result = calc->visit(tree);
    cout << "\nresult = " << result << endl;

    return 0;
}
