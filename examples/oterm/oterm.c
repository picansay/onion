/*
	Onion HTTP server library
	Copyright (C) 2010 David Moreno Montero

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <onion.h>
#include <onion_handler.h>
#include <onion_handler_path.h>
#include <onion_handler_static.h>

#include "oterm_handler.h"
#include <onion_handler_directory.h>

onion *o=NULL;

void free_onion(){
	fprintf(stderr,"\nClosing connections.\n");
	onion_free(o);
	exit(0);
}

int main(int argc, char **argv){
#ifdef __DEBUG__
	onion_handler *dir=onion_handler_directory(".");
#else
	onion_handler *dir=onion_handler_path("^/$",oterm_handler_index());
	onion_handler_add(dir, onion_handler_path("^/oterm.js$",oterm_handler_oterm()));
#endif

	onion_handler_add(dir, onion_handler_path("^/term$",oterm_handler_data()));
	onion_handler_add(dir, onion_handler_static(NULL,"<h1>404 - File not found.</h1>", 404) );
	
	o=onion_new(O_ONE);
	onion_set_root_handler(o, dir);
	int port=8080;
	if (getenv("ONION_PORT"))
		port=atoi(getenv("ONION_PORT"));
	
	onion_set_port(o, port);
	
	signal(SIGINT, free_onion);
	signal(SIGPIPE, SIG_IGN);
	int error=onion_listen(o);
	if (error){
		perror("Cant create the server");
	}
	
	onion_free(o);
	
	return 0;
}
