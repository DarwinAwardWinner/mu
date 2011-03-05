/*
** Copyright (C) 2010 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**
*/

#ifndef __MU_CONFIG_H__
#define __MU_CONFIG_H__

#include <glib.h>
#include <sys/types.h> /* for mode_t */
#include <mu-msg-fields.h>
#include <mu-util.h>

G_BEGIN_DECLS

#define MU_CONFIG_FORMAT_PLAIN	"plain"    /* plain text output */
#define MU_CONFIG_FORMAT_LINKS	"links"    /* output as symlinks */
#define MU_CONFIG_FORMAT_XML	"xml"      /* output xml */
#define MU_CONFIG_FORMAT_JSON	"json"     /* output json */
#define MU_CONFIG_FORMAT_SEXP	"sexp"     /* output sexps */
#define MU_CONFIG_FORMAT_XQUERY "xquery"   /* output the xapian query */

/* for cfind */
#define MU_CONFIG_FORMAT_MUTT	"mutt"     /* mutt alias style */
#define MU_CONFIG_FORMAT_WL     "wl"       /* Wanderlust address-book */
#define MU_CONFIG_FORMAT_CSV    "csv"      /* comma-sep'd values */
#define MU_CONFIG_FORMAT_ORG_CONTACT "org-contact" /* org-contact */
#define MU_CONFIG_FORMAT_BBDB   "bbdb"     /* BBDB */

enum _MuConfigCmd {
		MU_CONFIG_CMD_INDEX,
		MU_CONFIG_CMD_FIND,
		MU_CONFIG_CMD_CLEANUP,
		MU_CONFIG_CMD_MKDIR,
		MU_CONFIG_CMD_VIEW,
		MU_CONFIG_CMD_EXTRACT,
		MU_CONFIG_CMD_CFIND,
		MU_CONFIG_CMD_NONE,
	
		MU_CONFIG_CMD_UNKNOWN
};
typedef enum _MuConfigCmd MuConfigCmd;


/* struct with all configuration options for mu; it will be filled
 * from the config file, and/or command line arguments */

struct _MuConfig {

		MuConfigCmd      cmd;           /* the command, or
										 * MU_CONFIG_CMD_NONE */
		const char       *cmdstr;       /* cmd string, for user info */
	
		/* general options */
		gboolean		 quiet;			/* don't give any output */
		gboolean		 debug;			/* spew out debug info */
		char			*muhome;		/* the House of Mu */
		gboolean		 version;		/* request mu version */
		gboolean		 log_stderr;	/* log to stderr (not logfile) */
		gchar**	         params;		/* parameters (for querying) */
	
		/* options for indexing */
		char	        *maildir;		/* where the mails are */
		gboolean         nocleanup;		/* don't cleanup deleted mails from db */
		gboolean         reindex;		/* re-index existing mails */
		gboolean         rebuild;		/* empty the database before indexing */
		gboolean         autoupgrade;   /* automatically upgrade db
										 * when needed */
		int              xbatchsize;    /* batchsize for xapian
										 * commits, or 0 for
										 * default */
		int			     max_msg_size;  /* maximum size for message files */
	
		/* options for querying  */
		gboolean         xquery;        /* (obsolete) give the Xapian
										   query instead of search
										   results */
		char			*fields;		/* fields to show in output */	
		char	        *sortfield;		/* field to sort by (string) */
		gboolean		 descending;	/* sort descending? */
		unsigned		 summary_len;	/* max # of lines of msg in summary */
		char            *bookmark;		/* use bookmark */

		char			*formatstr;     /* output type
										 * (plain*,links,xml,json,sexp) */
		
		/* output to a maildir with symlinks */
		char            *linksdir;		/* maildir to output symlinks */
		gboolean		 clearlinks;	/* clear a linksdir before filling */
		mode_t			 dirmode;		/* mode for the created maildir */

		/* options for extracting parts */
		gboolean		*save_all;		/* extract all parts */
		gboolean		*save_attachments;		/* extract all attachment parts */
		gchar			*parts;			/* comma-sep'd list of parts
										 * to save /  open */
		char			*targetdir;		/* where to save the attachments */
		gboolean		 overwrite;		/* should we overwrite same-named files */
		gboolean         play;          /* after saving, try to play (open) the attmnt */ 
};
typedef struct _MuConfig MuConfig;

/**
 * create a new mu config object
 * 
 * set default values for the configuration options; when you call
 * mu_config_init, you should also call mu_config_uninit when the data
 * is no longer needed.
 * 
 * @param opts options 
 */
MuConfig *mu_config_new (int *argcp, char ***argvp)
       G_GNUC_WARN_UNUSED_RESULT;
/**
 * free the MuOptionsConfig structure; the the muhome and maildir
 * members are heap-allocated, so must be freed.
 * 
 * @param opts a MuConfig struct, or NULL
 */
void mu_config_destroy (MuConfig *opts);

/**
 * execute the command / options in this config
 * 
 * @param opts the commands/options
 * 
 * @return a value denoting the success/failure of the execution; MU_CONFIG_RETVAL_OK (0)
 * for success, non-zero for a failure. This is to used for the exit
 * code of the process
 */
MuExitCode mu_config_execute (MuConfig *opts);

G_END_DECLS

#endif /*__MU_CONFIG_H__*/
