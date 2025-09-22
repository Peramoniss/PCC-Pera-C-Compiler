Based on: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html#additive-expression

Only uses backtracking in non factored grammars

**statement_list** = *statement_list* *statement* | *statement*
**statement** = *selection_statement* |  *compound_statement* | *expression_statement* | *declaration*
**selection_statement** = TKIf TKAbrePar *expression* TKFechaPar *statement*
**compound_statement** = TKAbreChaves TKFechaChaves | TKAbreChaves *statement* TKFechaChaves
**expression_statement** = *expression* TKPontoEVirgula | TKPontoEVirgula
**declaration** = *declaration_specifiers* TKPontoEVirgula | *declaration_specifiers* *init_declarator_list* 
**declaration_specifiers** = (type_specifier | type_qualifier | storage_class_specifier) (*declaration_specifiers* | **EMPTY**)
**init_declarator_list** = *init_declarator_list* *init_declarator* TKVirgula | *init_declarator*
**init_declarator** = declarator TKAtrib *expression*
**expression** = *comma*
**comma** = *assignment_expression* , *assignment_expression* 
**assignment_expression** = TKId (TKAtrib | TKAtribAnd | TKAtribDiv | TKAtribLeftShift | TKAtribMais | TKAtribMenos | TKAtribMod |TKAtribOr | TKAtribProd  | TKAtribRightShift) *assignment_expression* | *logical_or* 
**logical_or** = *logical_and* TKOrLog *logical_and* | *logical_and*
**logical_and** = *bitwise_or* TKAndLog *bitwise_or* | *bitwise_or*
**bitwise_or** = *bitwise_xor* TKOr *bitwise_xor* | *bitwise_xor*
**bitwise_xor** = *bitwise_and* TKXOR *bitwise_and* | *bitwise_and*
**bitwise_and** = *relational* TKAnd *relational* | *relational*
**relational** = *relational_operators* (TKEquals | TKNotEqual) *relational_operators* | *relational_operators*
**relational_operators** = *bitwise_shift* (TKLessThan | TKLessOrEqualThan | TKGreaterThan | TKGreaterOrEqualThan) *bitwise_shift* | *bitwise_shift*
**bitwise_shift** = *additive_expression* (TKLeftShift | TKRightShift) *additive_expression* | *additive_expression*
**additive_expression** = *multiplicative_expression* (TKMais | TKSub) *multiplicative_expression* | *multiplicative_expression*
**multiplicative_expression** = *unary_expression* (TKProd | TKDiv | TKMod) *unary_expression* | *unary_expression*
**unary_expression** = (TKAnd | TKSub | TKMais | TKNot | TKBitNot | TKDuploMais | TKDuploMenos | TKProd) *unary_expression* | *postfix_expression*
**postfix_expression** = primary_expression (TKAbreColchete *expression* TKFechaColchete | TKAbrePar TKFechaPar | TKAbrePar *argument_expression_list* TKFechaPar | TKPonto TKId | TKPtrOpr TKId | TKDuploMais | TKDuploMenos) *postfix_expression* | primary_expression
**argument_expression_list** = *expression* TKVirgula *argument_expression_list* | **EMPTY**
**primary_expression** = TKId | TKCteInt | TKConstChar | TKAbrePar *expression* TKFechaPar