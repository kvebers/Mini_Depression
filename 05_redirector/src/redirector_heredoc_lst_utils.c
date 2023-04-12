/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector_heredoc_lst_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwillert <jwillert@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:49:43 by jwillert          #+#    #+#             */
/*   Updated: 2023/04/12 16:09:29 by jwillert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_heredoc	*heredoc_lst_get_new_node(void)
{
	t_heredoc	*new_node;

	new_node = malloc (sizeof (t_heredoc));
	if (new_node == NULL)
	{
		return (NULL);
	}
	new_node->next = NULL;
	new_node->full_path = NULL;
	new_node->hash = 0;
	new_node->fd = -1;
	return (new_node);
}

t_heredoc	*heredoc_lst_get_and_add_last(t_heredoc *lst_to_expand,
				t_heredoc *node_to_add)
{
	if (lst_to_expand == NULL || node_to_add == NULL)
	{
		return (NULL);
	}
	while (lst_to_expand->next != NULL)
	{
		lst_to_expand = lst_to_expand->next;
	}
	lst_to_expand->next = node_to_add;
	return (node_to_add);
}

t_heredoc	*heredoc_lst_get_by_index(t_heredoc *lst_to_loop, int index)
{
	if (lst_to_loop == NULL)
	{
		return (NULL);
	}
	if (index < 0)
	{
		while (lst_to_loop->next != NULL)
		{
			lst_to_loop = lst_to_loop->next;
		}
		return (lst_to_loop);
	}
	else
	{
		while (lst_to_loop->next != NULL && index > 0)
		{
			lst_to_loop = lst_to_loop->next;
		}
		return (lst_to_loop);
	}
}

t_heredoc	*heredoc_lst_get_and_update_lst(t_data *data)
{
	t_heredoc	*node_to_edit;

	if (data->heredoc == NULL)
	{
		data->heredoc = heredoc_lst_get_new_node();
		node_to_edit = data->heredoc;
	}
	else
	{
		node_to_edit = heredoc_lst_get_and_add_last(data->heredoc,
				heredoc_lst_get_new_node());
	}
	if (node_to_edit == NULL)
	{
		return (NULL);
	}
	if (heredoc_create_file(node_to_edit) == ERROR)
	{
		free(node_to_edit);
		return (NULL);
	}
	return (node_to_edit);
}

void	heredoc_lst_clean(t_data *data)
{
	t_heredoc	*current_node;
	t_heredoc	*next;
	int			index;

	index = 0;
	current_node = data->heredoc;
	if (current_node == NULL)
	{
		return ;
	}
	next = current_node->next;
	while (current_node != NULL)
	{
		next = current_node->next;
		free(current_node->full_path);
		current_node->full_path = NULL;
		free(current_node);
		current_node = next;
		index += 1;
	}
}
