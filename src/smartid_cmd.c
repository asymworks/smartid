/*
 * Copyright (C) 2014 Asymworks, LLC.  All Rights Reserved.
 *
 * Developed by: Asymworks, LLC <info@asymworks.com>
 * 				 http://www.asymworks.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimers.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimers in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the names of Asymworks, LLC, nor the names of its contributors
 *      may be used to endorse or promote products derived from this Software
 *      without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 */

#include <ctype.h>
#include <string.h>
#include <strings.h>						// strncasecmp

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include "smarti_codes.h"
#include "smartid_conn.h"
#include "smartid_cmd.h"
#include "smartid_logging.h"

//! Smart-I Command Structure
struct smarti_cmd_t
{
	const char *		name;				///< Command Name
	const char *		desc;				///< Command Description

	/**
	 * Command Callback Function
	 * @param[in] Smart-I Connection Structure
	 * @param[in] Smart-I Command Structure
	 * @param[in] Command Parameters
	 */
	void (* func)(smarti_conn_t, struct smarti_cmd_t *, const char *);
};

/**@{
 * @name Smart-I Command Handlers
 */
static void smartid_cmd_close(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_enum(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_help(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_model(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_open(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_serial(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_size(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_token(smarti_conn_t, struct smarti_cmd_t *, const char *);
static void smartid_cmd_xfer(smarti_conn_t, struct smarti_cmd_t *, const char *);
/*@}*/

//! Smart-I Command Table
static struct smarti_cmd_t smarti_cmd_table[] =
{
	{"close",	"Close the current IrDA connection",					smartid_cmd_close},
	{"enum",	"Print the list of IrDA devices found on the bus",		smartid_cmd_enum},
	{"help",	"Print the list of commands and their descriptions",	smartid_cmd_help},
	{"model",	"Print the model name of the open device",				smartid_cmd_model},
	{"open",	"Open a connection to an IrDA device",					smartid_cmd_open},
	{"serial",	"Print the serial number of the open device",			smartid_cmd_serial},
	{"size",	"Print the size of the next transfer in bytes",			smartid_cmd_size},
	{"token",	"Set the token for the next transfer operation",		smartid_cmd_token},
	{"xfer",	"Transfer data from the device",						smartid_cmd_xfer},
	{0,			0,														0}
};

void smartid_cmd_process(smarti_conn_t c, const char * cmd_line, size_t cmd_len)
{
	size_t name_len;
	int i;
	struct smarti_cmd_t * cmd;
	char * p;
	char * name;
	char * params;

	/* Separate Command Name from Parameters */
	cmd_line += strspn(cmd_line, " \t");
	name_len  = strcspn(cmd_line, " \t");

	if (name_len == 0)
	{
		/* No Command Received */
		smartid_conn_send_ready(c);
		return;
	}
	else if (name_len == strlen(cmd_line))
	{
		/* No Parameters Received */
		name = strndup(cmd_line, name_len);
		params = 0;
	}
	else
	{
		/* Split Command and Parameters */
		name = strndup(cmd_line, name_len);
		params = strdup(cmd_line + name_len + 1);
	}

	/* Lower-Case the Command */
	for (p = name ; *p; ++p) *p = tolower(*p);

	/* Process Command */
	smartid_log_debug("Received command '%s'", name);

	for (i = 0; smarti_cmd_table[i].name; i++)
	{
		cmd = & smarti_cmd_table[i];

		if (strncasecmp(name, cmd->name, name_len) == 0)
		{
			smartid_log_debug("Running command '%s'", name);
			cmd->func(c, cmd, params);
			break;
		}
	}

	if (! cmd->name)
	{
		smartid_log_warning("Received unknown command '%s' from %s", name, smartid_conn_client(c));
		smartid_conn_send_responsef(c, SMARTI_ERROR_UNKNOWN_COMMAND, "Unknown Command '%s'", cmd);
	}
}

static void smartid_cmd_close(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_close()");
		return;
	}
}

static void smartid_cmd_enum(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_enum()");
		return;
	}
}

static void smartid_cmd_help(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	int i;

	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_help()");
		return;
	}

	if (params)
	{
		smartid_conn_send_responsef(c, SMARTI_ERROR_INVALID_COMMAND, "Syntax error in %s", cmd->name);
		smartid_log_warning("Syntax error in command '%s': unexpected parameters", cmd->name);
		return;
	}

	for (i = 0; smarti_cmd_table[i].name; i++)
	{
		char buf[251];
		snprintf(buf, 250, "%s\t%s", smarti_cmd_table[i].name, smarti_cmd_table[i].desc);
		smartid_conn_send_response(c, SMARTI_STATUS_INFO, buf);
	}

}

static void smartid_cmd_model(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_model()");
		return;
	}
}

static void smartid_cmd_open(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_open()");
		return;
	}
}

static void smartid_cmd_serial(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_serial()");
		return;
	}
}

static void smartid_cmd_size(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_size()");
		return;
	}
}

static void smartid_cmd_token(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_token()");
		return;
	}
}

static void smartid_cmd_xfer(smarti_conn_t c, struct smarti_cmd_t * cmd, const char * params)
{
	if (! c || ! cmd)
	{
		smartid_log_error("Invalid pointer passed to cmd_xfer()");
		return;
	}
}
