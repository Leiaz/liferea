/*
 * @file liferea_media_player.c  media player helpers
 *
 * Copyright (C) 2012-2015 Lars Windolf <lars.windolf@gmx.de>
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

#include "media_player.h"
#include "media_player_activatable.h"
#include "plugins_engine.h"

#include <libpeas/peas-activatable.h>

struct _LifereaMediaPlayer {
	GtkBox 			parent;
	PeasExtensionSet 	*extensions;
	GSList 			*enclosures;
};

struct _LifereaMediaPlayerClass {
	GtkBoxClass parent_class;
};

G_DEFINE_TYPE (LifereaMediaPlayer, liferea_media_player, GTK_TYPE_BOX)

static void
liferea_media_player_dispose(GObject *gobject)
{
	LifereaMediaPlayer *self = LIFEREA_MEDIA_PLAYER(gobject);

	if (self->extensions)
		g_clear_object (&self->extensions);
	if (self->enclosures) {
		g_slist_free_full (self->enclosures, g_free);
		self->enclosures = NULL;
	}

	/* Chaining dispose from parent class. */
	G_OBJECT_CLASS(liferea_media_player_parent_class)->dispose(gobject);
}

static void
liferea_media_player_class_init (LifereaMediaPlayerClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

	gobject_class->dispose = liferea_media_player_dispose;
}

static void
liferea_media_player_on_extension_added (PeasExtensionSet *extensions,
					PeasPluginInfo 	*info,
					PeasExtension 	*exten,
					gpointer 	user_data)
{
	LifereaMediaPlayer *self = LIFEREA_MEDIA_PLAYER (user_data);
	if (self->enclosures)
		liferea_media_player_activatable_load (LIFEREA_MEDIA_PLAYER_ACTIVATABLE (exten), self->enclosures);
}

static void
liferea_media_player_init(LifereaMediaPlayer *self)
{
	self->extensions = peas_extension_set_new (PEAS_ENGINE (liferea_plugins_engine_get_default ()),
		LIFEREA_MEDIA_PLAYER_ACTIVATABLE_TYPE,
		"plugins-box", GTK_BOX (self),
		NULL);
	liferea_plugins_engine_set_default_signals (self->extensions, NULL);
	g_signal_connect (self->extensions, "extension-added", G_CALLBACK(liferea_media_player_on_extension_added), self);
}

GtkWidget *
liferea_media_player_new(void)
{
	return g_object_new (LIFEREA_TYPE_MEDIA_PLAYER,
		"orientation", GTK_ORIENTATION_VERTICAL,
		NULL);
}

static void
liferea_media_player_load_foreach (PeasExtensionSet *set,
                                   PeasPluginInfo *info,
                                   PeasExtension *exten,
                                   gpointer user_data)
{
	liferea_media_player_activatable_load (LIFEREA_MEDIA_PLAYER_ACTIVATABLE (exten), user_data);
}

static void
liferea_media_player_on_leaving_item_foreach (PeasExtensionSet *set,
					      PeasPluginInfo *info,
					      PeasExtension *exten,
					      gpointer user_data)
{
	liferea_media_player_activatable_on_leaving_item (LIFEREA_MEDIA_PLAYER_ACTIVATABLE (exten));
}

void
liferea_media_player_load (LifereaMediaPlayer *self, GSList *enclosures)
{
	if (!self->extensions)
		return;

	if (self->enclosures) {
		g_slist_free_full (self->enclosures, g_free);
	}
	self->enclosures = g_slist_copy_deep (enclosures, (GCopyFunc) g_strdup, NULL);
	peas_extension_set_foreach (self->extensions,
	                            liferea_media_player_load_foreach, self->enclosures);
}

void
liferea_media_player_on_leaving_item (LifereaMediaPlayer *self)
{
	if (!self->extensions)
		return;

	peas_extension_set_foreach (self->extensions,
	                            liferea_media_player_load_foreach, NULL);
}
