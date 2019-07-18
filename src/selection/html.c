/*
 * "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
 * (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
 * aplicativos mainframe. Registro no INPI sob o nome G3270.
 *
 * Copyright (C) <2008> <Banco do Brasil S.A.>
 *
 * Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
 * os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
 * Free Software Foundation.
 *
 * Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
 * GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
 * A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
 * obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
 * programa; se não, escreva para a Free Software Foundation, Inc., 51 Franklin
 * St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Este programa está nomeado como - e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 #include <clipboard.h>
 #include <lib3270/selection.h>
 #include <ctype.h>

/*--[ Implement ]------------------------------------------------------------------------------------*/

static void get_element_colors(v3270 * terminal, unsigned short attr, gchar **fgColor, gchar **bgColor)
{
	GdkRGBA *fg;
	GdkRGBA *bg = terminal->color+((attr & 0x00F0) >> 4);

	if(attr & LIB3270_ATTR_FIELD)
		fg = terminal->color+(attr & 0x0003)+V3270_COLOR_FIELD;
	else
		fg = terminal->color+(attr & 0x000F);

	*fgColor = gdk_rgba_to_string(fg);
	*bgColor = gdk_rgba_to_string(bg);

}

/// @brief Get formatted contents as HTML DIV.
static gchar * get_as_div(v3270 * terminal)
{
	GList	* element	= terminal->selection.blocks;
	GString	* string	= g_string_new("");
	gchar 	* bgColor	= gdk_rgba_to_string(terminal->color+V3270_COLOR_BACKGROUND);
	gchar 	* fgColor;

	g_string_append_printf(
		string,
		"<div style=\"font-family:%s,monospace;padding:1em;display:inline-block;background-color:%s\">",
			terminal->font.family,
			bgColor
	);

	g_free(bgColor);

	while(element)
	{
		lib3270_selection * block = ((lib3270_selection *) element->data);
		unsigned int row, col, src = 0;
		unsigned short flags = block->contents[0].flags;

		get_element_colors(terminal,flags,&fgColor,&bgColor);

		g_string_append_printf(
			string,
			"<span style=\"background-color:%s;color:%s\">",
			bgColor,
			fgColor
		);

		g_free(bgColor);
		g_free(fgColor);

#ifdef DEBUG
		g_string_append_c(string,'\n');
#endif // DEBUG

		for(row=0; row < block->bounds.height; row++)
		{
			for(col=0; col<block->bounds.width; col++)
			{
				if(flags != block->contents[src].flags)
				{
					flags = block->contents[src].flags;

					get_element_colors(terminal,flags,&fgColor,&bgColor);

					g_string_append_printf(
						string,
						"</span><span style=\"background-color:%s;color:%s\">",
						bgColor,
						fgColor
					);

					g_free(bgColor);
					g_free(fgColor);


				}

				if(block->contents[src].flags & LIB3270_ATTR_SELECTED && !isspace(block->contents[src].chr))
				{
					g_string_append_c(string,block->contents[src].chr);
				}
				else
				{
					g_string_append(string,"&nbsp;");
				}

				src++;

			}
			g_string_append(string,"<br />");
#ifdef DEBUG
			g_string_append_c(string,'\n');
#endif // DEBUG
		}

		g_string_append(string,"</span>");

		element = g_list_next(element);
	}

#ifdef DEBUG
	g_string_append_c(string,'\n');
#endif // DEBUG

	g_string_append(string,"</div>");

	return g_string_free(string,FALSE);

}

gchar * v3270_get_copy_as_html(v3270 * terminal)
{
	g_autofree char * text = get_as_div(terminal);
	return g_convert(text, -1, "UTF-8", lib3270_get_display_charset(terminal->host), NULL, NULL, NULL);
}
