
#gcc compilation aliases

alias gd='gcc -ansi -pedantic-errors -Wall -Wextra -g'
alias gc='gcc -ansi -pedantic errors -Wall -Wextra -DNDEBUG -O3'
alias gd9='gcc -std=c99 -pedantic-errors -Wall -Wextra -g'
alias gc9='gcc -std=c99 -pedantic-errors -Wall -Wextra -DNDEBUG -O3'

alias vlg='valgrind --leak-check=yes -track-origins=yes'


# g++ compilation aliases
alias gc+="g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -std=c++98"
alias gd+="g++ -ansi -pedantic-errors -Wall -Wextra -g -std=c++98"
alias gcnw+="gc+ -Werror"
alias gdnw+="gd+ -Werror"
alias gc+11="g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -std=c++11"
alias gd+11="g++ -ansi -pedantic-errors -Wall -Wextra -g -std=c++11"
alias gcnw+11="gc+11 -Werror"
alias gdnw+11="gd+11 -Werror"

