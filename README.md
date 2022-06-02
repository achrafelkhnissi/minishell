# Minishell

## Allowed functions
### - [`readline`](https://man7.org/linux/man-pages/man3/readline.3.html)
       readline will read a line from the terminal and return it, using
              prompt as a prompt.  If prompt is NULL or the empty string, no
              prompt is issued.  The line returned is allocated with malloc(3);
              the caller must free it when finished.  The line returned has the
              final newline removed, so only the text of the line remains.
### [`rl_clear_history`](https://tiswww.case.edu/php/chet/readline/readline.html#:~:text=Function%3A%20void-,rl_clear_history,-(void))
       Clear the history list by deleting all of the entries, in the same manner as
       the History library's clear_history() function. This differs from clear_history
       because it frees private data Readline saves in the history list.
### [`rl_on_new_line`](https://tiswww.case.edu/php/chet/readline/readline.html#:~:text=Function%3A%20int-,rl_on_new_line,-(void))
Tell the update routines that we have moved onto a new (empty) line. [`display.c:841`](https://fossies.org/dox/fudgit_2.42beta/display_8c_source.html)
```c 
       rl_on_new_line ()
       {
       if (visible_line)
       visible_line[0] = '\0';
       
       _rl_last_c_pos = _rl_last_v_pos = 0;
       _rl_vis_botlin = last_lmargin = 0;
       return 0;
       }
```
### [`rl_replace_line`](https://tiswww.case.edu/php/chet/readline/readline.html#:~:text=Function%3A%20void-,rl_replace_line,-\(const%20char%20*text)
       Replace the contents of rl_line_buffer with text. The point and mark are preserved,
       if possible. If clear_undo is non-zero, the undo list associated with the current line is cleared.

### [`rl_redisplay`](https://tiswww.case.edu/php/chet/readline/readline.html#:~:text=Function%3A%20void-,rl_redisplay,-(void))
       Change what's displayed on the screen to reflect the current contents of rl_line_buffer.
    
### [`add_history`](https://tiswww.case.edu/php/chet/readline/readline.html#:~:text=you%20must%20call-,add_history(),-to%20save%20the)
### [`printf`](https://man7.org/linux/man-pages/man3/printf.3.html)
### [`malloc`](https://man7.org/linux/man-pages/man3/free.3.html)
### [`free`](https://man7.org/linux/man-pages/man3/free.3p.html)
### [`write`](https://man7.org/linux/man-pages/man2/write.2.html)
### [`access`](https://man7.org/linux/man-pages/man2/access.2.html)
### [`open`](https://man7.org/linux/man-pages/man2/open.2.html)
### [`read`](https://man7.org/linux/man-pages/man2/read.2.html)
### [`close`](https://man7.org/linux/man-pages/man2/close.2.html)
### [`fork`](https://man7.org/linux/man-pages/man2/fork.2.html)
### [`wait`](https://man7.org/linux/man-pages/man2/wait.2.html)
### [`waitpid`](https://man7.org/linux/man-pages/man3/waitpid.3p.html)
### [`wait3, wait4`](https://man7.org/linux/man-pages/man2/wait3.2.html)
### [`signal`](https://man7.org/linux/man-pages/man2/signal.2.html)
### [`sigaction`](https://man7.org/linux/man-pages/man2/sigaction.2.html)
### [`sigemptyset`](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
### [`sigaddset`](https://man7.org/linux/man-pages/man3/sigaddset.3p.html)
### [`kill`](https://man7.org/linux/man-pages/man2/kill.2.html)
### [`exit`](https://man7.org/linux/man-pages/man3/exit.3.html)
### [`getcwd`](https://man7.org/linux/man-pages/man3/getcwd.3.html)
### [`chdir`](https://man7.org/linux/man-pages/man2/chdir.2.html)
### [`stat, lstat, fstat`](https://man7.org/linux/man-pages/man2/lstat.2.html)
### [`unlink`](https://man7.org/linux/man-pages/man2/unlink.2.html)
### [`execve`](https://man7.org/linux/man-pages/man2/execve.2.html)
### [`dup, dup2`](https://man7.org/linux/man-pages/man2/dup.2.html)
### [`pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html)
### [`opendir`](https://man7.org/linux/man-pages/man3/opendir.3.html)
### [`readdir`](https://man7.org/linux/man-pages/man3/readdir.3.html)
### [`closedir`](https://man7.org/linux/man-pages/man3/closedir.3.html)
### [`strerror`](https://man7.org/linux/man-pages/man3/strerror.3.html)
### [`perror`](https://man7.org/linux/man-pages/man3/perror.3.html)
### [`isatty`](https://man7.org/linux/man-pages/man3/isatty.3.html)
### [`ttyname`](https://man7.org/linux/man-pages/man3/ttyname.3.html)
### [`ttyslot`](https://man7.org/linux/man-pages/man3/ttyslot.3.html)
### [`ioctl`](https://man7.org/linux/man-pages/man2/ioctl.2.html)
### [`getenv`](https://man7.org/linux/man-pages/man3/getenv.3.html)
### [`tcsetattr`](https://man7.org/linux/man-pages/man3/tcsetattr.3p.html)
### [`tcgetattr`](https://man7.org/linux/man-pages/man3/tcgetattr.3p.html)
### [`tgetent`](https://www.freebsd.org/cgi/man.cgi?query=tgetent&sektion=3&apropos=0&manpath=freebsd)
### [`tgetflag`](https://linux.die.net/man/3/tgetflag)
### [`tgetnum`](https://linux.die.net/man/3/tgetnum)
### [`tgetstr`](https://linux.die.net/man/3/tgetstr)
### [`tgoto`](https://linux.die.net/man/3/tgoto)
### [`tputs`](https://linux.die.net/man/3/tputs)

## functions usage
- `tcgetattr` `tcsetattr` to block the control caracters affichage.

## PLAN
1. Set up needed function and linked list funcs for the lexter
2. Create a fully functional lexer
3. Create a simple parser to work only with `execve()` \
       - gets the command an optional if provided ex. `echo -n`
       - create a uniq node for every cmd, pipe and redir
4. Test the execution of nonbuild non-complex funtions.

## Questions
1. lexer
2. execution
3. parser

## Problems


## Notes
- In order for `readline` functions to work you need to install it using brew!
       BUT before that you need to update brew or use the following command `brew reinstall openssl@1.1` \
       because brew will fail to install readline for some reason!.
- If you have no more space, check with `du -sh /Users/$(USER)/*`
- if you encountered this error: `error: unknown type name 'FILE'
  FILE *outf;` \
  In your header file! you need to put `stdio.h` file before `readline/readline.h` and `readline/history.h` file \
  because `FILE` is defined in `stdio.h`
- it's a good practice to terminate a 2d array with NULL.
- Export without arguments: prints sorted env var.


## Resources
- [`man bash`](https://www.man7.org/linux/man-pages/man1/bash.1.html)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
