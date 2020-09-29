package bgu.spl.net.impl.stomp;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Out.ERROR;
import bgu.spl.net.srv.ConnectionsImpl;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class DataBase {
    // key- genre name, value- list of clients id's subscribed to this genre
    private ConcurrentHashMap<String, LinkedList<Integer>> genrelList = new ConcurrentHashMap<>();
    private final Users users = new Users();
    private ConcurrentHashMap<String , Integer> loggedIn = new ConcurrentHashMap<>();
    private ConnectionsImpl<Frame> connections = null;
    private final AtomicInteger messageID = new AtomicInteger(0);


    public DataBase() {
    }

    void initialConnections(ConnectionsImpl<Frame> connections) {
        if (this.connections == null)
            this.connections = connections;
    }

    private static class DataBaseHolder {
        private static DataBase instance = new DataBase();
    }

    public static DataBase getInstance() {
        return DataBaseHolder.instance;
    }

    public boolean register(int id, String name, String pass) {
        return users.addIfAbsent(id, name, pass);
    }

    public boolean login(int id, String pass, Frame frame, String name) {
        User user;
        users.lockReader();
        user = users.getUser(name);
        if (user == null) {
            String message = "User does not exist";
            error(id, frame, message);
            users.lockReaderRelease();
            return false;
        }
        users.lockReaderRelease();

        //if the user is already logged in
        if (loggedIn.contains(name)) {
            users.lockReader();
            String message = "User already logged in";
            error(id, frame, message);
            users.lockReaderRelease();
            return false;
        }

        //if user's password doesn't match the one that exist
        if (!users.getUser(id).getPassword().equals(pass)) {
            users.lockReader();
            String message = "Wrong password";
            error(id, frame, message);
            users.lockReaderRelease();
            return false;
        }

        loggedIn.putIfAbsent(name,id);
        return true;
    }

    private void error(int id, Frame frame, String message) {
        ERROR error = new ERROR();
        if (frame.getHeader("receipt-id") != null) {
            error.addHeader("receipt-id", frame.getHeader("receipt-id"));
        }
        error.addHeader("message", message);
        error.addHeader("FrameBody", frame.toString());
        connections.send(id, error);
    }

    public void addGenreID(String genre, String subID, int connectionID) {
        users.lockReader();
        users.getUser(connectionID).addGenreID(Integer.parseInt(subID), genre);
        users.lockReaderRelease();
//        int genreID = Integer.parseInt(id);
//        if (genrelList.get(genre) == null) {
//            genrelList.putIfAbsent(genre, new LinkedList<>());
//        }
//        users.lockReader();
//        genrelList.get(genre).add(genreID);
//        users.getUser(connectionID).addGenreID(genreID,genre);
//        users.lockReaderRelease();
    }

    public String unSubscribe(String genreID, int connectionID) {

        return users.getUser(connectionID).unsubscribe(Integer.parseInt(genreID));
//        int genreID = Integer.parseInt(id);
//        users.lockWriter();
//        String genre = users.getUser(connectionID).getGenreByID(genreID);
//        users.getUser(connectionID).unsubscribe(genreID);
//        genrelList.get(genre).remove(genreID);
//        users.lockWriterRelease();
    }

    public int getSubID(String genre, int userID) {
        return users.getUser(userID).getSubIDbyGenre(genre);
    }

    public void addLoggedID(String name , int id)
    {
        loggedIn.putIfAbsent(name,id);
    }

    public boolean doesUserExist(String name)
    {
        return users.getUser(name)!=null;
    }

    public int getUserID(String name)
    {
        return users.getUser(name).getConnectionId();
    }

    public void unRegister(int id)
    {
        users.getUser(id).lockReader();
        for ( String name : loggedIn.keySet())
        {
            if (loggedIn.get(name) == id)
            {
                loggedIn.remove(name);
            }
        }
        users.getUser(id).lockReaderRelease();
    }

    public int getMessageID ()
    {
        int curID = messageID.getAndIncrement();
        return curID;
    }

}
