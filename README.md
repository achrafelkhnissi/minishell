</p>
<p align="center">  
<img src ="https://github.com/achrafelkhnissi/1337/blob/master/Piscine-2021/imgs/IBM_TYPE_704.png">
</p>

<h1 align="center">
	minishell
</h1>
<p align="center">
	<b><i>As beautiful as a shell.. </i></b><br>
</p>

---------------------------------------------------------------------------
<p align="center">
	<img src="https://github.com/achrafelkhnissi/1337/blob/master/Piscine-2021/imgs/GoTTY.png" height=75>
	<h3 align="center">
	<a href="https://www.linkedin.com/in/achrafelkhnissi/">ael-khni</a>
	<span> · </span>
	<a href="https://github.com/JamaaFathiya">fathjami</a>
	</h3>
</p>
													    
-----------------------------------------------------------------------------

***`In this project, you’ll create your own little bash.`*** \
***`This is a step-by-step guide to writing your own simple shell, we’ll provide you with a lot of information based on our own experience.`***

- **What we’ll cover:**
    - readline.
    - pipes & redirections.
    - processes.
    - signals.
    - more…

## Display a prompt:

```c
char *readline (char *prompt);
```

The function `readline()` prints a prompt and then reads and returns a single line of text from the user. The line `readline`returns is allocated with `malloc();` you should `free()` the line when you are done with it.

So this is how you’ll get the user input.

```c
char *line;

line = readline("[minishell][:)]~> ");
```

 

In order to read a line of text from the user. The line returned has the final newline removed, so only the text remains.

If `readline` encounters an `EOF` (Ctrl + D) while reading the line, and the line is empty at that point, then `(char *)NULL` is returned. Otherwise, the line is ended just as if a newline `\n` had been typed.

## Have a working history:

If you want the user to be able to get at the line later, you must call `add_history()` to save the line away in a history list of such lines. By adding this function you’ll be able to navigate through history with up and down and retry some commands.

```c
add_history(*line);
```

At this point, you displayed a prompt with a working history, here are more things you need to check: \
if the line returned contains only spaces and tabs → all you need to do is to display a new prompt. \
if there is something in the line then you’ll add it to your history.

```c
while (1)
	{
			line = readline("[minishell][:)]~> ");
			if (!line)
			{
				printf("exit\n");
				exit(1);
			}
			if (strcmp(*line, "") == EQUAL || ft_strisspace(line))
				return (1);
			if (strlen(line) > 0)
				add_history(line);
	}
```

## Clean your history:

```c
rl_clear_history (void);
```

Clear the history list by deleting all of the entries. \
After getting the command line you need to execute it properly as bash does. \
We’ll give you a brief summary of how you’ll do that. There are 3 steps: 
- Lexer / tokenizer 
- Parser 
- executor
------------------------------------------------
# **Lexer:**

**lexing** or **tokenization** is the process of converting a sequence of characters into a sequence of *lexical tokens* for us we choose to traverse the line returned character by character and tokenize it, tokenizing is basically naming things (ex: this a WORD, this is a PIPE….)  **at this stage we don’t care if it’s a valid command or which command is it**, all we do is discovering what it’s in there.

Technically speaking, let’s see how can we do that:
- We chose to create a doubly-linked list to store our lexer results.

```c
typedef struct s_list
{
	t_elem	*head;
	t_elem	*tail;
	int		  size;
}	t_list;
```

*This is a list element:*

```c
typedef struct s_elem
{
	char			    *content;
	int				    len;
	enum e_token	type;
	enum e_state	state;
	t_elem			  *next;
	t_elem			  *prev;
}	t_elem;
```

**content**: a pointer to the string stored in a node. \
**len:** the content length. \
**type:** the content token.

- enum e_token
    
    ```c
    enum e_token
    {
    	WORD = -1,
    	WHITE_SPACE = ' ',
    	NEW_LINE = '\n',
    	QOUTE = '\'',
    	DOUBLE_QUOTE = '\"',
    	ESCAPE = '\\',
    	ENV = '$',
    	PIPE_LINE = '|',
    	REDIR_IN = '<',
    	REDIR_OUT = '>',
    	HERE_DOC,
    	DREDIR_OUT,
    };
    ```
    
    here we defined the tokens that we’ll need.
    

