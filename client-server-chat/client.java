// $Id: client.java,v 1.1 2013-08-08 14:10:22-07 - - $

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

class client {

   static class make_connection implements Runnable 
   {
      chatter.options opts;
      make_connection (chatter.options opts) {
         this.opts = opts;
      }

      public void run () {
         try {
            Socket socket = new Socket (opts.hostname, opts.portnumber);
            Thread output = new Thread (new writer (opts, 
                            new PrintWriter (socket.getOutputStream ())));
            Thread input  = new Thread (new reader (opts,
                            new Scanner (socket.getInputStream ())));
            output.start ();
            Thread.sleep (1000);
            input.start ();
         }
         catch (IOException exn) {
            out.println ("Client error: class make_connection");
         }
         catch (InterruptedException exn)
         {
            out.println ("Client error: class make_connection");
         }
      }
   }

   // receiver
   static class reader implements Runnable 
   {
      chatter.options opts;
      Scanner scanner;
      reader (chatter.options opts, Scanner scanner) {
         this.opts = opts;
         this.scanner = scanner;
      }

      public void run() {
         out.println ("Starting up client reader ...");
         while (scanner.hasNextLine ()) {
            out.printf ("%s\n", scanner.nextLine ());
         }
         out.println ("Client reader stopped running!");
         exit(1);    
      }
   }
   
   // sender
   static class writer implements Runnable 
   {
      Scanner scanner = new Scanner (System.in);
      chatter.options opts;
      PrintWriter sender;
      writer (chatter.options opts, PrintWriter sender) {
         this.opts = opts;
         this.sender = sender;
      }
      public void run() {
         out.println ("Starting up client writer ...");
         sender.printf ("%s: %n", opts.username);
         sender.flush ();
         while (scanner.hasNextLine()) {
            sender.printf ("\t%s\n", scanner.nextLine());
            sender.flush();
         }
         scanner.close ();
         sender.close ();
         out.println ("Shutting down client writer ...");
      }
   }
}
