// $Id: server.java,v 1.1 2013-08-08 14:10:22-07 - - $

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
import java.util.concurrent.*;
import static java.lang.System.*;

class server {

   static queue_map map;
   static BlockingQueue <String> buffer;

   chatter.options opts;

   //constructor
   server (chatter.options opts) 
   {
      this.opts = opts;
   }

   static class listener implements Runnable 
   {
      private Socket client;
      String username;
      listener (chatter.options opts, Socket client) {
         this.username = opts.username;
         this.client = client;
      }

      public void run () {
         out.println ("Starting up server listener...");
         try {
            Scanner in = new Scanner (client.getInputStream ());
            map.add_client (username);
            while (in.hasNextLine ()) { 
               String line = in.nextLine().trim();
               if (line.length () == 0) {
                  continue;
               }
               line = username + ": " + line;
               try {
                  buffer.put (line); 
               }
               catch (InterruptedException exn) {
                  out.printf ("Interrupt Server class listener: %s%n", exn); 
               }
            }
            map.set_false (username);
            in.close ();
            client.close (); 
         }
         catch (IOException exn) {
            out.printf ("Server error: class listener: %s%n", exn);
         }
      }
   }

   //reads from the client, prepending the username to the front
   //and then appending the new message to the buffer
   static class receiver implements Runnable 
   {
      private ServerSocket socket;
      private Socket client;
      chatter.options opts;
      receiver (chatter.options opts) {
         this.opts = opts;
      }

      public void run () {
         map = new queue_map ();
         buffer = new LinkedBlockingQueue <String> ();
         try {
            socket = new ServerSocket (opts.portnumber);
            Thread thread = new Thread (new sender (buffer));
            thread.setDaemon (true);
            thread.start();
            out.printf ("Waiting for someone to connect ... %n");
            for (;;) {
               client = socket.accept();
               out.printf ("Socket accepted! %n");
               Thread connect = new Thread (new user_connect (opts, client));
               connect.start ();
            }
         }
         catch (IOException exn) {
            out.printf ("Server error: class receiver: %s%n", exn);
         }
      }
   }

   //accept messages from queue manager, and sending off to client
   static class sender implements Runnable 
   {
      BlockingQueue <String> messages;
      sender (BlockingQueue <String> messages) {
         this.messages = messages;
      }

      public void run () {
         out.println ("Starting up server sender...");
         for (;;) { 
            String line;
            try {
               line = messages.poll (1000, TimeUnit.MILLISECONDS);
            }
            catch (InterruptedException exn) {
               out.printf ("Interrupt Server error: class sender: %s%n", exn);
               line = null;
            }
            if (line == null) continue;
            for (PrintWriter pw : map.queue_values()) {
               pw.printf ("%s%n", line);
               pw.flush();
            }
         }
      }
   }

   static class user_connect implements Runnable 
   {
      chatter.options opts;
      Socket client;
      Scanner input;
      user_connect (chatter.options opts, Socket client) {
         this.opts = opts;
         this.client = client;
      }

      public void run () {
         try {
            input = new Scanner (client.getInputStream ());
            String[] new_user = input.nextLine().split(":", 2);
            opts.username = new_user[0];
            Thread receive = new Thread (new listener (opts, client));
            Thread send    = new Thread (new is_connected (opts, client));
            receive.start ();
            Thread.currentThread ().sleep (500);
            send.start ();
         }
         catch (IOException exn) {
            out.println ("IO server error: class user_connect");
         }
         catch (InterruptedException exn) {
            out.println ("Interrupted server error: class user_connect");
         }
      }
   }

   static class is_connected implements Runnable
   {
      private Socket client;
      private String username;
      is_connected (chatter.options opts, Socket client) {
         this.username = opts.username;
         this.client = client;
      }

      public void run () {
         out.println ("Starting up client sender ...");
         try {
            PrintWriter pw = new PrintWriter (client.getOutputStream ());
            map.add_sender(username, pw);
            while (map.status_client (username)) {
               Thread.currentThread().sleep(500);
            }
            map.del_client (username);
            PrintWriter tmp = map.del_sender (username); 
            tmp.close ();
            pw.close ();
            client.close (); 
         }
         catch (IOException exn) {
            out.printf("IO Server error: class is_connected %s%n", exn);
         }
         catch (InterruptedException exn) {
            out.printf("Interrupt Server error: class is_connected %s%n", exn);

         }
      }
   }


   static class queue_map 
   {
      private static Map <String, Boolean> connected;
      private static Map <String, PrintWriter> queue_writer;
      queue_map () {
         connected = Collections.synchronizedMap
                     (new HashMap <String, Boolean> ());
         queue_writer = Collections.synchronizedMap
                     (new HashMap <String, PrintWriter> ());
      }

      static void add_client (String cli) {
         connected.put (cli, true);
      }

      static void del_client (String cli) {
         boolean bool;
         bool = connected.remove (cli);
      }

      static void set_false (String user) {
         connected.put (user, false);
      }

      static boolean exist_client (String cli) {
         boolean bool;
         bool = connected.containsKey (cli);
         return bool;
      }

      static boolean status_client (String cli) {
         boolean bool = connected.get (cli);
         return bool;
      }

      static void add_sender (String send, PrintWriter pw) {
         queue_writer.put (send, pw);
      }

      static PrintWriter del_sender (String send) {
         PrintWriter pw;
         pw = queue_writer.remove (send);
         return pw;
      }

      static Collection <PrintWriter> queue_values () {
         Collection <PrintWriter> value = queue_writer.values ();
         return value;
      }      
   }
}