**state:** we choose to add this information, the content state, if it's inside/outside (double/single) quotes.

- enum e_state
    
    ```c
    enum e_state
    {
    	IN_DQUOTE,
    	IN_QUOTE,
    	GENERAL,
    };
    ```
    

## Explanation:

Here is an example of how a line is tokenized:

</p>
<p align="center">  
<img src ="https://github.com/achrafelkhnissi/1337/blob/master/Piscine-2021/imgs/minishell_lexer.png">
</p>


```c
echo "hello  $USER " > file | grep h | cat << eof | cat >> file | echo 'done'
```

***In details:*** 

- we start with the first character **‘e’** its not a special char `**(|, >, <,>>,<<, $, ‘ ‘ )**`, so we store that pointer then we start counting the length till we find **the first special character** which is in this case **‘WHITE_SPACE’**.
- We store the white space.
- Now we have **‘DOUBLE QUOTES’**  so we change the state from **GENERAL** (which is the default) to **’IN_DQUOUTE’**  and we keep counting till finding the next **‘DOUBLE QUOTES’**, then the state is set back to general.
    - we store the string inside double quotes in a single node and if there is a `$` we store it as an **ENV** token.
- when we find **SINGLE_QUOTES** we store the string inside in a single node without caring if there is a `$` because it will not be expanded.

we keep doing the same thing till the end.

```
💡 It’s better to write all the functions you’ll need related to linked list before you start implementing your lexer.
```

These are the functions we used.

```c
// ----> list.c:
int				  is_empty(t_list *list);
t_list			*init_list(t_list *list);
t_elem			*new_elem(char *content, int len, enum e_token type, enum e_state state);
void        add_tail(t_list *list, t_elem *new);
void			  free_list(t_list *list);
```

Now we have our linked list with all the information needed. 

```
⚠️ Before launching the parser we check if there are any syntax errors, if yes we print an error message, then we display another prompt.
```

-------------------------------------------------------------

# PARSER:

For us, **Parsing** is the process of turning a **linked_list of tokens** into a **command tree.**

We chose to implement an **AST***(abstract syntax tree)* to store the result of the parser and then execute it recursively, here are the structs we used along the way:

- **Tree_struct:**
    
    ```c
    typedef struct s_ast_node
    {
    	enum e_node_type	type;
    	t_union				*content;
    }	t_ast_node;
    
    typedef struct s_ast
    {
    	t_ast_node	*root;	
    }	t_ast;
    ```
    
    <aside>
    ❓ **- Structs:** allocate enough space to store all of the fields in the struct. The first one is stored at the beginning of the struct, the
    second is stored after that, and so on.
    **- Unions:** only allocate enough space to store the largest field listed, and all fields are stored in the same space.
    
    </aside>
    
    We created the struct s_ast to store the root of the AST, and the struct s_ast_node to store a tree node.
    
    ***type:*** 
    
    ```c
    enum e_node_type
    {
    	CMD,
    	PIPE,
    };
    ```
    
    the tree node can be either a command or a pipeline:
    
    ```
    COMMAND#1 | COMMAND#2 | COMMAND#3:
    
                             __ PIPELINE__
                        ___/               \____
                      /                          \
               __ PIPELINE __                  COMMAND #3
             /                \
         COMMAND #1         COMMAND #2
    ```
    
- **Pipe_struct**:
    
    ```c
    typedef struct s_pipe
    {
    	t_ast_node	*left;
    	t_ast_node	*right;
    }	t_pipe;
    ```
    
