</p>
<p align="center">  
<img src ="https://github.com/achrafelkhnissi/1337/blob/master/Piscine-2021/imgs/IBM_TYPE_704.png">
</p>

# Minishell



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
