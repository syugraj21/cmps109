// $Id: server.java,v 1.1 2010-02-23 19:35:28-08 - - $

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
import static java.lang.System.*;

class server {
  static private ServerSocket socket;
  static BlockingQueue<String> main_queue;
  Thread main_server_thread;
  static user_manger list_of_users;
  static output_manger list_of_out;
  
  /* Constructor that starts the main thread
   */ 
  server (chatter.options opts) {
    try{
      main_queue = new LinkedBlockingQueue<String> ();
      list_of_users = new user_manger();
      list_of_out = new output_manger();
      socket = new ServerSocket (opts.server_portnumber);
      main_server_thread = new Thread (new main_server(opts));
      main_server_thread.start();
    } catch(IOException exn){
      System.err.println("Server Error: "+exn);
    } 
  }
  
  /* inner class main_server
   * Accepts new clients and starts the daemon thread
   */
  static class main_server implements Runnable{
    chatter.options opts;
    BlockingQueue <String> source;
    private Socket client;
    main_server (chatter.options opts){
      source = main_queue;
      this.opts = opts;
    }
    public void run(){
      try{
        Thread qeue_man = new Thread(new queue_manger(source));
        qeue_man.setDaemon(true);
        qeue_man.start();
        for(;;){
          client = socket.accept();
          Thread client_thread = new Thread(new new_client(client));
          client_thread.start();  
        }
      } catch (IOException exn){
        System.err.println("Server Error: "+exn);
      }
    }
  }
  
  
  /* inner class new_client
   * Adds the user to the Hashmap and starts the 
   * reader and writer threads
   */ 
  static class new_client implements Runnable{
    private Socket client;
    new_client(Socket s){
      client = s;
    }
    public void run(){
      try{
        Scanner client_input = new Scanner(client.getInputStream());
        String[] get_username = client_input.nextLine().split(":",2);
        list_of_users.insert(get_username[0]);
        Thread reader = new Thread 
          (new receiver(get_username[0],client));
        Thread writer = new Thread (new sender(get_username[0],client));
        reader.start();
        Thread.currentThread().sleep(1000);
        writer.start();
      } catch (InterruptedException exn){
        System.err.println("Server Error: "+exn);
      } catch (IOException exn){
        System.err.println("Server Error: "+exn); 
      }
    }
  }
  
  /* inner class reciever 
   * reads from the client
   */ 
  static class receiver implements Runnable {
    private Socket client;
    private String username;
    receiver (String new_user,Socket s){
      username = new_user;
      client = s;
    }
    public void run () {
      try{
        Scanner input = new Scanner (client.getInputStream());
        while (input.hasNextLine()){
          String message = input.nextLine();
          message.trim();
          message = username +": "+ message;
          put(main_queue,message);
        }
        list_of_users.set_false(username);
      } catch (IOException exn){
        System.err.println("Server Error: " + exn);
      }
    }
  }
  /* inner class sender 
   * sends message to the server
   */ 
  static class sender implements Runnable {
    private Socket client;
    private String username;
    sender(String user, Socket s){
      username = user;
      client = s;
    }
    public void run () {
      try{
         list_of_out.insert(username,client);
        Boolean flag = list_of_users.get_status(username);
        while (flag){
          Thread.currentThread().sleep(1000);
          flag = list_of_users.get_status(username);
        }
        list_of_users.delete(username);
        list_of_out.delete(username);
        client.close();
      } catch (InterruptedException exn){
        System.err.println("Server Error: "+exn);
      }catch (IOException exn){
        System.err.println("Server Error: "+exn);
      }
    }
  }
  
  /* inner class user_manger
   * Manges the hashmap of users
   */ 
  static class user_manger{    
    /*
     * SychronizedMap to prevent multiple threads 
     * from writing onto the hashmap
     */ 
    private Map <String,Boolean> user_map;
    user_manger(){
      user_map=Collections.synchronizedMap
        (new HashMap<String,Boolean>());
    }
    boolean get_status(String username){
      return user_map.get(username); 
    }
    void set_false(String username){
      user_map.put(username,false); 
    }
    void insert(String username){
      user_map.put(username,true);
    }
    void delete(String username){
      user_map.remove(username); 
    }
  }
  /* poll 
   * used to get messages from the Queue
   * code from examples
   */ 
  static String poll (BlockingQueue<String> source, long msec){
    String msg;
    try{
      msg = source.poll(msec, TimeUnit.MILLISECONDS);
    } catch (InterruptedException exn){
      msg = null;
    }
    return msg;
  }
  /* put
   * used to put messages into the queue
   * code from examples
   */ 
  static void put (BlockingQueue<String> source, String msg){
    try{
      source.put(msg);
    } catch (InterruptedException exn){
      //complain to standerd input
      System.err.println("Server Error: " +exn);
    }
  }
  
  /* inner class Queue_manger 
   * prints to all the clients
   */ 
  static class queue_manger implements Runnable{
    BlockingQueue <String> source;
    queue_manger (BlockingQueue<String> source){
      this.source = source;
    }
    public void run(){
      for(;;){
        try{
          String message = poll(source,1000);
          for(Socket client: list_of_out.get_all()){
            PrintWriter out = new PrintWriter(client.getOutputStream());
            out.printf("%s%n",message);
            out.flush();
            out.close();
          }
        } catch (IOException exn){
          System.err.println("Server Error: " +exn); 
        }
      }
    }
  }
    
    /* output_manger
     * manges all the Client Sockets
     */ 
    static class output_manger{
      private Map <String, Socket> output_threads;
      output_manger(){
        output_threads = Collections.synchronizedMap
          (new HashMap <String,Socket>());
      }
      Collection <Socket> get_all(){
        return output_threads.values(); 
      }
      void insert(String username,Socket out){
        output_threads.put(username,out);
      }
      Socket delete(String username){
        //Socket temp = output_threads.get(username);
        return output_threads.remove(username);
      }
    }
  }
  
