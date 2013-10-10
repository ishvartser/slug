// $Id: chatter.java,v 1.1 2013-08-08 14:10:22-07 - - $

/*
 Igor Shvartser, ishvarts@ucsc.edu
 Professor W. Mackey
 CMPS 109: Advanced Programming
 Assignment 5: Threads, Sockets, Client/Server
 August 30, 2013
 */

import java.io.*;
import java.net.*;
import java.util.*;
import static java.lang.System.*;

class chatter {
    
    static void quit (String format, Object... params)
    {
        err.printf (format, params);
        exit (1);
    }
    
    static String ident (options opts) {
        return String.format ("%s: port %d", opts.progname, opts.portnumber);
    }
    
    static String get_jarname()
    {
        String jarpath = getProperty ("java.class.path");
        int lastslash = jarpath.lastIndexOf ('/');
        if (lastslash < 0) return jarpath;
        return jarpath.substring (lastslash + 1);
    }
    
    static class options
    {
        final String progname = get_jarname();
        String traceflags;
        boolean is_server = true;
        String hostname;
        int portnumber;
        String username;
        String message;
        options (String[] args) {
            try {
                //username required for client, but not server
                if (args.length == 0 || args.length > 3) {
                    throw new NumberFormatException();
                }
                for (String arg: args) {
                    //if only port is given, server process started
                    if (args.length == 1 && !arg.contains (":")) {
                        portnumber = Integer.parseInt(args[0]);
                        is_server = true;
                    }
                    //if both hostname & portnumber specified, client process started
                    else if (arg.contains (":")) {
                        String[] input = arg.split(":", 2);
                        hostname = input[0];
                        portnumber = Integer.parseInt(input[1]);
                        is_server = false;
                    }
                    //hostname, portnumber and username specified for client
                    else {
                        if (is_server == false) {
                            break;
                        }
                        portnumber = Integer.parseInt(arg);
                        is_server = true;
                    }
                }
                username = args[args.length - 1];
            }
            catch (NumberFormatException exn) {
                quit ("Usage: %s hostname portnumber%n", progname);
            }
        }
    }
    
    public static void main (String[] args)
    {
        options opts = new options (args);
        
        try {
            //check to see if is_server is true, if yes, create thread
            if (opts.is_server) {
                Thread server = new Thread (new server.receiver (opts));
                server.start();
            }
            //if is_server is false, create thread for client
            if (!opts.is_server) {
                Thread client = new Thread (new client.make_connection (opts));
                client.start();
            }      
        }
        catch (IllegalArgumentException exn) {
            quit ("%s: %s%n", ident(opts), exn.getMessage());
        }
        
    }   
}

