package bgu.spl.net.impl.stomp;

import bgu.spl.net.Frames.Frame;

import java.util.HashMap;
import java.util.Map;

public class User extends Lock {

    private int connectionId;
    private String name;
    private String password;
    private boolean loggedIn = false;
    // id for each genre the user subscribed to.
    private Map<Integer, String> idVSgenre = new HashMap<>();
    private Map<String, Integer> genreVSid = new HashMap<>();


    public User(int connectionId, String name, String password) {
        this.connectionId = connectionId;
        this.name = name;
        this.password = password;
    }

    public void addGenreID(int genreID, String genre) {
        idVSgenre.putIfAbsent(genreID, genre);
        genreVSid.putIfAbsent(genre, genreID);
    }

    public String getGenreByID(int genreID) {
        return idVSgenre.get(genreID);
    }

    public int getSubIDbyGenre(String genre){
        if (genreVSid.containsKey(genre)) {
            return genreVSid.get(genre);
        }
        return -1;
    }

    public String unsubscribe(int genreID) {
        String genre = idVSgenre.get(genreID);
        idVSgenre.remove(genreID);
        genreVSid.remove(genre);
        return genre;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public String getName() {
        return name;
    }

    public String getPassword() {
        return password;
    }

    public boolean isLoggedIn() {
        return loggedIn;
    }

    public Map<Integer, String> getGenreId() {
        return idVSgenre;
    }

    public void logIn() {
        loggedIn = true;
    }

    public void logOut() {
        loggedIn = false;
    }

    public void setID (int id)
    {
        this.connectionId = id;
    }
}
