NAME		= minishell
	
CC		= cc
CFLAGS	+= -Wall -Wextra -Werror
RM		= rm -rf

UTILS		= $(addprefix utils/, ft_atoi ft_itoa ft_split ft_calloc ft_realloc get_line string string_utils)
LEXER		= $(addprefix lexer/, list lexer lexer_utils)
PARSER		= $(addprefix parser/, parser ast redir_utils parse_core parse_utils)
ENV			= $(addprefix env/, env_list env_utils env_search)
BUILINS		= $(addprefix builtins/, $(ENV) ft_export ft_unset ft_pwd ft_echo echo_utils ft_cd ft_exit ft_env export_utils_0 export_utils_1)
EXEC		= $(addprefix exec/, execute redir pipe find execute_cmd)
FILES		= $(addprefix srcs/, minishell main_utils signals error clean_up syntax $(UTILS) $(LEXER) $(PARSER) $(BUILINS) $(EXEC))

SRC		= $(FILES:=.c)
OBJ		= $(FILES:=.o)
HEADER		= $(addprefix includes/, minishell.h env.h)
INCLUDES	= -I includes
LDFLAGS	+= -lreadline

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

#Debug 
ifeq ($(DEBUG), 1)
   OPTS = -g
endif

.PHONY: all clean fclean re  norm

all: $(NAME)

$(NAME):  $(OBJ) $(HEADER)
	@printf "$(CURSIVE)$(GRAY) 	- Compiling $(NAME)... $(RESET)\n"
	@ $(CC) $(OBJ) $(INCLUDES) $(LDFLAGS) $(OPTS) -o $(NAME)
	@printf "$(GREEN)    - Executable ready.\n$(RESET)"

%.o: %.c $(HEADER)
	@printf "$(CURSIVE)$(GRAY) 	- Making object file $(notdir $@) from source file $(notdir $<) ... $(RESET)\n"
	@ $(CC) $(CFLAGS) $(INCLUDES) $(OPTS) -c $< -o $@

norm:
	@printf "$(CURSIVE)$(GRAY)"
	@norminette
	@printf "$(RESET)"

clean:
	@ $(RM) $(OBJ)
	@printf "$(CURSIVE)$(GRAY)	- Removing object files ... $(RESET)\n"
	@printf "$(YELLOW)    - Object files removed.$(RESET)\n"

fclean: clean
	@ $(RM) $(NAME)
	@printf "$(CURSIVE)$(GRAY)	- Removing $(NAME)... $(RESET)\n"
	@printf "$(YELLOW)    - Executable removed.$(RESET)\n"

push:
	@make fclean
	@git add *
	@git commit -am "OK."
	@git push

lldb:
	@make fclean && make DEBUG=1 && lldb minishell

re: fclean all
