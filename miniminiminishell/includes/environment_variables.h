#ifndef ENVIRONMENT_VARIABLES_H
# define ENVIRONMENT_VARIABLES_H

# include <stdlib.h>
# include "../libft/libft.h"

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

t_env_node	*create_env_node(char *str);
void		add_env_node(t_env_node **envp_list, t_env_node *new_envp_node);
t_env_node	*init_env_list(char **envp);
char		*free_env_key_and_get_env_value(t_env_node *env_list, char *env_key);
void		free_env_list(t_env_node *envp_list);
char 		**env_list_to_envp(t_env_node *env_list);

#endif
