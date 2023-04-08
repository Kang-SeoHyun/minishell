#ifndef TOKEN_H
# define TOKEN_H

# include <stdlib.h>
# include "../libft/libft.h"

typedef enum e_token_type
{
	TOKEN_TYPE_CHUNK,               // 해석의 가능성이 있음, 하나 이상의 CHUNK 또는 TOKEN으로 나뉠 수 있음
	TOKEN_TYPE_ARGV,                // 따옴표 또는 환경변수 처리 완료, 더이상 해석되지 않음
	TOKEN_TYPE_SPACE,               // 공백 ' '
	TOKEN_TYPE_PIPELINE,            // 파이프라인 |
	TOKEN_TYPE_REDIRECTION          // 리다이렉션 <<, <, >, >>
}	t_token_type;

typedef struct s_token
{
	char			*origin_string; // 원형 보존을 위해 필요
	char			*string;        // (따옴표와 환경변수 해석이 완료된) 문자열
	t_token_type	type;
	struct s_token	*next;
}	t_token;

t_token	*create_token(char *string, t_token_type type);
t_token	*add_token(t_token **token_list, t_token *new_token);
t_token	*delete_token(t_token *del_token, t_token **token_list);
void    free_token_list(t_token *token_list);

#endif