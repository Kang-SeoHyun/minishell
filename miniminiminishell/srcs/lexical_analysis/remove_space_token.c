#include "../../includes/miniminiminishell.h"

void	remove_space_token(t_token **token_list)
{
	t_token **head;
	t_token	*cur;

	head = token_list;
	cur = *token_list;
	while (cur)
	{
		if (cur->type == TOKEN_TYPE_SPACE)
			delete_token(cur, head);
		cur = cur->next;
	}
}
