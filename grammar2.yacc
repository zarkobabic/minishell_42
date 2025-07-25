  command_line     ::= logical_expr TOKEN_EOF
                    |  TOKEN_EOF
  
  logical_expr     ::= pipeline next_logical_expr

  next_logical_expr ::= (TOKEN_AND | TOKEN_OR) pipeline next_logical_expr
                     |  ε
  
  pipeline         ::= command TOKEN_PIPE pipeline
                    |  command
  
  command          ::= simple_command
                    |  TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
  
  simple_command   ::= command_element simple_command
                    |  command_element 
  
  command_element  ::= TOKEN_IDENTIFIER
                    |  redirection

  redirection_list ::= redirection redirection_list
                    |   ε
  
  redirection      ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER
                    |  TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
                    |  TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER
                    |  TOKEN_HEREDOC TOKEN_IDENTIFIER