- **Command_struct:**
    
    ```c
    typedef struct s_cmd
    {
    	char			**args;
    	t_fd			fd;
    	t_redir_list	*redir;
    }	t_cmd;
    ```
    
    **args:** a 2D array to store the arguments for each command with args[0] the command name.
    
    **fd:** a t_fd variable to store the fd_in and fd_out for every command we need to store it when there is a redirection or a pipeline as we’ll see later.
    
    ```c
    typedef struct s_fd
    {
    	int	in;
    	int	out;
    }	t_fd;
    ```
    
    **redir:** a t_redir linked list to store all the redirection related to a single command.
    
    - ***arg:*** a string to store the name of the redirection **file name** for **`(>,<,>>)`**, or **EOF** for `**(<<)**`. *we’ll see this in detail later*.
    - ***type:*** the redirection’s type **`(REDIR_IN, REDIR_OUT, DREDIR_OUT, HERE_DOC).`**
    
    ```c
    typedef struct s_redir_list
    {
    	t_redir_elem	*head;
    	t_redir_elem	*tail;
    	int				size;
    }	t_redir_list;
    
    typedef struct s_redir_elem
    {
    	char			*arg;
    	enum e_token	type;
    	t_redir_elem	*next;
    }	t_redir_elem;
    ```
    

## Explanation:

After we got the lexer list and the syntax is valid we start by parsing the first command.

- if the first list node is **WORD** type we know that it's going to be the command name for sure we store it in args[0] as we said before, and we continue to store the other arguments.
- if we find an **ENV** we expand it and store it in the args array.
    - **HOW?!**
        
        we store the env variables in key/value pairs to make things easier, and we’ve implemented all the functions needed to search/add/delete an env variable and also
        
        functions to convert a list to a 2D array and vice versa.
        
        ```
        💡 you can search in the env array directly.
	    ```
        
- If we find a (single/double) quotes we store all the content inside in args[i], without forgetting to expand env variables if we have double-quotes.
- If we find a redirection **`(>,<,>>)`** we store the type, and the first node of the **(WORD/ENV)** type is stored in the **arg** variable as the **file name.**
    
    ```
    ⚠️ If the arg is an env variable we expand it, if it’s not found we print an **`ambiguous redirect`** error**.**
    ```
    
- If we find **HERE_DOC**:
    - We open a temporary file and we start reading from **`STDIN_FILENO`** into it, and we compare every line entered with the `**EOF**` character specified by the user, then we stop reading when matched.
    
	```
    💡 We should check if the line contains an `**ENV**` so we can expand it.
	```
    
    - We assign the temporary file name to the arg variable so we can treat it as a redirection.
    
    ```
    ⚠️ DON’T forget to unlink the file using the unlink()function, so it can be deleted automatically after being used.
    
    ```
    
- If we find PIPE_LINE:
    - We initialize a **pipe tree_node**.
    - We assign the **tree root pointer** to the **pipe left child** and the **pipe node** becomes the new **tree root**.
    - We parse the next command the same way above to be the **pipe right child**.
    - We repeat the same process ***recursively***.

```
COMMAND: echo hello $USER1 $USER2 > file | cat -e file | grep h

			  __ PIPELINE __
                     ___/                \____
                   /                           \
            __ PIPELINE __                   COMMAND
	  /                \                    |
       COMMAND           COMMAND               ARGS
      /        \             |                  | 
   ARGS       REDIR        ARGS               0: grep
     |          |            |                1: h
   0: echo      >         0: cat
   1: ael-khni 	|	  1: -e
   2: fathjami file       2: file		
```


---------------------------------------------------

# Execution

Typing... 



























## Notes
- In order for `readline` functions to work you need to install it using brew!
       BUT before that you need to update brew or use the following command `brew reinstall openssl@1.1` \
       because brew will fail to install readline for some reason!.
- If you have no more space, check with `du -sh /Users/$(USER)/*`
- if you encountered this error: `error: unknown type name 'FILE'
  FILE *outf;` \
  In your header file! you need to put `stdio.h` file before `readline/readline.h` and `readline/history.h` file \
  because `FILE` is defined in `stdio.h`


## Resources
- [`man bash`](https://www.man7.org/linux/man-pages/man1/bash.1.html)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
