/*
 * @file media_player.h  media player helpers
 *
 * Copyright (C) 2012 Lars Windolf <lars.windolf@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LIFEREA_MEDIA_PLAYER_H
#define _LIFEREA_MEDIA_PLAYER_H

#include <glib.h>
#include <gtk/gtk.h>

#define LIFEREA_TYPE_MEDIA_PLAYER (liferea_media_player_get_type ())

#define LIFEREA_MEDIA_PLAYER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIFEREA_TYPE_MEDIA_PLAYER, LifereaMediaPlayer))
#define IS_LIFEREA_MEDIA_PLAYER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIFEREA_TYPE_MEDIA_PLAYER))
#define LIFEREA_MEDIA_PLAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), LIFEREA_TYPE_MEDIA_PLAYER, LifereaMediaPlayerClass))
#define IS_LIFEREA_MEDIA_PLAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LIFEREA_TYPE_MEDIA_PLAYER))
#define LIFEREA_MEDIA_PLAYER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), LIFEREA_TYPE_MEDIA_PLAYER, LifereaMediaPlayerClass))

typedef struct _LifereaMediaPlayer LifereaMediaPlayer;
typedef struct _LifereaMediaPlayerClass LifereaMediaPlayerClass;

GType liferea_media_player_get_type (void);

GtkWidget* liferea_media_player_new (void);

/**
 * liferea_media_player_load:
 * @self: a LifereaMediaPlayer widget
 * @enclosures: (element-type gchar*): a list of enclosure strings
 *
 * Loads a list of enclosures into all enabled LifereaMediaPlayerActivatable extensions.
 */
void liferea_media_player_load (LifereaMediaPlayer *self, GSList *enclosures);

#endif
