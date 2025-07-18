# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    grammar.yacc                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 17:49:20 by zbabic            #+#    #+#              #
#    Updated: 2025/07/17 17:49:47 by zbabic           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Grammar that I will use in the parsing and lexing (In yacc (kinda BNF) notation):
  
  command_line     ::= logical_expr TOKEN_EOF
                    |  TOKEN_EOF
  
  logical_expr     ::= pipeline next_logical_expr

  next_logical_expr ::= (TOKEN_AND | TOKEN_OR) pipeline next_logical_expr
                     |  ε
  
  pipeline         ::= command next_pipeline
  
  next_pipeline    ::= TOKEN_PIPE command next_pipeline
                    |  ε
  
  command          ::= simple_command
                    |  TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
  
  simple_command   ::= command_element command_element_list
  
  command_element_list ::= command_element command_element_list
                        |  ε
  
  redirection_list ::= redirection redirection_list
                    |   ε

  command_element  ::= TOKEN_IDENTIFIER
                    |  redirection
  
  redirection      ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER
                    |  TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
                    |  TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER
                    |  TOKEN_HEREDOC TOKEN_IDENTIFIER
