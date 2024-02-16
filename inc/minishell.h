#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/includes/libft.h"
# include <signal.h>
# include <string.h>

# define MALLOC_MSG C_RED"Malloc error"CLEAR
# define ERROR_MSG "Bad use: ./minishell"

#define BAD_SYNTAX_EXPORT "Bad syntax on export command"

/*
 * brief	Bool type
*/
typedef enum e_bool			t_bool;

/*
 * brief	The main minishell struct
*/
typedef struct s_msh		t_msh;

/*
 * brief	The command struct
*/
typedef struct s_cmd		t_cmd;

/*
 * brief	Information about the infiles and outfiles
*/
typedef struct s_io_file	t_io_file;

enum e_bool
{
	FALSE = 0,
	TRUE
};

enum
{
	WITH_QUOT = 0,
	WITHOUT_QUOT
};

typedef enum e_file_type
{
	INFILE,
	HERE_DOC,
	TRUNC,
	APPEND
}			t_file_type;

typedef enum e_error
{
	NONE = 0,
	CTR_D,
	SYNTAX
}			t_error;

struct s_msh
{
	/* Variables de entorno. Se acualiza */
	char	**envp;

	/* Input del usuario */
	char	*input;

	/* Lista de comandos */
	t_cmd	*cmds;
	int		cmds_count;

	/* Salida del ultimo comando */
	int		last_out;

	int		end;

	/* Último error */
	t_error	error;
};

struct s_cmd
{
	/* Input divisido por bloques */
	char		**input;

	/* Comando sin flags */
	char		*main;

	/* Argumentos (y flags) del comando*/
	char		**arguments;

	/* Archivos infile */
	t_io_file	*infiles;
	int			infiles_count;

	/* Archivos outfile */
	t_io_file	*outfiles;
	int			outfiles_count;
};

struct s_io_file
{
	char	*name;
	int		type;
	char	*filename;
};

typedef struct s_quotes
{
	int		flag;
	char	type;
}				t_quotes;

/* Init struct msh */
t_msh	*init_msh(char *envp[]);
void	*free_msh(t_msh *data);

/* Bucle */
int		manage(t_msh *data);

/* Parser */
char	*check_quots(t_msh *msh);

char	**divide_cmd_args(char *input, int limit);

/* Matrix utils*/
char	**add_part(char *str, char **mtx);
void	*free_parts(char *part, char **args);
int		matrix_length(char **mtx);

/* Quot utils */
int		is_quot(char *input, int index);

/* Utils */
int		is_space(char ch);
char	*string_add(char *str, char ch);
int		is_redirection(char ch);

/* Parser */
void	*parse(t_msh *msh);
void	*create_commands(t_msh *msh);

void	*add_infile(t_file_type type, char *name, t_cmd *cmd);
void	*add_outfile(t_file_type type, char *name, t_cmd *cmd);
char	**add_command(int start, int end, t_msh *msh, char **res);
void	*check_command(int *index, t_cmd *cmd, t_msh *msh);
void	*check_argument(int *index, t_cmd *cmd, t_msh *msh);
void	*check_infile(int start, int *index, t_cmd *cmd, t_msh *msh);
void	*check_outfile(int start, int *index, t_cmd *cmd, t_msh *msh);

/* Errores */
void	*set_error(t_error error, t_msh *msh);
void	check_error(t_msh *msh);
int		is_error(t_msh *msh);
void	exit_malloc(void);

/* Expand */
# define FIRST_LETTER 1
# define MID_LETTER 2

char	*expand(t_msh *data);
char	*last_state(t_msh *data, int *i);
void	*expand_var(t_msh *msh, int *i, int aux);

/* Errores */
void	*set_error(t_error error, t_msh *msh);
void	check_error(t_msh *msh);
int		is_error(t_msh *msh);

/* build ins*/
void	built_ins(t_msh *msh, int nb_comand);
int		is_builtin(char *cmds);
/* export */
void	bd_export(t_msh *msh, int nb_comand);
void	export_alone(t_msh *msh);
t_bool	check_if_variable_ok(char *arguments);
char	**replace_content(t_msh *msh, char *variable, char *content);
t_bool	check_if_var_exist(t_msh *msh, char *variable);
char	*get_variable(char *arguments);
char	*get_content(char *arguments);
/* */
void	bd_env(t_msh *msh, int nb_comand);
void	bd_echo(t_msh *msh, int nb_comand);
void	*bd_pwd(t_msh *msh, int nb_comand);
void	bd_cd(t_msh *msh, int nb_comand);
void	bd_exit(t_msh *msh, int nb_comand);
void	bd_unset(t_msh *msh, int nb_comand);

/* variables utils */
int		ft_correct_var_char(char c, int flag);
void	*free_expand(char *str1, char *str2);

/* executor */
int		executor(t_msh *msh);
void    exe_built_ins(t_msh *msh);

/* signals */
void	signals_manage(t_msh *msh);

/* Executer */
int	ejecuter(t_msh *msh);

#endif